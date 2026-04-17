# Phase 0.5: DCSS External Interface & Lua Scripting - Research Findings

**Status:** ✅ COMPLETE - Research findings documented

---

## Overview

DCSS has **extensive Lua scripting support** through the CLua and DLua systems:
- **CLua** - "Crawl Lua" - User-facing scripts (macros, RC files)
- **DLua** - "Dungeon Lua" - Dungeon generation and level scripts

Both systems are fully integrated and can call C++ code and vice versa.

---

## Lua Integration Architecture

**Two separate Lua contexts:**

1. **CLua (clua.h, clua.cc)**
   - User-facing scripting
   - Accessed via RC files and macros
   - Called during gameplay via `crawl_state.clua`
   - Can hook into ready(), input(), and other events

2. **DLua (dlua.h, dlua.cc)**
   - Dungeon generation scripting
   - Accessed via level design (.des files)
   - Called during level creation
   - Can define vaults, monsters, items

**Global instances:**
```cpp
extern CLua clua;  // User Lua
extern CLua dlua;  // Dungeon Lua
```

---

## CLua: User-Facing Scripting [clua.h]

**Location:** `crawl-ref/source/clua.h`, `crawl-ref/source/clua.cc`

**Key methods:**
```cpp
class CLua
{
public:
    lua_State *state();                    // Get raw Lua state
    bool callfn(string name, int args, int returns);  // Call Lua function
    
    bool load_file(string filename);       // Load .lua file
    bool exec_chunk(string code, string mode = "");  // Execute code string
    
    bool call_function(string name, ...);  // Variable arg function call
    
    string error;  // Last error message
    
    // ... many more ...
};
```

**Usage in DCSS:**
```cpp
// Call user's ready() function (called each turn if defined)
if (!clua.callfn("ready", 0, 0))
{
    mprf(MSGCH_ERROR, "Lua error: %s", clua.error.c_str());
}
```

---

## Hook Points: Where Lua Can Intervene

**From main.cc line 1231, during each turn:**
```cpp
// Before getting player input
if (!clua.callfn("ready", 0, 0))  // ← User's ready() function
{
    if (crawl_state.lua_script_killed)
        crawl_state.lua_ready_throttled = true;
    mprf(MSGCH_ERROR, "Lua error: %s", clua.error.c_str());
}
```

**Available Lua hooks (from RC file integration):**
- `ready()` - Called each turn before input
- `input()` - Can override input handling
- Message hooks - React to game messages
- Startup hooks - During character creation
- Various ability/spell hooks

---

## Calling Lua from C++

**Simple function call:**
```cpp
// Call Lua function: my_function(42, "hello")
lua_State *L = clua.state();
lua_getglobal(L, "my_function");
lua_pushnumber(L, 42);
lua_pushstring(L, "hello");
lua_pcall(L, 2, 1, 0);  // 2 args, 1 return value
int result = lua_tointeger(L, -1);
```

**Using CLua wrapper:**
```cpp
clua.callfn("my_function", 2, 1);  // 2 args, 1 return value
```

---

## Calling C++ from Lua

**Registered C++ functions available to Lua scripts:**

From clua.cc and cluautil.cc, these functions are bound:

```lua
-- Player information
you.name              -- Character name
you.race              -- Species name
you.class             -- Job name
you.hp                -- Current hitpoints
you.hp_max            -- Max hitpoints
you.mp                -- Current mana
you.mp_max            -- Max mana
you.exp               -- Experience
you.level             -- Character level
you.pos()             -- Current position {x, y}
you.branch()          -- Current branch
you.depth()           -- Current depth

-- Dungeon information
crawl.current_level() -- Current level_id
crawl.level_name()    -- Level name
crawl.view_geometry() -- Viewport dimensions

-- Game control
crawl.messages()      -- Get recent messages
crawl.take_input()    -- Get player input
crawl.sendkeys()      -- Inject key presses (!)

-- And many more...
```

**Key for Narrator integration:**
- Lua scripts can call `crawl.sendkeys()` to execute commands
- Lua can read game state via you.* and crawl.*
- Lua runs in a sandbox (throttled for security)

---

## Sending Commands via Lua

**The ready() hook approach:**

```lua
-- In RC file or loaded script:
function ready()
    -- This is called each turn
    -- We can send commands back to the game!
    
    -- Examples:
    crawl.sendkeys('m')      -- Move up
    crawl.sendkeys('j')      -- Move down
    crawl.sendkeys('h')      -- Move left
    crawl.sendkeys('l')      -- Move right
    crawl.sendkeys('zs')     -- Zap spell
    crawl.sendkeys('f')      -- Fire missile
    crawl.sendkeys('5')      -- Rest/wait
end
```

**But this bypasses normal input flow!**

From main.cc:1114-1123:
```cpp
if (crawl_state.is_replaying_keys() && crawl_state.is_repeating_cmd()
    && kbhit())
{
    // User pressed a key, so stop repeating
    crawl_state.cancel_cmd_repeat("Key pressed, interrupting...");
    crawl_state.prev_cmd = CMD_NO_CMD;
    return;
}
```

---

## Binding Custom C++ Functions to Lua

**How DCSS makes C++ functions available to Lua:**

In clua.cc, functions are registered via `luaL_Reg`:

```cpp
static const luaL_Reg clualib[] = {
    { "take_input", clua_take_input },
    { "sendkeys", clua_sendkeys },
    { "replay_messages", clua_replay_messages },
    // ... many more ...
    { nullptr, nullptr }
};
```

**The function wrappers:**
```cpp
static int clua_sendkeys(lua_State *L)
{
    // Called from Lua: crawl.sendkeys("mj")
    string keys = luaL_checkstring(L, 1);
    // Push keys into input buffer
    for (char c : keys)
        macro_buf_add(c);
    return 0;  // No return value
}
```

**For Narrator integration:**
- We could register our own C++ functions to Lua
- Make them callable from Lua scripts
- Example: `narrator.analyze(state)` returns AI decision
- Or: `narrator.send_command(cmd)` executes command programmatically

---

## API Available to Lua Scripts

**From cluautil.cc and various bindings:**

```lua
-- Messages
crawl.message(msg)           -- Print message to player
crawl.mpr(msg, color)        -- Print colored message
crawl.print(str)             -- Print text

-- Game state queries
you.name / you.race / you.class
you.hp / you.hp_max / you.mp / you.mp_max
you.xlvl / you.skill(skillname)
you.mutation(mutationname)
you.status()
you.inventory()

-- Targeting and positioning
crawl.los_exists(pos)        -- Can see position?
crawl.adjacent_cell(pos)     -- Adjacent cell exists?
crawl.pos_to_target(x, y)    -- Get target at position

-- Map and level
crawl.getline(prompt)        -- Get input from user (!)
crawl.getch()                -- Get single keypress
crawl.sendkeys(keys)         -- Inject key presses
```

---

## Limitations of Current Lua Integration

1. **No direct command dispatch** - Lua can't call `process_command()` directly
   - Workaround: Use `sendkeys()` to inject key codes
   - Problem: Keys then go through normal input processing

2. **Sandbox/throttling** - Lua scripts are throttled for security
   - Scripts that run too long are killed
   - MAX_THROTTLE_SLEEPS = 1000 calls

3. **Event-driven only** - Lua runs during game events
   - Called from ready(), not a continuous loop
   - Can't make decisions between commands

4. **Limited state access** - Lua sees game state but can't easily traverse all data
   - you.* and crawl.* are exposed
   - But dungeon/monster details might need C++ binding

---

## Integration Opportunity: Custom Narrator Binding

**We can extend Lua bindings to add Narrator API:**

Option 1: Register custom C++ functions to Lua
```cpp
// In a new file: narrator_lua.cc
static int narrator_get_state(lua_State *L)
{
    // Call Narrator API to get current state description
    // Return as Lua table or string
}

static int narrator_execute_action(lua_State *L)
{
    // Narrator decision converted to command
    // Execute via process_command()
}

void bind_narrator_to_lua()
{
    lua_register(clua.state(), "narrator_get_state", narrator_get_state);
    lua_register(clua.state(), "narrator_execute_action", narrator_execute_action);
}
```

Option 2: Write pure Lua integration
```lua
-- In RC file:
function narrator_turn()
    local state = crawl.current_state()
    -- Send state to Narrator AI (external process?)
    -- Get back action
    -- Execute: crawl.sendkeys(action_keys)
end

function ready()
    narrator_turn()
end
```

---

## Best Approach for Narrator

**My recommendation: Hybrid approach**

1. **Keep Lua as secondary interface** - Good for light scripting
2. **Use C++ for primary Narrator integration** - We already have process_command()
3. **Later: Write Lua bindings for Narrator** - For user scripts

**Phase 1 approach (simpler):**
- Modify main.cc to inject Narrator control
- Call process_command() directly (not via Lua)
- Narrator runs as external AI/process
- DCSS acts as simulation engine

**Phase 2+ approach (more flexible):**
- Register Narrator functions to Lua
- Allow RC files to use Narrator
- User Lua can call Narrator and get AI decisions
- More integrated with DCSS ecosystem

---

## Existing DLua Usage (Not Needed for Narrator)

**DLua handles dungeon generation:**

```cpp
// From dungeon.cc
dlua.callfn("dgn_clear_persistant_data", "");
dlua.callfn("make_level", &br, &level_id);
```

**DLua scripts (.des files):**
- Define vaults (mini-levels)
- Place monsters and items
- Create branches
- Used by dungeon generation, not gameplay

**For Narrator: Not relevant** (we're not creating custom dungeons, just playing through existing ones)

---

## Files Examined

- `clua.h:1-100+` - CLua class interface
- `clua.cc` - CLua implementation
- `cluautil.cc` - Lua bindings for game functions
- `main.cc:1231` - ready() hook call
- `dlua.h/dlua.cc` - DLua (dungeon Lua)

---

## Research Summary: Lua in DCSS

**Findings:**
1. ✅ Lua is fully integrated and mature
2. ✅ Can read game state via you.* and crawl.*
3. ✅ Can inject commands via crawl.sendkeys()
4. ⚠️ sendkeys() goes through input buffer (indirect)
5. ✅ Can register new C++ functions to Lua
6. ✅ ready() hook is called each turn

**For Narrator:**
- Option A: Bypass Lua, call process_command() directly from C++ (simpler, faster)
- Option B: Create Lua bindings, allow more flexible scripting (more powerful, more work)
- **Recommendation: Start with Option A, add Option B later**

---

## Next Phase: Phase 0 Summary

Time to consolidate all findings into a complete integration guide for Phase 1 implementation.

---

**Research Date:** 2026-04-16
**Checkpoint:** Phase 0.5 Complete

**Key Takeaway:** DCSS has mature Lua scripting with game state access and command injection. For Narrator integration, we can use direct C++ calls to process_command() for better control, or leverage Lua bindings for user scripts. Lua exists but isn't required for core Narrator functionality.
