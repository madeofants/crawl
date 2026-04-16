# Creating GAME_TYPE_NARRATIVE

## Problem

Current MVP tries to call `setup_game()` directly, but that skips important initialization steps that `startup_step()` handles:
- Checking save existence
- Running `_post_init()` 
- Setting up game state properly

## Solution

Create a new `GAME_TYPE_NARRATIVE` that:
1. Is recognized by DCSS game type system
2. Signals to `startup_step()` that it should use our character creation
3. Skips the UI menu  
4. Calls the proper initialization sequence

## Implementation

### Step 1: Add to game-type.h

```cpp
enum game_type
{
    GAME_TYPE_UNSPECIFIED,
    GAME_TYPE_NORMAL,
    GAME_TYPE_TUTORIAL,
    GAME_TYPE_ARENA,
    GAME_TYPE_SPRINT,
    GAME_TYPE_HINTS,
    GAME_TYPE_ZOTDEF,
    GAME_TYPE_INSTRUCTIONS,
    GAME_TYPE_HIGH_SCORES,
    GAME_TYPE_CUSTOM_SEED,
    GAME_TYPE_DESCENT,
    GAME_TYPE_NARRATIVE,    // ← ADD THIS
    NUM_GAME_TYPE
};
```

### Step 2: Handle in startup_step()

In startup.cc, add a case for GAME_TYPE_NARRATIVE that:
- Bypasses menu
- Calls setup_game() 
- Calls _post_init()
- Returns properly

### Step 3: Update narrator_control.cc

Set `ng.type = GAME_TYPE_NARRATIVE` instead of GAME_TYPE_NORMAL

### Step 4: Add any necessary name/id functions

If there are functions that need to know about GAME_TYPE_NARRATIVE:
- `game_type_name()` 
- `is_playable_game_type()`
- Any other game type switches

## Files to Modify

1. `game-type.h` - Add GAME_TYPE_NARRATIVE to enum
2. `startup.cc` - Add handling in startup_step()
3. `narrator_control.cc` - Use GAME_TYPE_NARRATIVE
4. Any files with game type switches (check grep results)

## What This Enables

Once GAME_TYPE_NARRATIVE exists:
- `narrator_control.cc` can use it
- Game initializes properly through startup flow
- World is generated
- Characters play normally
- Can run turns

## Next Action

Search for all references to GAME_TYPE_* to find what needs updating
