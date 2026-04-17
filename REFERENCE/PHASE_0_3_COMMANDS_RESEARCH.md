# Phase 0.3: DCSS Command Execution - Research Findings

**Status:** ✅ COMPLETE - Research findings documented

---

## Overview

DCSS uses an event-driven command system where all player actions are routed through a giant switch statement in `process_command()`. This is perfect for external control - we can call `process_command(CMD_X)` with any command and it will execute as if the player typed it.

---

## Command System Architecture

```
User Input (Keyboard, Mouse, Macros)
    ↓
_get_next_cmd() [main.cc:1250+]
    ↓
Returns: command_type enum
    ↓
_input() receives command [main.cc:1080]
    ↓
process_command(cmd, prev_cmd) [main.cc:2094]
    ↓
Giant switch statement on command_type
    ↓
Calls specific action function for that command
    ↓
Sets you.turn_is_over = true (if action consumed a turn)
    ↓
Returns to _input()
    ↓
If turn_is_over, world_reacts() runs simulation
```

**Key insight:** We can inject commands at the `process_command()` call and skip player input entirely.

---

## Command Types [command-type.h:3+]

**Location:** `crawl-ref/source/command-type.h`

All commands are defined as enums starting from `CMD_NO_CMD = 2000`. Here are the major categories:

### Movement Commands
```cpp
CMD_MOVE_LEFT, CMD_MOVE_DOWN, CMD_MOVE_UP, CMD_MOVE_RIGHT
CMD_MOVE_UP_LEFT, CMD_MOVE_DOWN_LEFT, CMD_MOVE_UP_RIGHT, CMD_MOVE_DOWN_RIGHT

// Run (continuous movement until obstacle)
CMD_RUN_LEFT, CMD_RUN_DOWN, CMD_RUN_UP, CMD_RUN_RIGHT
CMD_RUN_UP_LEFT, CMD_RUN_DOWN_LEFT, CMD_RUN_UP_RIGHT, CMD_RUN_DOWN_RIGHT

// Safe move (won't attack monsters)
CMD_SAFE_WAIT
CMD_SAFE_MOVE_LEFT, CMD_SAFE_MOVE_DOWN, CMD_SAFE_MOVE_UP, CMD_SAFE_MOVE_RIGHT
CMD_SAFE_MOVE_UP_LEFT, CMD_SAFE_MOVE_DOWN_LEFT, CMD_SAFE_MOVE_UP_RIGHT, CMD_SAFE_MOVE_DOWN_RIGHT
```

### Attack Commands
```cpp
CMD_ATTACK_LEFT, CMD_ATTACK_DOWN, CMD_ATTACK_UP, CMD_ATTACK_RIGHT
CMD_ATTACK_UP_LEFT, CMD_ATTACK_DOWN_LEFT, CMD_ATTACK_UP_RIGHT, CMD_ATTACK_DOWN_RIGHT
CMD_PRIMARY_ATTACK  // Use primary weapon attack

CMD_CLOSE_DOOR_LEFT, CMD_CLOSE_DOOR_DOWN, etc. (8 directions)
CMD_CLOSE_DOOR      // Close door in current direction
CMD_OPEN_DOOR
```

### Action Commands
```cpp
CMD_WAIT           // Do nothing, end turn
CMD_REST            // Rest until health/mana recover
CMD_GO_UPSTAIRS
CMD_GO_DOWNSTAIRS

// Spell/Ability casting
CMD_CAST_SPELL          // Select spell to cast
CMD_FORCE_CAST_SPELL    // Force cast without targeting confirmation
CMD_MEMORISE_SPELL      // Learn a new spell
CMD_DISPLAY_SPELLS      // Show spell list
CMD_USE_ABILITY         // Use character ability
```

### Item Commands
```cpp
CMD_PICKUP          // Pick up item under player
CMD_PICKUP_QUANTITY // Pick up quantity (e.g., 3 potions)
CMD_DROP            // Drop an item
CMD_DROP_LAST       // Drop last item

CMD_WIELD_WEAPON    // Equip a weapon
CMD_WEAR_ARMOUR     // Wear armor
CMD_WEAR_JEWELLERY  // Equip ring/amulet
CMD_REMOVE_ARMOUR   // Remove armor
CMD_REMOVE_JEWELLERY// Remove ring/amulet
CMD_WEAPON_SWAP     // Swap to alternate weapon

CMD_QUAFF           // Drink a potion
CMD_READ            // Read a scroll
CMD_EVOKE           // Use an item
CMD_ZAP_WAND        // Zap a wand
CMD_FIRE            // Fire ranged weapon
```

### Inspection Commands (Don't consume turn)
```cpp
CMD_LOOK_AROUND     // Examine surroundings
CMD_INSPECT_FLOOR   // Look at items on floor
CMD_FULL_VIEW       // Detailed view of visible area

CMD_DISPLAY_CHARACTER_STATUS   // Show char info
CMD_DISPLAY_INVENTORY          // Show backpack
CMD_DISPLAY_SKILLS             // Show skill menu
CMD_DISPLAY_MUTATIONS          // Show mutations
CMD_DISPLAY_RELIGION           // Show god info
CMD_DISPLAY_SPELLS             // Show spells
CMD_DISPLAY_MAP                // Show dungeon map
CMD_EXPERIENCE_CHECK           // Show XP info
```

### Game Menu Commands
```cpp
CMD_SAVE_GAME           // Save and continue
CMD_SAVE_GAME_NOW       // Save immediately
CMD_SUSPEND_GAME        // Suspend to menu
CMD_QUIT                // Quit game (with save prompt)
CMD_REDRAW_SCREEN       // Redraw display
CMD_CHARACTER_DUMP      // Save character sheet
```

---

## process_command() Function [main.cc:2094]

**Location:** `crawl-ref/source/main.cc:2094-2400+`

**Function signature:**
```cpp
void process_command(command_type cmd, command_type prev_cmd = CMD_NO_CMD)
```

**What it does:**
1. Sets `you.apply_berserk_penalty = true` (reset each turn)
2. Handles CMD_GAME_MENU by showing menu
3. Giant switch statement on cmd
4. Each case calls appropriate function
5. Many actions set `you.turn_is_over = true`

**Key code locations (from inspection):**

```cpp
void process_command(command_type cmd, command_type prev_cmd)
{
    you.apply_berserk_penalty = true;

    // Game menu redirect
    if (cmd == CMD_GAME_MENU)
    {
        GameMenu m;
        m.show();
        if (m.cmd == CMD_NO_CMD)
            return;
        cmd = m.cmd;
    }

    // Massive switch statement
    switch (cmd)
    {
        case CMD_MOVE_UP: move_player_action({0, -1}); break;
        case CMD_MOVE_DOWN: move_player_action({0, 1}); break;
        case CMD_MOVE_LEFT: move_player_action({-1, 0}); break;
        case CMD_MOVE_RIGHT: move_player_action({1, 0}); break;
        
        case CMD_WAIT: 
            update_acrobat_status();
            you.turn_is_over = true;
            break;
        
        case CMD_CAST_SPELL:
            do_cast_spell_cmd(false); 
            break;
        
        case CMD_QUAFF:
            use_an_item(OPER_QUAFF); 
            break;
        
        // ... hundreds more cases ...
        
        default:
            // Unknown command
            break;
    }
}
```

---

## Critical Control Field: turn_is_over

**In:** `player` class (you.turn_is_over)

**Type:** bool

**Semantics:**
- `true` = action consumed a full turn, world will react
- `false` = action did not consume turn (e.g., viewing inventory)

**How it works:**
1. Before executing command, `you.turn_is_over` is typically false
2. Command handler sets it to true if action consumes a turn
3. After `process_command()` returns, main loop checks `you.turn_is_over`
4. If true, calls `world_reacts()` to run AI/effects
5. If false, loops back to input waiting

**Example flow:**
```cpp
// CMD_WAIT - consumes turn
case CMD_WAIT:
    update_acrobat_status();
    you.turn_is_over = true;  // ← Sets turn over
    break;

// CMD_DISPLAY_INVENTORY - doesn't consume turn
case CMD_DISPLAY_INVENTORY:
    display_inventory();
    break;  // ← Doesn't set turn_is_over, so stays false
```

**For Narrator integration:** We must respect this:
- Queries/displays should NOT set turn_is_over
- Actions should set turn_is_over = true
- After each command, check if turn_is_over to know if world reacts

---

## Input Loop Integration Point

From main.cc:1080, the _input() function:

```cpp
static void _input()
{
    // ... setup ...
    
    // Get next command
    command_type cmd = _get_next_cmd();
    
    // Execute it
    if (!you.turn_is_over && cmd != CMD_NEXT_CMD)
        ::process_command(cmd, real_prev_cmd);  // ← Command executes here
    
    // React if turn consumed
    if (you.turn_is_over)
    {
        world_reacts();  // ← Engine simulation runs here
    }
}
```

**For external code:**
- Call `::process_command(CMD_X, CMD_NO_CMD)` to execute a command
- Check `you.turn_is_over` after to know if turn advanced
- If turn advanced, understand that `world_reacts()` will be called next iteration

---

## Command Execution Examples

### Movement
```cpp
// Move player up
process_command(CMD_MOVE_UP, CMD_NO_CMD);

// This calls: move_player_action({0, -1});
// Which updates player position and sets you.turn_is_over = true
```

### Wait/Rest
```cpp
// Wait one turn
process_command(CMD_WAIT, CMD_NO_CMD);
// Sets you.turn_is_over = true

// Rest until recovered
process_command(CMD_REST, CMD_NO_CMD);
// Sets you.turn_is_over = true
```

### Casting Spell
```cpp
// Cast spell (will prompt for spell selection)
process_command(CMD_CAST_SPELL, CMD_NO_CMD);

// This calls: do_cast_spell_cmd(false);
// Which prompts for spell, target, and executes
// Sets you.turn_is_over = true if spell cast
```

### Query (No Turn Consumed)
```cpp
// Display character status
process_command(CMD_DISPLAY_CHARACTER_STATUS, CMD_NO_CMD);
// Doesn't set you.turn_is_over (stays false)

// Display spell list
process_command(CMD_DISPLAY_SPELLS, CMD_NO_CMD);
// Doesn't set you.turn_is_over (stays false)
```

---

## Bypass Player Input: External Command Injection

**The opportunity for Narrator integration:**

Instead of the normal input flow:
```
Player input (keyboard/mouse)
  ↓
_get_next_cmd()
  ↓
process_command()
```

We can do:
```
Narrator analysis of game state
  ↓
Generate prose action description
  ↓
Convert to command_type
  ↓
Call process_command(cmd, CMD_NO_CMD) directly
  ↓
Skip _get_next_cmd() entirely
```

**Implementation strategy:**
1. Read game state (you.*, env.*)
2. Pass to Narrator AI
3. Get back prose action
4. Parse/convert to command_type
5. Call `::process_command(cmd, CMD_NO_CMD)`
6. Check `you.turn_is_over` to know if turn advanced
7. Loop back to step 1

---

## Command Availability & Restrictions

**Not all commands work in all situations:**

```cpp
// Commands that check player status:
case CMD_USE_ABILITY:
    if (!activate_ability())  // ← May fail if can't use ability
        flush_input_buffer(FLUSH_ON_FAILURE);
    break;

case CMD_MEMORISE_SPELL:
    if (!learn_spell())  // ← May fail if can't memorize
        flush_input_buffer(FLUSH_ON_FAILURE);
    break;

case CMD_PRIMARY_ATTACK:
    quiver::get_primary_action()->trigger();
    if (!you.turn_is_over)  // ← Check if action was valid
        flush_input_buffer(FLUSH_ON_FAILURE);
    break;
```

**For integration:** Some commands may silently fail if preconditions aren't met. We need to:
1. Check preconditions before issuing command (e.g., "Can I cast this spell?")
2. Or accept that invalid commands might not set turn_is_over

---

## Related Functions

**Getting next command:**
- `_get_next_cmd()` [main.cc:1250+] - Blocks waiting for input, returns command_type
- `_keycode_to_command()` [main.cc] - Converts key press to command

**Command validation:**
- `is_valid_command(command_type)` - Check if command is valid in current state

**Command info:**
- `command_to_string(command_type)` - Get command name as string
- `string_to_command(string)` - Parse string to command_type

---

## Integration Challenges & Notes

1. **Modal commands:** Some commands like spell selection prompt for additional input. When called from code, they'll prompt the player (which we don't want). May need to refactor or mock these.

2. **Screen updates:** Many commands trigger screen redraws. This is fine for normal gameplay but may cause visual artifacts if used programmatically.

3. **Input buffer:** The input system has a queue/buffer. Calling process_command() directly bypasses this. Should be fine but worth monitoring.

4. **Macros:** The system supports macros and key recording. External command execution might interact oddly with these.

5. **Prev command tracking:** `prev_cmd` is used for repeat commands and some context-dependent behaviors. We should pass `CMD_NO_CMD` for most external calls.

---

## Next Research Tasks

- **Phase 0.4:** Character creation integration (how to create characters from code)
- **Phase 0.5:** Lua scripting layer (is there already an API?)
- **Phase 0 Summary:** Write complete integration guide

---

## Files Examined

- `command-type.h:1-150+` - Command enum definitions
- `command.h:1-41` - Command function declarations
- `main.cc:2094-2400+` - process_command() implementation
- `main.cc:1080-1350` - _input() function

---

**Research Date:** 2026-04-16
**Checkpoint:** Phase 0.3 Complete

**Key Takeaway:** Commands are executed via `process_command(cmd, prev_cmd)`. Each command handler sets `you.turn_is_over = true` if the action consumes a turn. This is the perfect injection point for external AI-driven control.
