# Crawl Game Engine - Modder's Quick Start Guide

Your reference for understanding how the game works and making modifications.

---

## How the Game Boots: The Entry Point

**File:** `newgame.cc`

When you start a new game:
1. **Menu system appears** - Player sees list of game modes, species, jobs
2. **Player makes choices** - Navigates through menus (species → job → weapon → name)
3. **Game initializes** - All player stats/equipment set up
4. **Game starts** - Character appears in dungeon

---

## Where to Find What You Want to Modify

### Adding a New Species
**File:** `species.cc` and generated `species-groups.h`

**Steps:**
1. Define the species in `species.cc` (enum + data)
2. Run code generation: `util/species-gen.py` reads data files and generates `species-groups.h`
3. Specify recommended jobs in your species definition

**Key Structure:**
```cpp
struct species_def {
    species_type species;    // Enum identifier
    const char* name;        // Display name
    int xp_mod;             // Experience multiplier
    int hp_mod;             // HP modifier
    int mp_mod;             // MP modifier
    vector<spell_type> library;  // Starting spells
    // ... more properties
};
```

**Example:** To add "Dwarf" species with high HP modifier:
- Add entry to `species.cc` with HP modifier of 50 (tenths of HP)
- List which jobs it recommends (e.g., "Fighter", "Priest")
- Run code generation
- Rebuild

---

### Modifying a Job

**File:** `jobs.cc` and generated `job-groups.h`

**What you can change:**
- Starting stats (Strength, Intelligence, Dexterity)
- Recommended species for that job
- Starting spells in the job's spell library
- Starting equipment
- Starting skills and skill levels
- Weapon selection type (none, plain, or "good" weapons)

**Key Structure:**
```cpp
struct job_def {
    const char* name;
    int s, i, d;                          // Starting Str, Int, Dex
    vector<species_type> recommended_species;
    vector<spell_type> library;           // Starting spells
    vector<string> equipment;             // Starting items
    weapon_choice wchoice;                // Weapon selection
    vector<pair<skill_type, int>> skills; // Starting skills
};
```

**Example:** Make "Warrior" stronger by increasing starting Strength from 10 to 12:
- Edit Warrior job_def in `jobs.cc`
- Change `s` field from 10 to 12
- Rebuild

---

### Adding a New Game Mode

**File:** `newgame.cc` enum `game_type`

**Current modes:** NORMAL, TUTORIAL, SPRINT, HINTS, ZOTDEF, DESCENT, etc.

**What to do:**
1. Add new enum value to `game_type` 
2. Add initialization code in `ng-setup.cc` function `_setup_generic()`
3. The system will:
   - Show it in game mode selection menu
   - Route to appropriate initialization

---

## The Character Creation Flow

When player creates a character, this is what happens:

```
1. CHOOSE GAME MODE
   newgame.cc: _choose_gamemode_map()
   - User picks: NORMAL, SPRINT, TUTORIAL, etc.

2. CHOOSE SPECIES
   newgame.cc: _choose_species()
   - Filtered by game mode restrictions
   - Shows recommended/unrecommended tags
   - Restrictions: ng-restr.h (e.g., Draconian not in Sprint)

3. CHOOSE JOB
   newgame.cc: _choose_job()
   - Filtered by selected species
   - Some jobs banned for some species
   - Sprint bans certain jobs (e.g., JOB_DELVER)

4. CHOOSE WEAPON
   newgame.cc: _choose_weapon()
   - Weapon list filtered by job/species
   - Some jobs get "good" weapons, some get any weapon

5. ENTER NAME
   newgame.cc: _choose_name()
   - Player types name or gets random name
   - Can start pregenerated from preferences

6. GAME INITIALIZATION
   ng-setup.cc: _setup_generic()
   - Give starting spells
   - Give starting equipment
   - Set up dungeon branches
   - Initialize player position

7. GAME STARTS
   - Character appears in Dungeon:1
```

---

## Key Source Files for Modding

| File | Purpose | Modding Use |
|------|---------|------------|
| **species.cc** | Species definitions | Add/modify species |
| **jobs.cc** | Job definitions | Add/modify jobs |
| **newgame.cc** | Character creation flow | Modify menus, add choices |
| **ng-setup.cc** | Game initialization | Modify starting setup |
| **ng-restr.h** | Restrictions (species/job rules) | Restrict combinations |
| **menu.h** | Menu system | Custom menu options |
| **newgame-def.h** | Game config struct | Character data |
| **game-type.h** | Game modes enum | Add new game types |

---

## Common Mods: How-To Examples

### Example 1: Give Elves +1 Intelligence

**File:** `species.cc`

Find the Elf species definition. Elf has starting intelligence modifier. Increase it.

```cpp
// Before: Elf has intelligence bonus of 5 (in tenths)
// After: Change to 15 to give +1.5 extra intelligence
```

### Example 2: Make Warriors Start with a Spear

**File:** `jobs.cc`

Find Warrior job definition, modify `equipment` vector:

```cpp
vector<string> equipment = {
    "hand axe",      // Current starting weapon
    "leather armour"
};

// Change to:
vector<string> equipment = {
    "spear",         // New starting weapon
    "leather armour"
};
```

### Example 3: Add Sprint Mode Restriction

**File:** `ng-restr.h`

Crawl already has sprint restrictions defined. This is where job/species bans are enforced:

```cpp
// Example: Ban Necromancer job in Sprint mode
// (This is already done in source)
```

### Example 4: Add New Starting Spell to Wizard

**File:** `jobs.cc`

Find Wizard job definition, add to `library` vector:

```cpp
// library is vector of starting spells
// Add new spell to the list
library.push_back(SPELL_NEW_SPELL);
```

---

## How the Code is Generated

**Important:** Some files are generated automatically:
- `species-groups.h` - Generated from species data
- `job-groups.h` - Generated from job data

**Generation Process:**
1. You edit source files (species.cc, jobs.cc)
2. Run Python scripts: `util/species-gen.py`, `util/job-gen.py`
3. These read the source and generate the `.h` files
4. These `.h` files are included in compilation
5. Rebuild the game

**Build command:** `make` (this runs generation as part of build)

---

## The Restriction System

**File:** `ng-restr.h`

This defines which species can be which jobs:

```cpp
// Example restrictions:
// - Some species can only be certain jobs
// - Some jobs banned for some species
// - Sprint mode has special restrictions
```

**How it works:**
1. Player selects species
2. System looks up which jobs are allowed for that species
3. Menu shows only allowed jobs (or marks bad ones as restricted)
4. Player can override restrictions in some cases

---

## Where the Game Actually Starts

**File:** `ng-setup.cc`, function `_setup_generic()`

This function is called after character choices are made:
1. Sets up player stats and skills
2. Adds starting equipment
3. Initializes dungeon layout
4. Places player in starting position (Dungeon:1)
5. Game begins

**To add startup logic:**
- Modify `_setup_generic()` in `ng-setup.cc`
- Example: Give special starting items for new species

---

## The Menu System

**File:** `menu.h` and `outer-menu.h`

The character creation uses a hierarchical menu:
1. **MainMenu** - Shows game modes, species, jobs
2. **SubMenu** - Shows details/descriptions
3. **MenuEntry** - Individual selectable item

**To customize:**
- Modify OuterMenu in `outer-menu.cc`
- Add new menu pages
- Change descriptions and filters

---

## Quick Reference: Files to Know

```
Source Layout:
  newgame.cc              <- Character creation logic
  ├─ _choose_species()    <- Species selection menu
  ├─ _choose_job()        <- Job selection menu  
  ├─ _choose_weapon()     <- Weapon selection menu
  └─ _choose_name()       <- Name input

  species.cc              <- Species data definitions
  jobs.cc                 <- Job data definitions
  
  ng-setup.cc             <- Game initialization after char creation
  ├─ _setup_generic()     <- Main init function (called after choices)
  
  ng-restr.h              <- Species/job restrictions
  game-type.h             <- Game mode types
```

---

## Building After Modifications

After you edit species.cc or jobs.cc:

```bash
cd crawl-ref/source
make clean
make
```

The build system automatically:
1. Runs Python code generation
2. Generates species-groups.h and job-groups.h
3. Compiles with new definitions

---

## Summary: 3 Things to Know

1. **Character creation is a menu flow** (species → job → weapon → name)
   - See: `newgame.cc` functions

2. **Species and jobs are defined in data files**
   - See: `species.cc` and `jobs.cc`
   - Automatically generated into headers

3. **After choices, the game initializes**
   - See: `ng-setup.cc` `_setup_generic()`
   - This is where starting equipment is given

**That's all you need to start modding this game engine!**
