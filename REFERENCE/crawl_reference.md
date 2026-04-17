# Crawl Game Engine - Menu & Character Creation Reference

## Project Overview
This document analyzes the Crawl roguelike game's menu system and character creation subsystem, serving as a reference for developing our own game engine.

---

## 1. MENU SYSTEM ARCHITECTURE

### 1.1 Core Menu Framework

**Location:** `source/menu.h`, `source/outer-menu.h`

The menu system is built on a **UI widget framework** with the following hierarchy:

#### Menu Components:
- **MenuEntry**: Basic menu item with text, tags, and rendering properties
  - Has color settings (`MENU_ITEM_STOCK_COLOUR = LIGHTGREY`)
  - Supports custom formatting and prefixes
  
- **Menu**: Full menu implementation
  - Inherits from widget system
  - Supports multiple columns, filtering, and custom entry levels
  
- **MenuButton**: Interactive button for non-game menus
  - Properties: `id`, `hotkey`, `highlight_colour`, `description`
  - Events: `on_activate_event()`
  - States: `focused`, `hovered`, `active`
  
- **OuterMenu**: Container menu for managing multiple buttons
  - Grid-based layout system
  - Supports linked menus (up to 4 connected menus)
  - Focus management and scrolling

#### Menu Entry Levels:
```cpp
enum MenuEntryLevel {
    MEL_NONE = -1,
    MEL_TITLE,
    MEL_SUBTITLE,
    MEL_ITEM,
};
```

#### Menu Letter Assignment:
```cpp
struct menu_letter {
    char letter;
    // Cycles: a-z → A-Z → a-z
    menu_letter& operator++();
};

struct menu_letter2 {
    char letter;
    // Cycles: a-z → 0-9 → a-z
};
```

### 1.2 Menu Theming & Rendering

**Key Highlight Colors:**
- **SDL (Tiles):**
  - Normal: `LIGHTGRAY`
  - Bad: `LIGHTGRAY`
  - Control: `LIGHTGRAY`
  - Good: `LIGHTGREEN`

- **Console (ASCII):**
  - Normal: `BLUE`
  - Bad: `LIGHTGRAY`
  - Control: `BLUE`
  - Good: `GREEN`

**Rendering:**
- Widget-based rendering system (via `_render()` override)
- Supports both tile and console modes
- Uses `formatted_string` for rich text with color codes

### 1.3 Platform-Specific UI Implementations

- **Tile Local:** Uses `ShapeBuffer` and `LineBuffer` for rendering
- **Tile Web:** Serializes menu state to JSON for web clients
- **Console/ASCII:** Text-based rendering with color attributes

---

## 2. CHARACTER CREATION FLOW

### 2.1 High-Level Process Flow

```
choose_game()
├── _choose_gamemode_map()         [Sprint/Tutorial mode selection]
├── _choose_seed()                 [Custom seed input]
├── _choose_char()
│   ├── _construct_species_menu()
│   ├── _construct_backgrounds_menu()
│   └── [UI interaction for selection]
├── _choose_name()                 [Character name input]
└── write_newgame_options_file()   [Save preferences]
```

### 2.2 Main Entry Point

**Function:** `bool choose_game(newgame_def& ng, newgame_def& choice, const newgame_def& defaults)`

**Location:** `source/newgame.cc:1008`

**Parameters:**
- `ng`: Output - the final game definition
- `choice`: Input/Output - user's choices (can include random flags)
- `defaults`: Input - default fallback options

**Process:**
1. Clear screen and set text color to LIGHTGREY
2. Copy name and type from choice to ng
3. Handle game mode-specific setup:
   - GAME_TYPE_SPRINT or TUTORIAL → `_choose_gamemode_map()`
   - GAME_TYPE_CUSTOM_SEED → `_choose_seed()`
4. Call `_choose_char()` for species/job selection
5. Call `_choose_name()` if name not provided
6. Validate: non-empty name, valid species/job combo, valid game type
7. Save preferences to file
8. Return false (indicates new character, not restore)

### 2.3 Species and Jobs Selection

**Organization:**
Species and jobs are organized into **groups** for UI presentation:

```cpp
struct species_group {
    const char* name;                    // "Reptiles", etc.
    coord_def position;                  // Relative screen position
    int width;                           // Column width
    vector<species_type> species_list;   // Playable species in group
    void attach(...);                    // Add to menu
};

struct job_group {
    const char* name;                    // "Warrior", "Mage", etc.
    coord_def position;                  // Relative screen position
    int width;                           // Column width
    vector<job_type> jobs;               // Job types in group
    void attach(...);                    // Add to menu
};
```

**Job Groups in Crawl:**
- Warrior: Fighter, Gladiator, Monk, Hunter, Brigand
- Zealot: Berserker, Cinder Acolyte, Chaos Knight
- Adventurer: Artificer, Shapeshifter, Wanderer, Delver
- Warrior-mage: Warper, Hexslinger, Enchanter, Reaver
- Mage: Hedge Wizard, Conjurer, Summoner, Necromancer, Forgewright, Elementalists, Alchemist

### 2.4 Restrictions and Validation

**Random Choice Types:**
```cpp
enum {
    SP_UNKNOWN,    // Not yet chosen
    SP_RANDOM,     // Completely random
    SP_VIABLE,     // Random from viable combinations
};

enum {
    JOB_UNKNOWN,
    JOB_RANDOM,
    JOB_VIABLE,
};
```

**Restriction Check Functions:**
- `species_allowed(job, species)` → Returns restriction level
- `job_allowed(species, job)` → Returns restriction level
- `_job_allowed(species_type sp, job_type job)` → Includes sprint-specific rules

**Restriction Levels:**
```cpp
enum char_choice_restriction {
    CC_BANNED,        // Species/job combo not allowed
    CC_RESTRICTED,    // Special restrictions apply
    CC_ALLOWED        // Free to use
};
```

### 2.5 UI Menu Construction for Species

**Function:** `_construct_species_menu(const newgame_def& ng, const newgame_def& defaults, UINewGameMenu* ng_menu)`

**Logic:**
1. Iterate through all `species_groups`
2. Filter groups: only show if job is unknown OR group has at least one playable species for current job
3. Call `group.attach()` to add group to menu
4. Each group gets menu letters (a, b, c, ...) automatically assigned

**Filter Condition:**
```cpp
if (ng.job == JOB_UNKNOWN || 
    any_of(group.species_list.begin(), group.species_list.end(),
           [](species_type sp) { return species_allowed(ng.job, sp) != CC_BANNED; }))
```

### 2.6 UI Menu Construction for Jobs/Backgrounds

**Function:** `_construct_backgrounds_menu(...)`

**Similar to species menu:**
1. Iterate through `jobs_order[]` (predefined job groups)
2. Show job groups only if they have at least one playable job for current species
3. Attach group to menu with auto-incrementing letters

---

## 3. NEWGAME DEFINITION STRUCTURE

### 3.1 Core Data Structure

**Location:** `source/newgame-def.h`

```cpp
struct newgame_def {
    // Character identity
    string name;
    
    // Game configuration
    game_type type;        // NORMAL, SPRINT, TUTORIAL, CUSTOM_SEED, ARENA
    string filename;       // Save file name
    uint64_t seed;         // RNG seed for procedural generation
    bool pregenerate;      // Pre-generate all dungeon levels
    
    // Map/Mode specific
    string map;            // Sprint map name (or "random")
    string arena_teams;    // Arena team configuration
    
    // Allowed combinations (for UI validation)
    vector<string> allowed_combos;
    vector<species_type> allowed_species;
    vector<job_type> allowed_jobs;
    vector<weapon_type> allowed_weapons;
    
    // Character definition
    species_type species;  // Chosen or random indicator
    job_type job;          // Chosen or random indicator
    weapon_type weapon;    // Starting weapon choice
    
    // Full randomization flag
    bool fully_random;     // Entire character chosen randomly in one step
    
    // Methods
    void write_prefs(FILE *f) const;
    void clear_character();  // Reset species, job, weapon to UNKNOWN
};
```

### 3.2 Equality Comparison

Only considers values relevant for serialization:
- name, type, seed, pregenerate, map, arena_teams
- species, job, weapon, fully_random

---

## 4. STARTING WEAPON SELECTION

**Function:** `weapon_type starting_weapon_upgrade(weapon_type wp, job_type job, species_type species)`

**Purpose:** Upgrade starting weapons based on species/job synergies (e.g., Ogre gets larger weapons)

---

## 5. GAME SETUP PROCESS

### 5.1 Setup Flow

**Main Entry:** `void setup_game(const newgame_def& ng, bool normal_dungeon_setup=true)`

**Location:** `source/ng-setup.cc`

**Process:**
1. `_init_player()` - Initialize player object to defaults
2. `give_items_skills(ng)` - Distribute starting items and skills
3. `initial_dungeon_setup()` - Initialize dungeon structure

### 5.2 Player Initialization

**Function:** `_init_player()`

```cpp
you = player();                    // Reset to default player object
dlua.callfn("dgn_clear_persistant_data", "");  // Clear persistent dungeon data
```

### 5.3 Starting Items Creation

**Function:** `item_def* newgame_make_item(object_class_type base, int sub_type, int qty=1, int plus=0, int force_ego=0, bool force_tutorial=false)`

**Features:**
- Finds first available inventory slot
- Auto-equips relevant items
- Auto-memorizes first spell from spell books
- Skips item creation in tutorial mode (unless `force_tutorial=true`)
- Tries alternative sub_types if primary is useless
- Returns pointer to created item (or nullptr if failed)

**Example Sprint Bonus Items:**
```cpp
newgame_make_item(OBJ_POTIONS, POT_CURING);
newgame_make_item(OBJ_POTIONS, POT_HEAL_WOUNDS);
newgame_make_item(OBJ_POTIONS, POT_HASTE);
newgame_make_item(OBJ_POTIONS, POT_MAGIC, 2);
newgame_make_item(OBJ_POTIONS, POT_BERSERK_RAGE);
newgame_make_item(OBJ_SCROLLS, SCR_BLINKING);
```

### 5.4 Starting Stat Distribution

**Minimum Start Stat:** 3

**Function:** `_unfocus_stats()` - Ensures no stat drops below minimum

**Logic:**
1. Check each stat against MIN_START_STAT
2. If stat too low, reduce two other stats equally to compensate
3. Prevents extremely weak characters (e.g., Ghost Berserker)

### 5.5 Dungeon Initialization

**Function:** `void initial_dungeon_setup()`

Initializes:
- Branch entry points and depths
- Temple layout (ecumenical temple + overflow temples)
- Monster spawning zones
- Item distribution tables
- Unique monster placements

---

## 6. MENU OPTIONS & SPECIAL SELECTIONS

### 6.1 Menu Special Options

**Enum:** `MenuOptions` (values 0-8)

```cpp
M_QUIT = -1              // User quit
M_ABORT = -2             // Abort character creation
M_APTITUDES = -3         // Show aptitude screen
M_HELP = -4              // Show help
M_VIABLE = -5            // Reset to recommended (viable) character
M_RANDOM = -6            // Random character
M_VIABLE_CHAR = -7       // Mark choice as viable random
M_RANDOM_CHAR = -8       // Mark choice as fully random
M_DEFAULT_CHOICE = -9    // Use default from options
```

### 6.2 Random Character Generation

**Three Modes:**
1. **Fully Random:** Both species and job are random
2. **Viable Random:** Species and job are both "viable" (balanced combinations)
3. **Semi-Random:** User picks species, job randomized (or vice versa)

**Functions:**
- `_is_random_species(sp)` - True if SP_RANDOM or SP_VIABLE
- `_is_random_job(job)` - True if JOB_RANDOM or JOB_VIABLE
- `_is_random_choice(ng)` - True if both are random types
- `_is_random_viable_choice(ng)` - True if random + viable

---

## 7. SEEDING & GAME CONFIGURATION

### 7.1 Seed Input

**Function:** `_choose_seed(newgame_def& ng, newgame_def& ng_choice, const newgame_def& defaults)`

**Features:**
- Text input field for 64-bit unsigned integer seed
- Clear button (hotkey: `-`)
- "Today's daily seed" button (hotkey: `d`) - uses YYYYMMDD format
- Paste from clipboard support (hotkey: `p` or `Ctrl+V`)
- Validation for seed format
- Information about what seed affects

**Pregeneration Toggle:**
- Option to fully pregenerate dungeon at startup
- Affects performance: slower startup, faster gameplay

### 7.2 Sprint/Tutorial Mode Setup

**Function:** `_choose_gamemode_map(newgame_def& ng, newgame_def& ng_choice, const newgame_def& defaults)`

Handles mode-specific configuration (not detailed in current code sections)

---

## 8. NAME GENERATION & SELECTION

### 8.1 Random Name Generation

**Function:** `string newgame_random_name()`

**Location:** `source/newgame.cc`

Generates random player names (mechanism not detailed in current sections)

### 8.2 Name Validation

**Function:** `bool is_good_name(const string& name, bool allow_cap_letters)`

**Requirements:**
- Non-empty name
- Valid character set
- Optional capitalization rules

### 8.3 Name Input

**Function:** `_choose_name(newgame_def& ng, newgame_def& ng_choice)`

**Process:**
1. Prompt for player name
2. Optionally allow random generation
3. Validate name
4. Store in `ng.name`

---

## 9. PREFERENCES PERSISTENCE

### 9.1 Saving Character Creation Options

**Function:** `void write_newgame_options_file(const newgame_def& choice)`

Saves user's character creation choices to a configuration file for future defaults.

**Saved Values:**
- Character name (if remembered)
- Last chosen species
- Last chosen job
- Last chosen weapon
- Seed (if used)
- Game type
- Map selection (for sprint)

---

## 10. KEY FILES & LOCATIONS

| File | Purpose |
|------|---------|
| `source/newgame.cc` | Main character creation logic (2044 lines) |
| `source/newgame.h` | Character creation declarations |
| `source/newgame-def.h` | Game definition data structure |
| `source/ng-setup.cc` | Game initialization (21015 lines) |
| `source/ng-setup.h` | Setup function declarations |
| `source/ng-init.cc` | Non-player dungeon initialization |
| `source/ng-input.cc` | User input handling for char creation |
| `source/ng-restr.cc` | Species/job restriction rules |
| `source/ng-wanderer.cc` | Wanderer job special handling |
| `source/menu.h` | Menu UI framework |
| `source/outer-menu.h` | Outer menu container widget |
| `source/menu-type.h` | Menu type definitions |
| `source/job-groups.h` | Job grouping definitions |
| `source/species-groups.h` | Species grouping definitions |

---

## 11. DESIGN PATTERNS & BEST PRACTICES

### 11.1 Separation of Concerns

1. **Character Definition** (`newgame_def`) - Data container
2. **Choice Handling** - Selection logic with restrictions
3. **UI Rendering** - Menu system and display
4. **Validation** - Restriction checking
5. **Setup** - Game initialization

### 11.2 Menu Construction Pattern

1. Define groups with display metadata (name, position, width)
2. Filter groups based on current selections
3. Use menu_letter for automatic key assignments
4. Attach filtered items to menu widget
5. Run UI event loop for user selection

### 11.3 Random Generation Pattern

Three levels of control:
- **Full User Control:** Species and job both specified
- **Hybrid Random:** One random, one user-specified
- **Viable Random:** Random from balanced combinations only
- **Fully Random:** Everything random

### 11.4 Starting Item Pattern

```cpp
1. Define item by class and sub-type
2. Specify quantity and modifiers
3. Call newgame_make_item()
4. Item auto-placed in inventory
5. Equipment auto-equipped if applicable
6. Spells auto-memorized if from book
```

---

## 12. IMPLEMENTATION NOTES FOR OUR GAME

### Key Takeaways:
1. **Modularity:** Separate data structures from UI rendering
2. **Flexibility:** Support multiple random modes (fully random, viable, semi-random)
3. **Persistence:** Remember user preferences
4. **Validation:** Centralize species/job restriction logic
5. **UI Framework:** Use a widget-based approach for menus
6. **Group Organization:** Organize complex choices (species, jobs) into semantic groups
7. **Seeding:** Support reproducible gameplay through seed management
8. **Hot Keys:** Assign keyboard shortcuts automatically or manually

---

**Document Generated:** 2026-04-16  
**Analysis Scope:** Crawl Character Creation & Menu Systems  
**Lines of Code Analyzed:** ~2000+ lines from core files
