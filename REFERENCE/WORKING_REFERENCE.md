# Crawl Reference - For Coding

Quick facts to use while modding Crawl.

## Character Creation Flow

```
Game Mode → Species → Job → Weapon → Name → Init → Play
```

**Files:**
- Menu logic: `newgame.cc`
- Species data: `species.cc` 
- Job data: `jobs.cc`
- Init after choices: `ng-setup.cc` (calls `_setup_generic()`)
- Restrictions: `ng-restr.h`

## Data Structures

**species_def** - What makes a species:
```
name, abbrev, xp_mod, hp_mod, mp_mod, wl_mod, mutations[], flags, habitat, undeadness, size
```

**job_def** - What makes a job:
```
name, abbrev, s/i/d (stats), recommended_species[], library[] (spells), 
equipment[] (starting items), wchoice (weapon type), skills[]
```

**newgame_def** - Character choices:
```
name, type (game mode), seed, species, job, weapon, fully_random
```

## Key File Locations

| What | Where |
|------|-------|
| Add/modify species | `species.cc` |
| Add/modify jobs | `jobs.cc` |
| Starting equipment | `jobs.cc` in `equipment[]` |
| Starting spells | `jobs.cc` in `library[]` |
| Restrictions | `ng-restr.h`, `ng-restr.cc` |
| Game modes | `game-type.h` |
| Menu system | `newgame.cc`, `menu.h`, `outer-menu.h` |
| Game init | `ng-setup.cc` - function `_setup_generic()` |

## Common Changes

### Add a new species
1. Edit `species.cc` - add to species array
2. Define: name, abbrev, modifiers, mutations, flags
3. Run `make` (generates species-groups.h)

### Modify job stats
1. Edit `jobs.cc` - find job in jobs array
2. Change: `s`, `i`, `d` fields
3. Run `make`

### Change starting equipment
1. Edit `jobs.cc` - find job's `equipment[]`
2. Change item names (vault format)
3. Run `make`

### Add starting spell
1. Edit `jobs.cc` - find job's `library[]`
2. Add spell_type to vector
3. Run `make`

### Add game mode
1. Add enum to `game-type.h`
2. Add init code to `ng-setup.cc` in `_setup_generic()`
3. Run `make`

### Restrict species/job combo
1. Edit `ng-restr.cc`
2. Add check to return `CC_BANNED` or `CC_RESTRICTED`
3. Run `make`

## Build

```bash
cd crawl-ref/source
make
```

Python generators run automatically. Compiles with generated headers.

## Enums

**game_type:**
```
GAME_TYPE_NORMAL, TUTORIAL, SPRINT, HINTS, ZOTDEF, INSTRUCTIONS, 
HIGH_SCORES, DESCENT, UNSPECIFIED
```

**Species (SP_*):**
```
HUMAN, ELF, DWARF, HALFLING, OREAD, MERFOLK, CENTAUR, NAGA, 
OGRE, KOBOLD, DRACONIAN, GNOME, SPRIGGAN, OCTOPODE, FELID, TENGU, 
DEMIGOD, DEMONSPAWN, GHOUL, VAMPIRE, MUMMY, GARGOYLE, FORMICID
```

**Jobs (JOB_*):**
```
FIGHTER, RANGER, PALADIN, BERSERKER, DEATH_KNIGHT, 
SKALD, DELVER, HUNTER, ROGUE, ASSASSIN, 
WIZARD, CONJURER, ENCHANTER, SUMMONER, NECROMANCER, TRANSMUTER, 
FIRE_ELEMENTALIST, COLD_ELEMENTALIST, EARTH_ELEMENTALIST, 
AIR_ELEMENTALIST, VENOM_MAGE, WANDERER, MONK, MYSTIC, PRIEST, HEALER
```

## Important Notes

- `species-groups.h` and `job-groups.h` are **generated** (don't edit directly)
- Generated from Python scripts: `util/species-gen.py`, `util/job-gen.py`
- Enum name: `CC_UNRESTRICTED` (not `CC_ALLOWED`)
- `ng-setup.cc` is 574 lines (not 21,015)
- Setup calls `_setup_generic()` at line 414

## Sprint Mode Special

- Bans certain jobs (e.g., `JOB_DELVER`)
- Some species restricted
- Check `_job_allowed()` wrapper in `newgame.cc:141-145`

## Menu System

- **UINewGameMenu** class manages char creation UI (newgame.cc:1167-1300)
- **OuterMenu** shows filtered hierarchical menus
- **MenuEntry** is individual menu item
- Menu filters items by viable/restricted/banned

## Platform Stuff

```cpp
#ifdef USE_TILE_LOCAL   // Desktop version
#ifdef USE_TILE_WEB     // Web version
// No ifdef = text console (fallback)
```

Different colors/rendering per platform.
