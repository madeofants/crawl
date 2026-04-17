# Phase 0: DCSS Architecture Research - COMPLETE SUMMARY

**Status:** ✅ COMPLETE - All 5 research tasks finished

**Dates:** 2026-04-16 (one session)

---

## What We Learned

This phase analyzed DCSS internals to understand how to integrate an AI Narrator. We examined:
- Game startup and main loop
- Player state structures
- Command execution system
- Character creation flow
- Lua scripting integration

**Result:** Complete understanding of DCSS architecture and clear integration points.

---

## Phase 0 Tasks Completed

### 0.1: Game Startup & Main Loop ✅
**File:** PHASE_0_1_STARTUP_RESEARCH.md

**Key findings:**
- Entry: `main()` in main.cc:240
- Game loop: `while(true) _input()` in main.cc:498
- Command execution: `process_command(cmd, prev_cmd)` in main.cc:2094
- Turn control: `you.turn_is_over` boolean
- World simulation: `world_reacts()` function

**For Narrator:**
- Can inject commands via `process_command()`
- Turn-by-turn flow is perfect for prose interaction
- Infinite loop exits via exception to `_launch_game_loop()`

### 0.2: Game State Structures ✅
**File:** PHASE_0_2_STATE_RESEARCH.md

**Key findings:**
- Player data: `you` global object (player class, 1000+ lines)
- Game state: `crawl_state` global
- Environment: `env` contains level data
- All accessible from code

**Critical fields for Narrator:**
- `you.hp`, `you.hp_max` - Health
- `you.xpos()`, `you.ypos()` - Position
- `you.where_are_you`, `you.depth` - Location
- `you.experience_level`, `you.gold` - Resources
- `you.turn_is_over` - Turn consumption flag
- `you.duration[DUR_*]` - Active effects

### 0.3: Command Execution ✅
**File:** PHASE_0_3_COMMANDS_RESEARCH.md

**Key findings:**
- 200+ commands defined in command-type.h
- Movement: CMD_MOVE_UP, CMD_MOVE_DOWN, etc
- Actions: CMD_CAST_SPELL, CMD_QUAFF, CMD_REST, etc
- Queries: CMD_DISPLAY_INVENTORY (don't consume turn)
- All routed through process_command() giant switch
- Commands set `you.turn_is_over = true` if they consume a turn

**For Narrator:**
- Easy to convert prose actions to commands
- Clear separation of turn-consuming vs non-consuming
- Can query game state without consuming a turn

### 0.4: Character Creation Integration ✅
**File:** PHASE_0_4_CHARACTER_CREATION_RESEARCH.md

**Key findings:**
- Entry: `startup_step()` in startup.cc:1013
- Setup: `setup_game(newgame_def)` in ng-setup.cc:373
- Full initialization: `_setup_generic()` in ng-setup.cc:508
- Can bypass menu and call setup_game() directly
- Character choices stored in newgame_def struct

**For Narrator:**
- Can programmatically create characters
- Populate newgame_def with name/species/job
- Call setup_game() to initialize
- Game ready for first turn immediately

### 0.5: Lua Scripting ✅
**File:** PHASE_0_5_LUA_RESEARCH.md

**Key findings:**
- CLua for user scripts (macros, RC files)
- DLua for dungeon generation
- ready() hook called each turn
- Can read `you.*` and `crawl.*` state
- Can inject commands via `crawl.sendkeys()`
- Can register custom C++ functions

**For Narrator:**
- Lua exists but not required for core functionality
- Can add Lua bindings later for user scripting
- Primary approach: Direct C++ integration

---

## The Complete Integration Picture

### Data Flow for Narrator Interaction

```
[Narrator AI External Process]
         ↓ prose action
         ↓
[Parse action to command]
         ↓
[Read game state from DCSS]
    ↙       ↙       ↙
[you.*] [env.*] [crawl_state]
    ↓
[Create prose narrative]
    ↓
[Return to Narrator for next decision]
         ↑
    Infinite loop
```

### Core API for Narrator Integration

**Reading state (no turn consumed):**
```cpp
// Get player info
you.your_name, you.species, you.char_class
you.hp, you.hp_max, you.magic_points, you.max_magic_points
you.xpos(), you.ypos()
you.where_are_you, you.depth
you.experience_level, you.gold
you.turn_is_over
you.duration[DUR_*]  // Active effects
```

**Executing actions (consumes turn if successful):**
```cpp
process_command(CMD_MOVE_UP, CMD_NO_CMD);
process_command(CMD_CAST_SPELL, CMD_NO_CMD);
process_command(CMD_REST, CMD_NO_CMD);
process_command(CMD_WAIT, CMD_NO_CMD);
// ... 200+ commands available
```

**Creating characters:**
```cpp
newgame_def ng;
ng.name = "Character Name";
ng.species = SP_HUMAN;
ng.job = JOB_FIGHTER;
ng.type = GAME_TYPE_NORMAL;
setup_game(ng);  // Player now exists and ready
```

---

## Key Integration Points (for Phase 1 Implementation)

### 1. Character Creation Hook
**Where:** `startup.cc:startup_step()` or `ng-setup.cc:setup_game()`

**What to do:** Create wrapper function
```cpp
bool narrator_create_character(string name, species_type sp, job_type job)
{
    newgame_def ng;
    ng.name = name;
    ng.species = sp;
    ng.job = job;
    ng.type = GAME_TYPE_NORMAL;
    setup_game(ng);
    return true;
}
```

### 2. State Query Interface
**Where:** All over (you.*, env.*, crawl_state)

**What to do:** Create getter functions
```cpp
string get_game_state_json()
{
    // Return all relevant state as JSON
    // Position, health, location, visible monsters, items, effects, etc.
    json state;
    state["hp"] = you.hp;
    state["hp_max"] = you.hp_max;
    // ... hundreds more ...
    return state.dump();
}
```

### 3. Command Execution Interface
**Where:** `main.cc:process_command()`

**What to do:** Create wrapper function
```cpp
bool narrator_execute_command(string prose_action)
{
    // Parse prose to command_type
    command_type cmd = prose_to_command(prose_action);
    
    // Execute
    process_command(cmd, CMD_NO_CMD);
    
    // Return whether turn was consumed
    return you.turn_is_over;
}
```

### 4. Main Loop Integration
**Where:** `main.cc:_input()` or `_launch_game()`

**What to do:** Add AI decision hook
```cpp
// In _input() function, replace _get_next_cmd() with:
if (narrator_mode_enabled)
{
    // Get state
    string state = get_game_state_json();
    
    // Send to Narrator AI (external process/API)
    string action = call_narrator_api(state);
    
    // Execute
    cmd = prose_to_command(action);
}
else
{
    cmd = _get_next_cmd();  // Normal input
}
```

---

## Architecture Overview

```
DCSS Executable (crawl.exe)
├── Main Loop: while(true) _input()
│   ├─ Get game state (you.*, env.*)
│   ├─ [INTEGRATION POINT] Get next command from Narrator AI
│   ├─ Execute: process_command(cmd)
│   ├─ Set turn_is_over if command consumes turn
│   └─ If turn_is_over: world_reacts() for AI/effects
│
├── Character Creation: setup_game(newgame_def)
│   └─ [INTEGRATION POINT] Narrator can call this directly
│
└── Lua Interface (optional future):
    └─ ready() hook called each turn
    └─ Can query you.*, crawl.* state
    └─ Can inject commands via sendkeys()
```

---

## Files Created During Phase 0

| File | Purpose |
|------|---------|
| PHASE_0_1_STARTUP_RESEARCH.md | Game startup and main loop analysis |
| PHASE_0_2_STATE_RESEARCH.md | Player/game state structures |
| PHASE_0_3_COMMANDS_RESEARCH.md | Command execution system |
| PHASE_0_4_CHARACTER_CREATION_RESEARCH.md | Character creation flow |
| PHASE_0_5_LUA_RESEARCH.md | Lua scripting integration |
| PHASE_0_COMPLETE_SUMMARY.md | This file - overall summary |

---

## What We Know Is Possible

✅ **Character Creation:**
- Create characters from code with specific species/job/name
- Game is fully initialized after setup_game()

✅ **State Reading:**
- Access all player data (hp, position, spells, skills, etc)
- Access level/dungeon data
- Query visible monsters and items
- All without consuming turns

✅ **Command Execution:**
- Execute 200+ game commands from code
- Commands behave exactly as if player typed them
- Can check if command consumed a turn

✅ **Turn-by-Turn Loop:**
- Game waits for input each turn
- We can inject commands via process_command()
- World simulation (AI, effects) happens automatically
- Perfect for external narrative AI

✅ **Lua Scripting (optional):**
- Existing ready() hook for each turn
- Can register custom C++ functions
- Can inject commands via Lua

---

## What We Don't Know (Phase 1 Research)

⚠️ **Performance:**
- How fast can we read state and execute commands?
- Will external AI API calls cause lag?
- Do we need local AI or can we use remote API?

⚠️ **Specific Game State Access:**
- How to efficiently get list of visible monsters?
- How to access monster AI state and intentions?
- How to query specific item properties?
- (Probably just need to read the right C++ structs)

⚠️ **Save/Load Interaction:**
- How do saves work with AI control?
- Can player save mid-game with AI running?
- Do we need special save handling?

⚠️ **Unimplemented Features:**
- Text output formatting for prose
- Monster/NPC reaction systems
- Custom content/species/items
- (Not research, just implementation)

---

## Recommended Phase 1 Structure

**Phase 1.0:** Integration Layer
- Create narrator_control.h/.cc
- Implement character creation wrapper
- Implement state query functions
- Implement command execution wrapper

**Phase 1.1:** Narrative Interface
- Create simple prose parser
- Map common actions to commands
- Create output formatter

**Phase 1.2:** Main Loop Integration
- Modify _input() to check for narrator mode
- Route to narrator_execute_command()
- Handle turn consumption

**Phase 1.3:** Testing
- Create simple test characters
- Execute test sequences of commands
- Verify state reading

---

## Key Takeaways

1. **DCSS is designed for modification** - Clean architecture, clear entry points
2. **We have all the hooks we need** - Character creation, state access, command execution
3. **Turn-by-turn flow is perfect** - Matches prose narrative interaction model
4. **Minimal changes needed** - Mostly just wrappers and integration layer
5. **Lua is a bonus** - Not required for core functionality but available for users

---

## Decision: Go-No-Go for Phase 1

**GO** ✅

All critical research complete. DCSS architecture supports AI-driven gameplay. All necessary integration points identified. No blockers found. Ready to proceed with Phase 1 implementation.

---

**Research Completed:** 2026-04-16 (Phase 0.0-0.5)
**Next:** Phase 1 - DCSS Integration Layer Implementation
**Time Estimate:** 4-8 hours for basic integration, 2-4 weeks for complete system

