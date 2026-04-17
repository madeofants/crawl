# Crawl Menu & Character Creation - Quick Reference Card

## 🎮 Character Creation Flow (30 seconds)

```
START → Game Type → Species (filtered by job) → Job (filtered by species) 
  → Name Input → Validation → Save Prefs → READY TO PLAY
```

---

## 📊 Key Data Structure: newgame_def

```cpp
struct newgame_def {
    // Identity
    string name;              // Player name
    
    // Game config
    game_type type;           // NORMAL, SPRINT, TUTORIAL, CUSTOM_SEED, ARENA
    uint64_t seed;            // RNG seed
    bool pregenerate;         // Pre-gen dungeon?
    
    // Character
    species_type species;     // SP_UNKNOWN, SP_HUMAN, ... SP_RANDOM, SP_VIABLE
    job_type job;             // JOB_UNKNOWN, JOB_FIGHTER, ... JOB_RANDOM, JOB_VIABLE
    weapon_type weapon;       // Starting weapon
    
    // Constraints
    vector<species_type> allowed_species;
    vector<job_type> allowed_jobs;
    
    // Flags
    bool fully_random;        // Entire char random in one step?
};
```

---

## 🎯 5 Core Functions

| Function | Purpose | Location |
|----------|---------|----------|
| `choose_game(ng, choice, defaults)` | Main orchestrator - runs full character creation | newgame.cc:1008 |
| `_choose_char(ng, choice, defaults)` | Species + Job selection with filtering | newgame.cc:~700 |
| `_choose_name(ng, choice)` | Player name input | newgame.cc:~500 |
| `setup_game(ng)` | Post-creation: init player, items, skills, dungeon | ng-setup.cc:~100 |
| `newgame_make_item(base, sub_type, qty, plus, ego)` | Create starting item + auto-equip | ng-setup.cc:96 |

---

## 🎛️ Random Modes

| Mode | Species | Job | Use Case |
|------|---------|-----|----------|
| **Full Control** | Specific (e.g., Human) | Specific (e.g., Fighter) | Experienced players |
| **Viable Random** | SP_VIABLE | JOB_VIABLE | Recommended for new players |
| **Fully Random** | SP_RANDOM | JOB_RANDOM | Challenge runs |
| **Semi-Random** | Specific | JOB_RANDOM (or vice versa) | Mixed control |

**Hotkeys:**
- `(M)` → Mark viable random
- `(R)` → Mark fully random
- `(?)` → Show help

---

## 🧬 Species Groups

```
Humanoid     Reptile       Draconian    Demonic      Undead       Magical
├─ Human     ├─ Kobold     ├─ Dragon    ├─ Felid     ├─ Mummy     ├─ Vine Stalker
├─ Half-Elf  ├─ Trog       └─...       ├─ Orc       ├─ Skeleton  ├─ Formicid
└─ Dwarf     └─ Merfolk               ├─ Minotaur  ├─ Vampire   ├─ Barachi
                                       └─ Troll     └─ Ghost     └─ Gargoyle
```

---

## 💼 Job Groups (5 Categories, 26 Total Jobs)

| Warrior (5) | Zealot (3) | Adventurer (4) | Warrior-Mage (4) | Mage (10) |
|-------------|-----------|----------------|------------------|-----------|
| Fighter | Berserker | Artificer | Warper | Hedge Wizard |
| Gladiator | Cinder Acolyte | Shapeshifter | Hexslinger | Conjurer |
| Monk | Chaos Knight | Wanderer | Enchanter | Summoner |
| Hunter | | Delver | Reaver | Necromancer |
| Brigand | | | | + 5 more mages |

---

## ✅ Validation Rules

**Restriction Levels:**
```cpp
enum char_choice_restriction {
    CC_BANNED = no combo allowed
    CC_RESTRICTED = special rules apply
    CC_ALLOWED = free to use
};
```

**Example Rules:**
- JOB_DELVER banned in Sprint mode
- Some species/job combos fundamentally broken
- Viable combos recommended for beginners

**Check Function:**
```cpp
char_choice_restriction result = job_allowed(species, job);
if (result == CC_BANNED) reject_combo();
```

---

## 🎨 UI Components

| Component | Purpose |
|-----------|---------|
| **MenuButton** | Clickable button with id, hotkey, highlight, description |
| **OuterMenu** | Grid layout container for buttons, manages focus |
| **Menu** | Standard menu with entry levels (TITLE/SUBTITLE/ITEM) |
| **menu_letter** | Auto-assigns keyboard shortcuts a-z → A-Z → a-z |

---

## 🎮 Platform Colors

**SDL/Tiles:**
- Normal: LIGHTGRAY
- Good: LIGHTGREEN
- Bad: LIGHTGRAY
- Control: LIGHTGRAY

**ASCII/Console:**
- Normal: BLUE
- Good: GREEN
- Bad: LIGHTGRAY
- Control: BLUE

---

## 📝 Menu Letter Assignment

```
menu_letter:    a, b, c, ..., z, A, B, ..., Z, a, b, ...
                (lowercase → UPPERCASE → repeat)

menu_letter2:   a, b, c, ..., z, 0, 1, ..., 9, a, b, ...
                (lowercase → digits → repeat)
```

---

## 🔐 Seeding Options

**Input Methods:**
- Manual: Type 64-bit unsigned integer
- Daily: Press `[d]` → Uses YYYYMMDD format
- Paste: Press `[p]` or `[Ctrl+V]` → From clipboard
- Clear: Press `[-]` → Empty input field

**Affects:**
- Dungeon layout
- Monster placement & stats
- Item generation
- NPC behavior
- Game version-dependent (upgrades may affect seeding)

---

## 🔧 Game Setup Pipeline

```
1. setup_game(newgame_def ng)
   ├─ _init_player()            → Reset to defaults
   ├─ give_items_skills(ng)     → Distribute starting loot
   │  ├─ newgame_make_item()    → Create each item
   │  ├─ Auto-equip equipment
   │  └─ Auto-memorize spells
   ├─ _unfocus_stats()          → Rebalance (min 3 per stat)
   └─ initial_dungeon_setup()   → Init branches, temples, etc.

2. Game is ready to play
```

---

## 💡 Design Patterns

### Pattern 1: Cascading Filters
```
User picks Species A
  ↓
Filter Job menu to show only jobs compatible with A
User picks Job B
  ↓
Validate Species A + Job B combo
```

### Pattern 2: Menu Letter Cycling
```
Define menu_letter class with operator++
Assign a, b, c, ... automatically to items
Cycle through lowercase, uppercase, digits as needed
```

### Pattern 3: Preference Persistence
```
Get user choice
  ↓
Save to newgame_options_file
  ↓
On next run, load_newgame_options_file
  ↓
Use as defaults
```

### Pattern 4: Widget Platform Abstraction
```
Abstract Widget base class
  ├─ _render() (abstract)
  ├─ _get_preferred_size() (abstract)
  └─ _allocate_region() (abstract)
      ├─ SDL implementation
      ├─ Web/JSON implementation
      └─ ASCII/Console implementation
```

### Pattern 5: Random Mode Flagging
```
if (choice.fully_random) {
    // Entire character chosen randomly
    if (choice.species == SP_VIABLE)
        pick_viable_random_char();
    else
        pick_fully_random_char();
}
```

---

## 📂 Key Files at a Glance

| File | Lines | Purpose |
|------|-------|---------|
| newgame.cc | 2,044 | Character creation orchestrator |
| ng-setup.cc | 21,015 | Game initialization |
| newgame.h | ~100 | Character creation declarations |
| newgame-def.h | ~72 | Game definition struct |
| menu.h | ~500+ | Menu framework |
| outer-menu.h | ~110 | Container menu widget |
| ng-input.cc | ~100 | Input handling |
| ng-restr.cc | ~100 | Restriction checking |

---

## 🚀 Implementation Checklist

- [ ] Define your species/job taxonomy
- [ ] Create restriction matrix (allowed combos)
- [ ] Build game_def equivalent struct
- [ ] Implement widget base class
- [ ] Create button + menu UI components
- [ ] Build character selection UI (species + job)
- [ ] Add name input screen
- [ ] Implement validation (like job_allowed)
- [ ] Add starting items system
- [ ] Create preferences save/load
- [ ] Add random generation modes
- [ ] Implement seeding system
- [ ] Build dungeon initialization

---

## 💬 Quick Code Snippets

**Check if combo is allowed:**
```cpp
if (job_allowed(species, job) == CC_BANNED) {
    // Invalid combo
}
```

**Create starting item:**
```cpp
newgame_make_item(OBJ_WEAPONS, WPN_HAND_AXE, 1);
// Auto-equipped to inventory
```

**Mark viable random:**
```cpp
ng_choice.fully_random = true;
ng_choice.species = SP_VIABLE;
ng_choice.job = JOB_VIABLE;
```

**Cycle menu letters:**
```cpp
menu_letter letter;  // Starts at 'a'
++letter;  // Now 'b'
++letter;  // Now 'c'
// ... continues a-z, A-Z, a-z, ...
```

**Save preferences:**
```cpp
newgame_def choice;
choice.species = SP_HUMAN;
choice.job = JOB_FIGHTER;
write_newgame_options_file(choice);
```

---

## 📖 When to Refer Back

| Document | Use When... |
|----------|-----------|
| **ANALYSIS_SUMMARY** | You need a 2-minute overview |
| **crawl_reference.md** | You need detailed technical info |
| **crawl_architecture_diagrams.md** | You need visual flow/architecture |
| **This card** | You need to remember key functions/patterns |

---

**Last Updated:** 2026-04-16  
**Source:** Crawl Roguelike (github.com/crawl/crawl)  
**Your Fork:** github.com/madeofants/crawl

*Print this card for quick reference while implementing!*
