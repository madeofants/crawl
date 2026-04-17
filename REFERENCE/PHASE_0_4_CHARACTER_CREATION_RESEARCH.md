# Phase 0.4: DCSS Character Creation Integration - Research Findings

**Status:** ✅ COMPLETE - Research findings documented

---

## Overview

Character creation in DCSS is a multi-stage process:
1. **User choice** - Species, job, name via menu
2. **Validation** - Check if combination is valid
3. **Setup** - Initialize player stats, equipment, skills based on job
4. **Dungeon generation** - Create starting level
5. **First turn** - Ready for gameplay

For the Narrator AI, we need to skip step 1 (menu) and call setup directly with programmatic choices.

---

## Entry Point: startup_step() [startup.cc:1013]

**Location:** `crawl-ref/source/startup.cc:1013-968`

**What it does:**
1. Call `_initialize()` to set up all global systems
2. Show startup menu (unless bypassed)
3. Check for existing save or call `choose_game()` for new character
4. If new character, call `setup_game()` with newgame_def
5. Return true for new game, false for loaded game

**Key code flow:**

```cpp
bool startup_step()
{
    _initialize();  // Initialize all systems
    
    newgame_def choice = Options.game;  // Command-line choices
    crawl_state.type = choice.type;
    
    // Show menu to user (UNLESS bypassed)
    _show_startup_menu(choice, defaults);
    crawl_state.type = choice.type;
    
    // Load existing save OR create new character
    if (save_exists(choice.filename) && restore_game(choice.filename))
    {
        save_player_name();
        return false;  // ← Loaded game
    }
    else if (choose_game(ng, choice, defaults))  // NEW CHARACTER
    {
        setup_game(ng);  // ← This initializes player with ng choices
        newchar = true;
    }
    
    // ... more setup ...
    
    return newchar;  // ← true for new game, false for loaded
}
```

**For Narrator integration:**
- We can skip the menu by setting Options.name_bypasses_menu = true
- Or call `setup_game()` directly with our newgame_def
- `choose_game()` validates the choices and fills in defaults

---

## Game Setup: setup_game() [ng-setup.cc:373]

**Location:** `crawl-ref/source/ng-setup.cc:373-415`

**What it does:**
1. Set game type (normal, sprint, tutorial, etc)
2. Handle seed and randomization
3. Call `_setup_generic()` to initialize player

**Function signature:**
```cpp
void setup_game(const newgame_def& ng,
                bool normal_dungeon_setup = true)
```

**Parameters:**
- `ng` - newgame_def struct with all character choices
- `normal_dungeon_setup` - Whether to generate normal dungeon (vs test mode)

**Key code:**
```cpp
void setup_game(const newgame_def& ng, bool normal_dungeon_setup)
{
    crawl_state.type = ng.type;  // Game type
    
    // Handle seed
    if (Options.seed_from_rc && ng.type != GAME_TYPE_CUSTOM_SEED)
        Options.seed = Options.seed_from_rc;
    
    crawl_state.map = ng.map;  // Sprint map selection
    
    // Initialize hints mode if needed
    switch (crawl_state.type)
    {
    case GAME_TYPE_HINTS:
        init_hints();
        break;
    }
    
    // Do the actual setup
    _setup_generic(ng, normal_dungeon_setup);
}
```

---

## Player Initialization: _setup_generic() [ng-setup.cc:508]

**Location:** `crawl-ref/source/ng-setup.cc:508-700+`

**What it does:**
1. Create fresh player object: `you = player()`
2. Copy name/species/job from ng to `you`
3. Apply species stat bonuses via `species_stat_init()`
4. Apply job stat bonuses via `job_stat_init()`
5. Fix any too-low stats via `_unfocus_stats()`
6. Give starting mutations, items, skills based on job
7. Generate dungeon level
8. Initialize player position on level

**Key code flow:**

```cpp
static void _setup_generic(const newgame_def& ng, bool normal_dungeon_setup)
{
    unwind_bool no_more(crawl_state.show_more_prompt, false);
    
    rng::reset();  // Initialize RNG from seed
    _init_player();  // you = player();
    you.game_seed = crawl_state.seed;
    
    initialise_item_sets();
    shopping_list.refresh();
    
    // Copy choices to player object
    you.your_name = ng.name;
    you.species = ng.species;
    you.char_class = ng.job;
    you.chr_class_name = get_job_name(you.char_class);
    
    // Apply stat modifiers
    species_stat_init(you.species);      // Species bonuses
    update_vision_range();
    job_stat_init(you.char_class);       // Job bonuses
    _unfocus_stats();                    // Fix bad stat combos
    
    // Apply mutations and items
    give_basic_mutations(you.species);   // Species mutations
    give_items_skills(ng);               // Starting items and skills
    roll_demonspawn_mutations();         // Demonspawn extra mutations
    
    if (crawl_state.game_is_sprint())
        _give_bonus_items();
    
    if (crawl_state.game_is_tutorial())
        _setup_tutorial_miscs();
    
    // Setup spells for innate casters
    if (you.has_mutation(MUT_INNATE_CASTER))
        _setup_innate_spells();
    
    // Initialize inventory items properly
    for (int i = 0; i < ENDOFPACK; ++i)
    {
        auto &item = you.inv[i];
        if (!item.defined()) continue;
        item.pos = ITEM_IN_INVENTORY;
        item.link = i;
        item.slot = index_to_letter(...);
        item_colour(item);
    }
    
    // Finalize skills
    calc_total_skill_points();
    update_can_currently_train();
    
    // Generate dungeon
    if (normal_dungeon_setup)
        dungeon_events.clear();
        run_map_preludes();
        generate_dungeon(Options.seed, false);
    
    // Position player on level
    place_player_on_level();
    enter_notes();
}
```

---

## Character Choices: newgame_def struct [ng-input.h]

**Location:** `crawl-ref/source/ng-input.h`

**Structure:**
```cpp
struct newgame_def
{
    string name;               // Player name
    species_type species;      // Species enum (SP_HUMAN, SP_ELF, etc)
    job_type job;              // Job enum (JOB_FIGHTER, JOB_WIZARD, etc)
    
    game_type type;            // Game type (GAME_TYPE_NORMAL, etc)
    string filename;           // Save file path
    string arena_teams;        // Arena mode teams
    string map;                // Sprint map name
    
    // ... other fields ...
};
```

**Enums:**
- **species_type** - Defined in species.h (SP_HUMAN, SP_ELF, SP_DWARF, etc)
- **job_type** - Defined in jobs.h (JOB_FIGHTER, JOB_WIZARD, JOB_ROGUE, etc)
- **game_type** - Defined in game-type.h (GAME_TYPE_NORMAL, GAME_TYPE_SPRINT, etc)

---

## Validation: choose_game() [ng-input.cc]

**Location:** `crawl-ref/source/ng-input.cc`

**What it does:**
1. Shows character creation menu (OR uses passed-in choices)
2. Validates species/job combination
3. Returns true if valid, false if cancelled

**Function signature:**
```cpp
bool choose_game(newgame_def &ng,
                 const newgame_def &defaults,
                 newgame_def &choice)
```

**For Narrator integration:**
- If we pre-fill newgame_def with valid choices
- And set Options.name_bypasses_menu = true
- We can skip the menu entirely
- Then call setup_game() directly

---

## Integration Strategy for Narrator

### Option 1: Direct Call (Simplest)
```cpp
// Fill in character details
newgame_def ng;
ng.name = "Narrator Character";
ng.species = SP_HUMAN;
ng.job = JOB_FIGHTER;
ng.type = GAME_TYPE_NORMAL;
ng.filename = "";

// Setup game directly
setup_game(ng);  // This initializes player and dungeon

// Now player is ready for first turn
// Game continues to _launch_game() which enters main loop
```

### Option 2: Via startup_step()
```cpp
// Set Options to bypass menu
Options.name_bypasses_menu = true;
Options.game.name = "Narrator Character";
Options.game.species = SP_HUMAN;
Options.game.job = JOB_FIGHTER;

// Let startup_step() handle it
bool is_new_game = startup_step();

// Returns to _launch_game() and enters main loop
```

### Option 3: Custom Wrapper
```cpp
// Create wrapper function in our modding layer
bool narrator_setup_character(string name, species_type sp, job_type job)
{
    newgame_def ng;
    ng.name = name;
    ng.species = sp;
    ng.job = job;
    ng.type = GAME_TYPE_NORMAL;
    ng.filename = "";
    
    setup_game(ng);
    return true;
}
```

**Recommendation:** Option 3 (wrapper) is cleanest for external integration

---

## Available Species & Jobs

**Species (partial list - see species.h for full enum):**
```
SP_HUMAN, SP_ELF, SP_DWARF, SP_ORC, SP_HALFLING, 
SP_KOBOLD, SP_GNOME, SP_CENTAUR, SP_MINOTAUR,
SP_MERFOLK, SP_TENGU, SP_FELID, SP_DEMIGOD,
... and more
```

**Jobs (partial list - see jobs.h for full enum):**
```
JOB_FIGHTER, JOB_WIZARD, JOB_ROGUE, JOB_RANGER,
JOB_CLERIC, JOB_NECROMANCER, JOB_MONK, JOB_BERSERKER,
JOB_ENCHANTER, JOB_CONJURER, JOB_FIRE_ELEMENTALIST,
... and more
```

**Query via code:**
```cpp
// Get species name
string name = species::name(SP_HUMAN);  // Returns "Human"

// Get job name  
string name = get_job_name(JOB_FIGHTER);  // Returns "Fighter"

// List all species
for (int i = 0; i < NUM_SPECIES; ++i)
    cout << species::name((species_type)i) << endl;

// List all jobs
for (int i = 0; i < NUM_JOBS; ++i)
    cout << get_job_name((job_type)i) << endl;
```

---

## Game Types

**Available game types (game-type.h):**
```
GAME_TYPE_NORMAL      - Standard dungeon crawl
GAME_TYPE_TUTORIAL    - Beginner tutorial
GAME_TYPE_SPRINT      - Fast-paced race to Orb (shorter dungeon)
GAME_TYPE_HINTS       - Hints mode (beginner-friendly)
GAME_TYPE_DESCENT     - Descent (no regular levels, goes straight down)
GAME_TYPE_CUSTOM_SEED - Normal game with specific seed
GAME_TYPE_ARENA       - Arena battles
```

**For Narrator:**
- Most likely GAME_TYPE_NORMAL
- Could use GAME_TYPE_SPRINT for shorter games
- Tutorial/hints are player-facing features

---

## Character Restrictions & Validation

**Not all species/job combinations are valid:**

```cpp
// From ng-restr.h:
// Valid combos are defined via species-groups.h and job-groups.h (auto-generated)

// Check if combo is valid:
CC_UNRESTRICTED  - No restrictions
CC_RESTRICTED    - Has restrictions (need to check)
CC_BANNED        - Not allowed in this game mode
```

**To validate before setup:**
```cpp
// Check if species/job combo is viable
bool is_valid = is_good_combination(SP_HUMAN, JOB_WIZARD);

// Get restrictions
creature_type_restriction restrict = 
    _get_restriction(ng.species, ng.job);
```

---

## Critical Flow for Integration

```
Narrator AI decides character
    ↓
Create newgame_def struct with choices
    ↓
Call setup_game(ng)
    ↓
_setup_generic() executes:
  - Initialize you object
  - Apply stat modifiers
  - Give starting items/spells
  - Generate dungeon level
  - Place player on level
    ↓
Return to _launch_game()
    ↓
Enter main loop (while(true) _input())
    ↓
Ready for Narrator to issue first command
```

---

## Important Files

- `ng-input.h:1-50+` - newgame_def struct definition
- `ng-input.cc` - Character creation menu and validation
- `ng-setup.cc:373-415` - setup_game() entry point
- `ng-setup.cc:508-700+` - _setup_generic() full implementation
- `startup.cc:1013-968` - startup_step() main startup flow
- `species.h` - Species definitions and enum
- `jobs.h` - Job definitions and enum
- `game-type.h` - Game type enum

---

## Next Research Tasks

- **Phase 0.5:** Lua scripting integration (is there an existing script layer?)
- **Phase 0 Summary:** Write complete DCSS integration architecture guide

---

**Research Date:** 2026-04-16
**Checkpoint:** Phase 0.4 Complete

**Key Takeaway:** Character creation is cleanly separated into `setup_game(newgame_def)`. We can programmatically create characters by filling a newgame_def struct and calling setup_game(). This is perfect for the Narrator AI to create starting characters.
