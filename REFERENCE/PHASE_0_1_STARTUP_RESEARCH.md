# Phase 0.1: DCSS Game Startup & Main Loop - Research Findings

**Status:** ✅ COMPLETE - Research findings documented

---

## Overall Architecture

```
main() [main.cc:240]
  ↓
  Parse args, init systems
  ↓
_launch_game_loop() [main.cc:390]  ← RESTART POINT (game restarts from here on save/load)
  ├─ try/catch for game_ended_condition
  ├─ calls _launch_game() [main.cc:424]
  └─ handles game restart logic
      ↓
      startup_step() [startup.cc:1013]  ← Character creation/loading happens here
      ↓
      _launch_game() continues [main.cc:424-500]
      ├─ Welcome message
      ├─ Initialize screen
      ├─ Start hints if needed
      ├─ Save initial state
      └─ while(true) _input()  ← MAIN GAME LOOP
          ↓
          world_reacts()  ← Engine simulation/AI
          ↓
          _input() again
```

---

## Entry Point: main() [main.cc:240-350]

**What it does:**
1. Initialize locale and crash handler
2. Parse command-line arguments  
3. Initialize game systems (keybindings, colors, monsters, items)
4. Load init file (RC file)
5. Parse command-line args again (for everything else)
6. Handle score listings if requested
7. Initialize tiles/graphics if enabled
8. **Call `_launch_game_loop()`** ← This is where actual game happens

**Key code flow:**
```cpp
int main(int argc, char *argv[])
{
    // ... setup code ...
    _launch_game_loop();  // Line 344
    // ... cleanup ...
    return 0;
}
```

**Important:** main() initializes global systems that are reused across game restarts.

---

## Game Loop Manager: _launch_game_loop() [main.cc:390-422]

**What it does:**
- Wraps the actual game in try/catch for game_ended_condition
- Handles save/load/restart logic
- Calls `_launch_game()` in a loop to support restarts without full program exit

**Key code flow:**
```cpp
static void _launch_game_loop()
{
    bool game_ended = false;
    do
    {
        try
        {
            game_ended = false;
            _launch_game();  // Actual game runs here
        }
        catch (const game_ended_condition &ge)
        {
            game_ended = true;
            crawl_state.last_game_exit = ge;  // Save exit info
            _reset_game();  // Reset global state for restart
            
            // Decide whether to restart (Sprint menu, etc)
            if (ge.exit_reason == game_exit::save)
                crawl_state.last_type = GAME_TYPE_UNSPECIFIED;
        }
        catch (const ext_fail_exception &fe)
        {
            end(1, false, "%s", fe.what());  // Fatal error
        }
        catch (const short_read_exception&)
        {
            end(1, false, "Error: truncation inside the save file.\n");
        }
    } while (crawl_should_restart(crawl_state.last_game_exit.exit_reason)
             && game_ended
             && !crawl_state.seen_hups);  // Exit loop on HUP
}
```

**Implication for integration:**
- We can throw `game_ended_condition` to safely exit/restart a game
- `_reset_game()` resets global state - important to understand what that clears
- `crawl_state` holds the exit reason and message

---

## Game Startup: _launch_game() [main.cc:424-500]

**What it does:**
1. Call `startup_step()` to handle character creation/loading
2. Set up screen and welcome messages  
3. Initialize hints if in hints mode
4. Save the game state
5. **Enter the main game loop: `while(true) _input()`**

**Key code flow:**
```cpp
NORETURN static void _launch_game()
{
    const bool game_start = startup_step();  // TRUE if new game, FALSE if loaded
    
    // ... setup (keybindings, hints mode, messages) ...
    
    // Welcome message shows player's name, species, job
    msg::stream << "<yellow>Welcome" 
        << (game_start? "" : " back") << ", "
        << you.your_name << " the "
        << species::name(you.species)
        << " " << get_job_name(you.char_class) << ".</yellow>";
    
    // ... more setup messages ...
    
    _prep_input();  // Initialize input system
    
    if (game_start)
    {
        if (crawl_state.game_is_hints())
            _startup_hints_mode();
        _take_starting_note();
    }
    else
        hints_load_game();  // Load hints state if game was loaded
    
    level_change();  // Catch up on any pending level-ups
    save_game_state();  // Save initial state for crash recovery
    
    cursor_control ccon(!Options.use_fake_player_cursor);
    while (true)
        _input();  // ← MAIN GAME LOOP - NEVER RETURNS (throws exception to exit)
}
```

**Key variables:**
- `you.*` - Player data structure (see Phase 0.2)
- `game_start` - Boolean: new game (true) or loaded game (false)
- `crawl_state` - Global game state

**Character creation happens in:** `startup_step()` [startup.cc:1013]
- We've already researched this in reference docs
- Returns true if new game, false if loaded

---

## Main Game Loop: _input() [main.cc:1080-1350+]

**What it does each turn:**
1. Handle pending revivals, level-ups, experience
2. Check if player is incapacitated (stun, etc) - handle stun time
3. Handle autopickup and autoinscribe
4. Handle any pending delays (spell channeling, spells in progress)
5. Get next command from input (`_get_next_cmd()`)
6. Execute command via `::process_command(cmd, prev_cmd)`
7. Call `world_reacts()` - engine simulation and NPC/monster actions
8. Loop back to step 1

**Key code flow (simplified):**
```cpp
static void _input()
{
    // Handle pending stuff
    if (you.pending_revival)
        revive();
    apply_exp();
    
    // Handle incapacitation (stun, etc)
    if (you.cannot_act())
    {
        world_reacts();
        return;  // Skip command processing this turn
    }
    
    // Get next command
    _prep_input();
    command_type cmd = _get_next_cmd();  // Blocks until input received
    
    // Execute command
    if (!you.turn_is_over && cmd != CMD_NEXT_CMD)
        ::process_command(cmd, real_prev_cmd);  // Line 1295
    
    // Let world react (monsters move, etc)
    if (you.turn_is_over)
    {
        world_reacts();
    }
}
```

**Critical insight:**
- Game loops infinitely until `game_ended_condition` exception is thrown
- Each call to `_input()` represents one player turn (or partial turn for delays)
- `you.turn_is_over` controls whether world reacts
- Commands are executed via `::process_command(cmd, prev_cmd)` - external function (command.cc)

---

## Command Execution: process_command() [command.cc]

**Located in:** `crawl-ref/source/command.cc`

**What it does:**
- Takes a command_type enum (CMD_MOVE_UP, CMD_CAST_SPELL, etc)
- Routes to appropriate handler function
- Sets `you.turn_is_over` if the command consumed a turn

**Important for integration:**
- Commands are enum values, not strings
- We can call `process_command(CMD_X, CMD_NO_CMD)` from code
- This is how we'll execute programmatic actions

**Relevant lines:**
```cpp
// Line 1295 in _input():
if (!you.turn_is_over && cmd != CMD_NEXT_CMD)
    ::process_command(cmd, real_prev_cmd);
```

**Files to examine next phase:**
- `command.h` - Command type definitions
- `command.cc` - Command dispatch logic
- `cmd-*.cc` - Individual command implementations

---

## Player Data: you.* [you.h, player.h]

**Located in:** `crawl-ref/source/you.h` and `crawl-ref/source/player.h`

**Key fields (from code inspection):**
- `you.your_name` - Character name (string)
- `you.species` - Player species (enum)
- `you.char_class` - Player job/class (enum)
- `you.hp` - Current hitpoints
- `you.max_hp` - Maximum hitpoints
- `you.x, you.y` - Player position in dungeon
- `you.turn_is_over` - Boolean: did this turn consume a full turn?
- `you.cannot_act()` - Method: is player stunned/incapacitated?
- `you.pending_revival` - Boolean: pending revival from death?
- `you.elapsed_time_at_last_input` - Timestamp
- `you.duration[DUR_*]` - Duration timers for various effects
- `you.props` - Property map (key-value storage for effects)

**We'll detail this more in Phase 0.2 research.**

---

## Game State: crawl_state [crawl-state-type.h]

**Key fields:**
- `crawl_state.type` - Game type (normal, sprint, tutorial, etc)
- `crawl_state.game_is_tutorial()` - Is this tutorial mode?
- `crawl_state.last_game_exit` - Info about why game ended
- `crawl_state.seen_hups` - Did we receive terminal close signal?

**We'll detail this more in Phase 0.2 research.**

---

## Flow Diagram: From Startup to First Turn

```
startup_step() [startup.cc]
    ├─ Load existing save OR run character creation menu
    ├─ Call _setup_generic() [ng-setup.cc:414]
    └─ Return: true (new game) or false (loaded save)
         ↓
    _launch_game() [main.cc:424] continues
    ├─ Print welcome message
    ├─ Print goal message if new game
    ├─ Print hints if hints mode
    ├─ save_game_state() - Crash recovery save
    ├─ run_uncancels() - Cleanup from character creation
    └─ Enter main loop
         ↓
    _input() [main.cc:1080] - First turn
    ├─ Check for pending revivals/level-ups
    ├─ Check if player can act
    ├─ _get_next_cmd() - Wait for player input (BLOCKS HERE)
    ├─ process_command() - Execute the command
    └─ world_reacts() - Monsters move, effects tick down
         ↓
    _input() called again - Second turn
    ├─ ...same as above...
         ↓
    (infinite loop until game_ended_condition thrown)
```

---

## Integration Points (for Phase 1)

1. **Creating characters without UI:**
   - Research `startup_step()` and `_setup_generic()` to see if we can call them from code
   - May need to set global state before calling (game_type, etc)

2. **Executing commands from code:**
   - Call `process_command(cmd, prev_cmd)` with our desired command
   - Important: this modifies global player state (you.*, turn_is_over, etc)
   - Must ensure `you` struct is properly initialized first

3. **Reading game state:**
   - Access player info via `you.` struct
   - Access monsters/dungeon via functions (not yet researched)
   - May need to call screen update functions to get monster/item visibility

4. **Exiting/saving programmatically:**
   - Throw `game_ended_condition` with reason and message
   - Or call `save_game()` and then throw exception

5. **World reactions and AI:**
   - `world_reacts()` handles all monster/effect simulation
   - Called once per player turn
   - Must understand what state changes it makes

---

## Next Research Tasks

- **Phase 0.2:** Game state structures (you.h, player.h, dungeon access)
- **Phase 0.3:** Command routing (command.h, command.cc, cmd-*.cc)
- **Phase 0.4:** Character creation integration (newgame.cc, ng-setup.cc)
- **Phase 0.5:** Scripting layer (Lua integration)

---

## Files Modified/Examined

- `main.cc:240-500` - Main entry and startup
- `main.cc:1080-1350+` - Input loop
- `startup.cc:1013` - Character creation/loading

## Key Insight

**The game is purely event-driven from `_input()`:**
1. We can inject commands via `process_command()`
2. We can read state after commands execute
3. We can loop: get state → send command → get state → loop
4. **This is perfect for an external narrator AI** - we read game state, tell it to the narrator, get back prose actions, convert to commands, execute, repeat

---

**Research Date:** 2026-04-16
**Checkpoint:** Phase 0.1 Complete
