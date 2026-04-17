# Phase 1: Integration Layer - Quick Start Guide

**Status:** Ready to begin

**Goal:** Create the integration layer that lets external code control DCSS via Narrator AI

---

## Quick Review: What We Learned

From Phase 0 research:

1. **Character Creation** → `setup_game(newgame_def)`
2. **State Access** → `you.*`, `env.*`, `crawl_state`
3. **Command Execution** → `process_command(cmd, prev_cmd)`
4. **Turn Control** → `you.turn_is_over` boolean
5. **Main Loop** → `while(true) _input()` in main.cc:498
6. **Event Flow** → Input → Execute → Simulate → Repeat

---

## Phase 1 High-Level Plan

### Task 1.0: Create Narrator Integration Layer
- **File:** narrator_control.h and narrator_control.cc
- **Functions:**
  - `narrator_create_character(name, species, job)` - Create a new character
  - `narrator_get_game_state()` - Return all relevant state
  - `narrator_execute_command(command_enum)` - Execute a command
  - `narrator_is_turn_over()` - Check if turn was consumed

### Task 1.1: Create State Query Interface
- **File:** narrator_state.h and narrator_state.cc
- **Functions:**
  - `get_player_info()` - Name, species, job, level, gold
  - `get_health_info()` - HP, MP, status effects
  - `get_position_info()` - X, Y, branch, depth
  - `get_visible_monsters()` - Monsters within view
  - `get_floor_items()` - Items at player location

### Task 1.2: Create Command Mapping
- **File:** narrator_commands.h and narrator_commands.cc
- **Functions:**
  - `string_to_command(string)` - Parse "move up" → CMD_MOVE_UP
  - `command_to_string(command_type)` - Get readable name
  - `is_valid_command(command_type)` - Check if usable now
  - Command categories (movement, action, query, etc)

### Task 1.3: Modify Main Loop Input
- **File:** main.cc (modify _input() function)
- **Change:** Replace `_get_next_cmd()` with narrator check
- **Code:**
  ```cpp
  if (narrator_mode_active())
      cmd = narrator_get_next_command();
  else
      cmd = _get_next_cmd();  // Normal input
  ```

### Task 1.4: Testing
- **File:** narrator_test.cc
- **Tests:**
  - Create character and verify state
  - Execute movement commands
  - Execute action commands
  - Verify state changes
  - Verify turn consumption

---

## File Structure to Create

```
crawl-ref/source/
├── narrator_control.h      ← Main integration interface
├── narrator_control.cc
├── narrator_state.h        ← Game state queries
├── narrator_state.cc
├── narrator_commands.h     ← Command mapping
├── narrator_commands.cc
├── narrator_test.cc        ← Testing
├── main.cc (modified)      ← Add input routing
└── Makefile (modified)     ← Add new targets
```

---

## Key Code Locations to Modify

1. **main.cc:1080** - _input() function
   - Add check: `if (narrator_mode_active())`
   - Get command from narrator instead of `_get_next_cmd()`

2. **Makefile** - Build rules
   - Add narrator_*.cc files to compilation

3. **command.h** - Command declarations
   - Already has `process_command()` exported

---

## API Design (Summary)

### Character Creation
```cpp
bool narrator_create_character(
    const string &name,
    species_type species,
    job_type job);
// Returns: true if successful, false if invalid combo
```

### State Query
```cpp
struct narrator_game_state {
    // Player info
    string player_name;
    string species_name;
    string job_name;
    int level;
    int gold;
    int hp, hp_max;
    int mp, mp_max;
    
    // Position
    int x, y;
    string branch;
    int depth;
    
    // Visible entities
    vector<monster_info> visible_monsters;
    vector<item_info> floor_items;
};

narrator_game_state narrator_get_game_state();
```

### Command Execution
```cpp
bool narrator_execute_command(command_type cmd);
// Returns: true if command was executed, false if invalid
// Sets you.turn_is_over if turn was consumed

bool narrator_is_turn_over();
// Returns: true if last command consumed a turn
```

---

## Implementation Order

**Phase 1.0 (Priority 1) - Core Integration**
1. Create narrator_control.h/.cc with basic API
2. Create narrator_state.h/.cc with state queries
3. Add narrator mode flag to Options
4. Modify main.cc:_input() to check narrator mode

**Phase 1.1 (Priority 2) - Command Mapping**
1. Create narrator_commands.h/.cc
2. Implement string_to_command parser
3. Test all common commands

**Phase 1.2 (Priority 3) - Testing**
1. Create test suite
2. Test character creation
3. Test command sequences
4. Verify state accuracy

**Phase 1.3 (Priority 4) - Polish**
1. Error handling
2. Edge cases
3. Documentation
4. Performance tuning

---

## Success Criteria for Phase 1

✅ Can create character from code  
✅ Can read all player state without turn consumption  
✅ Can execute movement commands  
✅ Can execute action commands (cast, quaff, etc)  
✅ Can detect when turn is consumed  
✅ Can detect when world reacts  
✅ All state remains accurate after commands  

---

## Testing Strategy

### Unit Tests
- Test character creation with various species/jobs
- Test state query accuracy
- Test command parsing
- Test valid/invalid commands

### Integration Tests
- Create character → Execute command → Check state
- Movement → Check position
- Cast spell → Check turn consumed
- Wait → Check world reacts

### Edge Cases
- Invalid command combinations
- Commands in restricted states
- Rapid command sequences
- Save/load interactions

---

## Reference Documents for Phase 1

**Must Read Before Starting:**
1. PHASE_0_COMPLETE_SUMMARY.md - Overall architecture
2. PHASE_0_1_STARTUP_RESEARCH.md - Main loop details
3. PHASE_0_3_COMMANDS_RESEARCH.md - Command system
4. PHASE_0_4_CHARACTER_CREATION_RESEARCH.md - Character creation

**For Implementation:**
1. PHASE_0_2_STATE_RESEARCH.md - State structure details
2. command.h - Command declarations
3. player.h - Player struct fields
4. ng-setup.cc - Character setup implementation

---

## Expected Duration

- **Phase 1.0:** 2-4 hours
- **Phase 1.1:** 1-2 hours
- **Phase 1.2:** 1-2 hours
- **Phase 1.3:** 1-2 hours
- **Total:** 5-10 hours for complete Phase 1

---

## Next Steps

1. ✅ You've completed Phase 0 research
2. ⏭️ Create narrator_control.h/.cc skeleton
3. ⏭️ Implement character creation wrapper
4. ⏭️ Implement state query functions
5. ⏭️ Modify main loop input handling
6. ⏭️ Create tests

**Status:** Ready to proceed. Begin with Task 1.0.

---

**Created:** 2026-04-16  
**For:** Phase 1 Implementation Team  
**Based On:** Complete Phase 0 Research
