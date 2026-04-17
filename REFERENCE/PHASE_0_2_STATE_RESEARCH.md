# Phase 0.2: DCSS Game State Structures - Research Findings

**Status:** ✅ COMPLETE - Research findings documented

---

## Overview

Game state in DCSS is stored primarily in the global `player` object (accessed as `you`). This is a 1000+ line C++ class with 100+ member variables covering:
- Character identity (name, species, job)
- Resources (HP, MP, XP)
- Equipment and inventory
- Skills and training
- God and piety
- Mutations and abilities
- Game progress (level, location, time)
- State flags (pending revival, durations, etc)

---

## Core Player Object: `player` class [player.h:148-1073]

**Location:** `crawl-ref/source/player.h`

**Access:** Global variable `extern player you;` (defined in player.h:145)

**Initialized:** `_setup_generic()` during character creation

### Identity Data (Permanent)

```cpp
string your_name;              // Player name (e.g., "Foo the Barbarian")
species_type species;          // Species enum (SP_HUMAN, SP_ELF, etc)
job_type char_class;           // Job/class enum (JOB_FIGHTER, JOB_WIZARD, etc)

// Saved string copies for save files
string chr_species_name;       // e.g., "Human"
string chr_class_name;         // e.g., "Fighter"
string chr_god_name;           // e.g., "Ashenzari"

time_t birth_time;             // Unix timestamp of character creation
```

### Health & Vitality

```cpp
int hp;                        // Current hitpoints
int hp_max;                    // Max HP (base)
int hp_max_adj_temp;           // Temporary max HP loss (e.g., draining)
int hp_max_adj_perm;           // Permanent max HP loss + background bonus

int magic_points;              // Current mana/magic points
int max_magic_points;          // Max mana
int mp_max_adj;                // Max MP loss

uint8_t max_level;             // Character level (1-27)
int experience_level;          // Experience level (same as max_level usually)
unsigned int experience;       // XP toward next level
unsigned int total_experience; // Total XP ever earned (for scaling)

int gold;                      // Gold carried
```

**Critical for Narrator:**
- Read `hp` and `hp_max` to report player health
- Player's primary resource for combat decisions

### Position & Movement

```cpp
branch_type where_are_you;     // Which dungeon branch (BRANCH_DUNGEON, etc)
int depth;                     // Depth in branch (0 = surface, deeper = more dangerous)
int num_turns;                 // Number of turns elapsed
int exploration;               // Exploration percentage (16.16 fixed point)
int elapsed_time;              // Total game time in auts (100 aut = 1 second)
int elapsed_time_at_last_input;// Used for time display delta
```

**Position on current level:**
- Position is stored separately (not in player object)
- See Phase 0.3 for getting player coordinates

### Inventory & Equipment

```cpp
FixedVector< item_def, ENDOFPACK + 1 > inv;  // Inventory array (52 slots)
player_equip_set equipment;                  // What's equipped where

FixedBitVector<NUM_RUNE_TYPES> runes;       // Collected runes
int obtainable_runes;                        // How many runes can exist

FixedBitVector<NUM_GEMS> gems_found;        // Gems found
FixedBitVector<NUM_GEMS> gems_shattered;    // Gems shattered
```

### Skills & Training

```cpp
FixedVector<uint8_t, NUM_SKILLS> skills;    // Skill levels (1-27)
FixedVector<training_status, NUM_SKILLS> train;  // Training mode for each skill
FixedVector<unsigned int, NUM_SKILLS> training;  // XP% devoted to skill
FixedVector<unsigned int, NUM_SKILLS> skill_points;  // Unallocated points

int skill_cost_level;          // Difficulty scaling for skill training
int experience_pool;           // XP waiting to be applied
int exp_available;             // Scaled XP pool
```

### Spells

```cpp
FixedBitVector<NUM_SPELLS> spell_library;   // All spells player has memorized
FixedBitVector<NUM_SPELLS> hidden_spells;   // Hidden spells (not shown in menu)
FixedVector<spell_type, MAX_KNOWN_SPELLS> spells;  // Current spell slots (25 spells max)

uint8_t spell_no;              // Currently selected spell (quick access)
```

### God & Piety (Religion)

```cpp
god_type religion;             // Which god worshipped (GOD_ASHENZARI, etc)
uint8_t raw_piety;             // 0-200 (stored as 0-200, but displayed 0-6*)
uint8_t piety_hysteresis;      // Docking accumulator
uint8_t gift_timeout;          // Turns until next gift available

FixedVector<uint8_t, NUM_GODS> penance;     // Penance from each god
FixedVector<uint8_t, NUM_GODS> worshipped;  // Gods you've worshipped this game
FixedVector<short, NUM_GODS> num_current_gifts;  // Gifts from each god
FixedVector<short, NUM_GODS> num_total_gifts;    // Lifetime gifts
```

### Mutations & Transformation

```cpp
FixedVector<uint8_t, NUM_MUTATIONS> mutation;       // Mutation levels
FixedVector<uint8_t, NUM_MUTATIONS> innate_mutation;// Permanent mutations
FixedVector<uint8_t, NUM_MUTATIONS> temp_mutation;  // Temporary mutations

transformation form;           // Current form (TRAN_NONE, TRAN_DRAGON, etc)
transformation default_form;   // Default form (usually TRAN_NONE)
bool fishtail;                 // Merfolk fishtail active?

monster_type symbol;           // Player glyph (usually MONS_PLAYER)
```

### Duration Effects

```cpp
durations_t duration;          // Array of duration timers (DUR_*)

// Examples of duration types:
// DUR_HASTE - Haste effect duration
// DUR_SLOW - Slow effect duration  
// DUR_POISON - Poison damage ticks
// DUR_CONFUSION - Confusion effect
// DUR_EXHAUSTED - Exhaustion from haste
// DUR_PARALYSIS - Paralyzed
// etc.
```

**Duration type defined in:** `crawl-ref/source/duration-type.h`

### Turn Control

```cpp
bool turn_is_over;             // Has this turn consumed a full turn?
bool apply_berserk_penalty;    // Apply berserk penalty at turn end?
int berserk_penalty;           // Penalty value
```

**Critical for integration:**
- Set `turn_is_over = true` to end the player's turn
- Set `turn_is_over = false` to allow more actions

### Status Checks (Methods)

```cpp
bool cannot_act();             // Is player incapacitated? (paralyzed, stunned, etc)
bool is_player();              // Always true (for poly code)
int melee_attack_delay();      // Returns delay object for attack speed
bool berserk();                // Is player berserk?
```

### Miscellaneous Important Fields

```cpp
bool wizard;                   // Has player entered wizard mode?
bool explore;                  // Has player entered explore mode?
bool suppress_wizard;          // Suppress wizard mode checks?

int lives;                     // Resurrection lives remaining (from items)
int deaths;                    // Number of deaths this game

bool pending_revival;          // Is player scheduled to revive?

vector<mid_t> beholders;       // Monsters mesmerizing player
vector<mid_t> fearmongers;     // Monsters causing fear

FixedVector<int, NUM_ABILITIES> sacrifice_piety;  // Piety from sacrifices
```

---

## Game State: crawl_state [crawl-state-type.h]

**Location:** `crawl-ref/source/crawl-state-type.h`

**Access:** Global variable `crawl_state`

### Key Fields

```cpp
game_type type;                // GAME_TYPE_NORMAL, GAME_TYPE_SPRINT, etc
game_chapter chapter;          // Game progression phase

bool waiting_for_command;      // Is input waiting?
bool seen_hups;                // Did we receive terminal close signal?
bool terminal_resized;         // Did terminal resize?

string last_game_exit;         // Message from last game exit
game_exit_type last_game_exit_reason;  // Why game ended

input_buffer input;            // Input queue
repeat_mode repeat_cmd;        // Command repeat mode

command_type prev_cmd;         // Previous command

bool mon_acting;               // Is a monster currently acting?
vector<mid_t> mon_acting_list;// Monsters in initiative order
```

---

## Level State: env [env.h]

**Location:** `crawl-ref/source/env.h`

**What it contains:**
- Current dungeon level layout
- Monster list
- Item list on ground
- Clouds/effects
- Line-of-sight cache

**Key for Narrator:** This is where we read dungeon state, monsters, items

**Research note:** Phase 0.3 and beyond will detail this more

---

## Data Struct: item_def [item-def.h]

**Used for:** Inventory items, ground items

**Key fields:**
```cpp
object_class_type base_type;   // OCLASS_WEAPON, OCLASS_ARMOUR, etc
int sub_type;                  // Specific item type
int quantity;                  // How many (usually 1, >1 for ammo/potion)
int plus;                      // Plus values (damage/defense bonus)
int enchantment;               // Enchantment level
string inscription;            // Rune letters and other notes
```

---

## Key Integration Points (for Phase 1)

### Reading Player State

```cpp
// Get player name
string name = you.your_name;

// Get position
int x = you.xpos();  // Method to get X coordinate
int y = you.ypos();  // Method to get Y coordinate

// Get health
int current_hp = you.hp;
int max_hp = you.hp_max;
float health_percent = (float)you.hp / you.hp_max * 100.0f;

// Get location
branch_type branch = you.where_are_you;  // Which dungeon
int depth = you.depth;                   // How deep

// Get spells
for (int i = 0; i < MAX_KNOWN_SPELLS; i++)
{
    if (you.spells[i] != SPELL_NO_SPELL)
        // Player knows this spell
}

// Get mutations
for (int i = 0; i < NUM_MUTATIONS; i++)
{
    if (you.mutation[i])
        // Player has this mutation at level you.mutation[i]
}

// Get durations
if (you.duration[DUR_POISON] > 0)
    // Player is poisoned for you.duration[DUR_POISON] more turns

// Get god info
god_type god = you.religion;
int piety = you.raw_piety;  // 0-200 scale
```

### Writing Player State

```cpp
// End the turn
you.turn_is_over = true;

// Apply damage
you.hp -= 10;
if (you.hp <= 0)
    you.hp = 0;  // Don't go negative (ouch.cc will handle death)

// Add duration
you.duration[DUR_HASTE] = 20;  // 20 more turns of haste

// Apply mutation
you.mutation[MUT_CLAWS] = 3;  // 3 levels of claws

// Add item to inventory
item_def new_item;
new_item.base_type = OBJ_WEAPONS;
new_item.sub_type = WPN_HAND_AXE;
you.inv[0] = new_item;  // Put in first inventory slot
```

---

## Environmental Access: env struct [env.h]

**Global:** `extern crawl_environment env;`

**Key for Narrator:**
```cpp
// Get current level map
dungeon_environment &current_level = env;

// Check what's at a position
const dungeon_feature_type feature = env.grid(x, y);

// List of monsters on current level
monster_list &mons = env.mons;
for (auto m : mons)
{
    if (m != nullptr)
        // This is a valid monster
}

// Items on ground
env.item[x][y]  // Item at position

// Current level ID
level_id current = env.level_id;
```

---

## Coordinate System

**Important:** DCSS uses (x, y) coordinates where:
- x ranges from 0 to (map_width - 1)
- y ranges from 0 to (map_height - 1)
- (0,0) is top-left
- x increases right, y increases down

**Typical dungeon:** 80x70 cells

**Player methods to get position:**
```cpp
int x = you.xpos();
int y = you.ypos();
```

---

## Critical Status Fields for Narrator

**These are the most important fields to track for the narrative AI:**

1. **Health & Life:**
   - `you.hp` - Current hitpoints (integer)
   - `you.hp_max` - Maximum hitpoints

2. **Position:**
   - `you.xpos()` / `you.ypos()` - Current position
   - `you.where_are_you` - Current branch
   - `you.depth` - Current depth

3. **Vitality:**
   - `you.experience_level` - Level (1-27)
   - `you.gold` - Money

4. **Status:**
   - `you.turn_is_over` - Did action consume turn?
   - `you.cannot_act()` - Is player incapacitated?
   - `you.duration[DUR_*]` - Active effect durations

5. **Resources:**
   - `you.magic_points` - Current mana
   - `you.max_magic_points` - Max mana

6. **God:**
   - `you.religion` - Current god
   - `you.raw_piety` - Piety level (0-200)

---

## Next Research Tasks

- **Phase 0.3:** Command execution and input handling
- **Phase 0.4:** Character creation flow (we've studied newgame.cc already)
- **Phase 0.5:** Lua scripting integration
- **Phase 0 Summary:** Consolidate all findings into architecture guide

---

## Files Modified/Examined

- `player.h:1-1073` - Main player class
- `player.h:148+` - player class definition and fields
- `crawl-state-type.h` - Game state structure
- `env.h` - Environment/level state
- `item-def.h` - Item structure
- `duration-type.h` - Duration enum definitions

---

**Research Date:** 2026-04-16
**Checkpoint:** Phase 0.2 Complete

**Key Takeaway:** All player state is in the `you` global object - read it to report state, write it to modify character status, and use `turn_is_over` to control turn consumption.
