# Crawl Game Architecture - Visual Diagrams & Call Flows

## DIAGRAM 1: Character Creation Call Flow

```
┌─────────────────────────────────────────────────────────────┐
│ MAIN ENTRY: choose_game()                                   │
│ Inputs: ng, choice, defaults                                │
│ Output: Populates ng with final character definition        │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ├─── Clear Screen & Set Colors
                     │
                     ├─── Copy Metadata (name, type, map)
                     │
                     ├─┬─ Branch by Game Type
                     │ ├─ SPRINT/TUTORIAL → _choose_gamemode_map()
                     │ ├─ CUSTOM_SEED → _choose_seed()
                     │ └─ NORMAL → [skip]
                     │
                     ├─── _choose_char()
                     │    │
                     │    ├─ _construct_species_menu()
                     │    │  ├─ Filter species by current job
                     │    │  ├─ Assign menu letters
                     │    │  └─ Display menu & wait for selection
                     │    │
                     │    └─ _construct_backgrounds_menu()
                     │       ├─ Filter jobs by current species
                     │       ├─ Assign menu letters
                     │       └─ Display menu & wait for selection
                     │
                     ├─── _choose_name() (if name empty)
                     │    └─ Prompt for name + validate
                     │
                     ├─── Validation checks
                     │    ├─ Name non-empty
                     │    ├─ Species/job combo valid
                     │    └─ Game type valid
                     │
                     └─── write_newgame_options_file()
                          └─ Save preferences for next time

         RETURN: false (indicates new character, not restore)
```

---

## DIAGRAM 2: Menu System Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    UI Widget Framework                   │
├─────────────────────────────────────────────────────────┤
│                                                           │
│  ┌────────────────────────────────────────────────────┐  │
│  │ Widget (Base Class)                                │  │
│  │ - _render()                                        │  │
│  │ - _get_preferred_size()                            │  │
│  │ - _allocate_region()                               │  │
│  │ - on_event()                                       │  │
│  └────────────┬───────────────────────────────────────┘  │
│               │                                           │
│               ├─────────────────┬──────────────────┐      │
│               │                 │                  │      │
│        ┌──────▼─────┐    ┌──────▼────┐    ┌──────▼────┐  │
│        │ MenuButton │    │    Box     │    │ Checkbox  │  │
│        ├────────────┤    ├────────────┤    ├───────────┤  │
│        │ id         │    │ HORZ/VERT  │    │ checked   │  │
│        │ hotkey     │    │ alignment  │    │ child     │  │
│        │ description│    │ children[] │    │           │  │
│        │ states:    │    └────────────┘    └───────────┘  │
│        │ - focused  │                                      │
│        │ - hovered  │                                      │
│        │ - active   │                                      │
│        └────────────┘                                      │
│                                                           │
│  ┌────────────────────────────────────────────────────┐  │
│  │ OuterMenu                                          │  │
│  │ ├─ Grid layout                                     │  │
│  │ ├─ Button management (add, get, iterate)          │  │
│  │ ├─ Focus management                               │  │
│  │ ├─ Linked menus (4 directional)                   │  │
│  │ └─ Description display                            │  │
│  └────────────────────────────────────────────────────┘  │
│                                                           │
│  ┌────────────────────────────────────────────────────┐  │
│  │ Menu (Full menu implementation)                    │  │
│  │ ├─ MenuEntry items                                │  │
│  │ ├─ Multi-column support                           │  │
│  │ ├─ Entry level: TITLE/SUBTITLE/ITEM               │  │
│  │ ├─ Filtering                                      │  │
│  │ └─ Custom rendering                               │  │
│  └────────────────────────────────────────────────────┘  │
│                                                           │
└─────────────────────────────────────────────────────────┘

Rendering Backends:
  ┌─────────────────┐  ┌──────────────┐  ┌──────────────┐
  │  Tile (SDL)     │  │ Tile (Web)   │  │ ASCII/Console│
  │ ShapeBuffer     │  │ JSON/DOM     │  │ Text Colors  │
  │ LineBuffer      │  │ WebSocket    │  │              │
  └─────────────────┘  └──────────────┘  └──────────────┘
```

---

## DIAGRAM 3: Character Selection Process

```
                    ┌──────────────────────┐
                    │  SELECT SPECIES      │
                    │                      │
                    │  Groups:             │
                    │  - Humanoid          │
                    │  - Draconian         │
                    │  - Reptile           │
                    │  - Demonic           │
                    │  - etc.              │
                    │                      │
                    │ User picks letter    │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │  SELECT JOB/ROLE     │
                    │                      │
                    │  Groups:             │
                    │  - Warrior (5 jobs)  │
                    │  - Zealot (3 jobs)   │
                    │  - Adventurer (4)    │
                    │  - Warrior-mage (4)  │
                    │  - Mage (10 jobs)    │
                    │                      │
                    │ User picks letter    │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │ VALIDATE COMBO       │
                    │                      │
                    │ Is combo allowed?    │
                    │ (check restrictions) │
                    └──────────┬───────────┘
                               │
                    ┌──────────┴──────────┐
                    │                     │
                    ▼                     ▼
            ✓ Allowed          ✗ Restricted/Banned
               │                       │
               ▼                       ▼
        Continue to          Show warning &
        name selection      ask to select again
               │
               ▼
        ┌──────────────────────┐
        │  RANDOM ALTERNATIVES │
        │                      │
        │ (M) ← Viable Random  │
        │ (R) ← Fully Random   │
        │ (V) ← View Aptitudes │
        └──────────────────────┘
```

---

## DIAGRAM 4: Game Definition Data Model

```
┌────────────────────────────────────────────────────────────────┐
│ struct newgame_def                                             │
├────────────────────────────────────────────────────────────────┤
│                                                                │
│ IDENTITY:                                                      │
│ ├─ name: string         → Player name                         │
│ └─ filename: string     → Save file path                      │
│                                                                │
│ GAME CONFIGURATION:                                            │
│ ├─ type: game_type      → NORMAL | SPRINT | TUTORIAL |       │
│ │                         CUSTOM_SEED | ARENA               │
│ ├─ seed: uint64_t       → RNG seed for reproducibility       │
│ └─ pregenerate: bool    → Pre-generate dungeon?              │
│                                                                │
│ MODE-SPECIFIC:                                                 │
│ ├─ map: string          → Sprint map name or "random"        │
│ └─ arena_teams: string  → Arena team configuration           │
│                                                                │
│ UI/CHOICE CONSTRAINTS:                                         │
│ ├─ allowed_combos: vector<string>                            │
│ ├─ allowed_species: vector<species_type>                     │
│ ├─ allowed_jobs: vector<job_type>                            │
│ └─ allowed_weapons: vector<weapon_type>                      │
│                                                                │
│ CHARACTER DEFINITION:                                          │
│ ├─ species: species_type                                      │
│ │  Values: SP_UNKNOWN | SP_HUMAN | SP_ELF | SP_DWARF |      │
│ │         ... (specific) | SP_RANDOM | SP_VIABLE           │
│ │                                                            │
│ ├─ job: job_type                                             │
│ │  Values: JOB_UNKNOWN | JOB_FIGHTER | JOB_MAGE | ...       │
│ │         ... (specific) | JOB_RANDOM | JOB_VIABLE         │
│ │                                                            │
│ └─ weapon: weapon_type                                        │
│    Values: WPN_UNKNOWN | WPN_DAGGER | WPN_HAND_AXE | ...   │
│                                                                │
│ RANDOMIZATION FLAG:                                            │
│ └─ fully_random: bool   → Entire char random in one step?   │
│                                                                │
├────────────────────────────────────────────────────────────────┤
│ METHODS:                                                       │
│ ├─ newgame_def()        → Constructor with defaults           │
│ ├─ clear_character()    → Reset to SP_UNKNOWN/JOB_UNKNOWN    │
│ ├─ write_prefs()        → Serialize to file                   │
│ ├─ operator==()         → Compare relevant fields             │
│ └─ operator!=()                                               │
└────────────────────────────────────────────────────────────────┘
```

---

## DIAGRAM 5: Species & Job Grouping

```
┌─────────────────────────────────────────────────────────────┐
│ SPECIES GROUPS                                              │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│ struct species_group {                                      │
│   const char* name;              → Display name            │
│   coord_def position;            → Screen x,y position     │
│   int width;                     → Column width for layout │
│   vector<species_type> species_list;                       │
│   void attach(newgame_def&, UINewGameMenu*, menu_letter&); │
│ }                                                           │
│                                                             │
│ ┌─ Humanoid       ┌─ Reptile        ┌─ Draconian          │
│ │ • Human         │ • Kobold         │ • Draconian         │
│ │ • Half-Elf      │ • Troglodyte     │ • Ghoul             │
│ │ • Dwarf         │ • Lizardfolk     │ • Spriggan          │
│ │ • Tengu         │ • Merfolk        └─                     │
│ └─                └─                                        │
│                                                             │
│ ┌─ Demonic        ┌─ Undead          ┌─ Magical            │
│ │ • Felid         │ • Mummy           │ • Vine Stalker      │
│ │ • Orc           │ • Skeleton        │ • Formicid          │
│ │ • Minotaur      │ • Vampire         │ • Barachi           │
│ │ • Troll         │ • Ghost           │ • Gargoyle          │
│ └─                └─                  └─                    │
│                                                             │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│ JOB GROUPS                                                  │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│ struct job_group {                                          │
│   const char* name;              → Display name            │
│   coord_def position;            → Screen x,y position     │
│   int width;                     → Column width            │
│   vector<job_type> jobs;                                   │
│   void attach(newgame_def&, UINewGameMenu*, menu_letter&); │
│ }                                                           │
│                                                             │
│ ┌─ Warrior        ┌─ Zealot         ┌─ Adventurer         │
│ │ • Fighter       │ • Berserker      │ • Artificer         │
│ │ • Gladiator     │ • Cinder Acolyte │ • Shapeshifter      │
│ │ • Monk          │ • Chaos Knight   │ • Wanderer          │
│ │ • Hunter        └─                 │ • Delver            │
│ │ • Brigand                          └─                    │
│ └─                                                          │
│                                                             │
│ ┌─ Warrior-Mage   ┌─ Mage                                  │
│ │ • Warper        │ • Hedge Wizard                         │
│ │ • Hexslinger    │ • Conjurer                             │
│ │ • Enchanter     │ • Summoner                             │
│ │ • Reaver        │ • Necromancer                          │
│ └─                │ • Forgewright                          │
│                   │ • Fire Elementalist                    │
│                   │ • Ice Elementalist                     │
│                   │ • Air Elementalist                     │
│                   │ • Earth Elementalist                   │
│                   │ • Alchemist                            │
│                   └─                                        │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---

## DIAGRAM 6: Restriction Validation Flow

```
┌─────────────────────────────────────┐
│ User selects: Species A, Job B      │
└────────────────┬────────────────────┘
                 │
                 ▼
        ┌─────────────────────┐
        │ Check Restriction:  │
        │ job_allowed(       │
        │   species_A,       │
        │   job_B            │
        │ )                  │
        └────────┬────────────┘
                 │
        ┌────────┴──────────┐
        │                   │
        ▼                   ▼
    CC_BANNED         CC_ALLOWED
        │                   │
        ├─ Combo not       ├─ OK to proceed
        │  valid          │
        │                  │
        ├─ Sprint          ├─ Store in ng
        │  specific       │
        │  rules          ├─ Continue with
        │                  │  name selection
        └─ Show error      └─
          & retry

Special Cases:
- JOB_DELVER banned in Sprint mode
- Some species/job combos fundamentally broken
- Viability checked separately
```

---

## DIAGRAM 7: Seed & Pregeneration Options

```
┌────────────────────────────────────────────────────┐
│ CUSTOM SEED GAME TYPE                              │
├────────────────────────────────────────────────────┤
│                                                    │
│  Text Input: ├─────────────────────────┤ [?] Help │
│  Seed        │  1234567890123456     │           │
│              └─────────────────────────┘           │
│                                                    │
│  ┌──────────────┐  ┌──────────────┐              │
│  │ [-] Clear    │  │ [d] Daily    │              │
│  └──────────────┘  └──────────────┘              │
│                                                    │
│  Hotkeys:                                          │
│  - [p] / [Ctrl+V]  → Paste from clipboard        │
│  - [-]             → Clear input                  │
│  - [d]             → Today's date (YYYYMMDD)     │
│  - [?]             → Show help                    │
│  - [Enter]         → Confirm & continue          │
│  - [ESC]           → Abort character creation    │
│                                                    │
│  ☑ Fully pregenerate the dungeon                 │
│                                                    │
│  Seed affects:                                     │
│  - Dungeon layout                                  │
│  - Monster placement                              │
│  - Item generation                                │
│  - NPC behavior                                   │
│                                                    │
│  Note: Upgrading mid-game may affect seeding!    │
│                                                    │
└────────────────────────────────────────────────────┘
```

---

## DIAGRAM 8: Game Setup Pipeline

```
AFTER CHARACTER CHOICE:

┌────────────────────────────┐
│ setup_game(newgame_def ng) │
└────────────────┬───────────┘
                 │
        ┌────────┴────────┐
        │                 │
        ▼                 ▼
    _init_player()   give_items_skills()
        │                 │
        │            ┌────┴────┐
        │            │         │
        │            ▼         ▼
        │        Classes  Starting
        │        & Stats  Weapons
        │            │
        │            ├─ Equipment
        │            │  (auto-equip)
        │            │
        │            ├─ Spellbooks
        │            │  (auto-memorize)
        │            │
        │            ├─ Consumables
        │            │  (potions, scrolls)
        │            │
        │            └─ Skills
        │               (training points)
        │
        ▼
    _unfocus_stats()
        │
        ├─ Check min stat >= 3
        │
        └─ Rebalance if needed
        
        │
        ▼
    initial_dungeon_setup()
        │
        ├─ initialise_branch_depths()
        ├─ initialise_temples()
        ├─ setup_random_spawn_points()
        └─ place_uniques()

┌────────────────────────────┐
│ GAME READY TO BEGIN         │
└────────────────────────────┘
```

---

## DIAGRAM 9: Menu Letter Assignment Pattern

```
Menu Letter Sequencing:

┌─────────────────────────────────────────┐
│ menu_letter (standard)                  │
├─────────────────────────────────────────┤
│ a → b → c → ... → z → A → B → ...      │
│                           → Z → a ...   │
│                                         │
│ Cycles: lowercase → UPPERCASE → repeat │
└─────────────────────────────────────────┘

┌─────────────────────────────────────────┐
│ menu_letter2 (alternative)              │
├─────────────────────────────────────────┤
│ a → b → c → ... → z → 0 → 1 → ...     │
│                           → 9 → a ...   │
│                                         │
│ Cycles: lowercase → digits → repeat    │
└─────────────────────────────────────────┘

Application in UI:

Species Groups:                Job Groups:
a) Humanoid         →         a) Warrior
b) Reptile          →         b) Zealot
c) Draconian        →         c) Adventurer
d) Demonic          →         d) Warrior-Mage
e) Undead           →         e) Mage
f) Magical          →         

Within each group, items get sub-letters:
a) Humanoid
   1. Human                    OR using menu_letter2:
   2. Half-Elf                 a. Human
   3. Dwarf                    b. Half-Elf
   ...                         c. Dwarf
```

---

## DIAGRAM 10: Platform-Specific Rendering

```
┌──────────────────────────────────────────────────────┐
│ WIDGET RENDERING (Abstract)                          │
└─────────────────┬──────────────────────────────────┘
                  │
        ┌─────────┴─────────┬──────────────┐
        │                   │              │
        ▼                   ▼              ▼
    SDL/Tiles          Tile Web        ASCII/Console
    (USE_TILE_LOCAL)   (USE_TILE_WEB)  (TEXT MODE)
        │                   │              │
        ├─ ShapeBuffer      ├─ JSON        ├─ Text output
        │                   │  serialization│
        ├─ LineBuffer       ├─ DOM update  ├─ Color codes
        │                   │  via WebSocket│
        ├─ SDL rendering    ├─ JavaScript  ├─ ANSI colors
        │                   │  UI          │
        └─ Pixel perfect    └─ Browser     └─ Terminal
          rendering            rendering     rendering

Color Palette:

  SDL/Tiles:                Console/ASCII:
  - LIGHTGRAY (normal)      - BLUE (normal)
  - LIGHTGRAY (bad)         - LIGHTGRAY (bad)
  - LIGHTGRAY (control)     - BLUE (control)
  - LIGHTGREEN (good)       - GREEN (good)
```

---

**Visual Documentation Generated:** 2026-04-16  
**Purpose:** Reference for implementing game architecture
