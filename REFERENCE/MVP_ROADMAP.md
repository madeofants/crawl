# MVP Roadmap: "Start a game and have a world generated"

## Reality Check

DCSS **already does this**. The game:
- Generates a multi-level world (D:1-D:27 plus branches)
- Places monsters, items, terrain
- Simulates it with a full game loop
- Handles all physics and AI

**Our MVP isn't building new features - it's just bypassing the UI.**

## What We Already Have (From Phase 0 Research)

| Component | Status | Location | Evidence |
|-----------|--------|----------|----------|
| Character Creation | ✓ Works | `setup_game()` in ng-setup.cc | Tested, generates valid character |
| World Generation | ✓ Works | Automatic in `setup_game()` | Generates D:1 immediately |
| Game Loop | ✓ Works | `_input()` in main.cc | Runs each turn |
| Monster AI | ✓ Works | `world_reacts()` | Monsters move and attack |
| Physics | ✓ Works | DCSS engine | Gravity, damage, effects |
| State Querying | ✓ Works | `you`, `env`, `crawl_state` globals | Direct read access |

## What We Need to Add

### Sprint 1: "Bypass the UI Menu" (2-3 hours)

**Goal:** Start a game without touching the DCSS UI

**What to build:**
1. Simple character creation wrapper
   - Input: name, species, job
   - Call: `setup_game(newgame_def)` directly
   - Output: Game running with character in world
   
2. World state reader
   - Read: player position, HP, monsters, items
   - No turn consumption - just query `you` and `env` globals
   - Output: JSON/struct with world snapshot

3. Simple turn executor
   - Input: "run 5 turns"
   - Call: Game loop 5 times
   - Output: World evolved, state changed

**Success Criteria:**
- [ ] Create character from code (not UI)
- [ ] Read initial world state
- [ ] Execute 5 turns
- [ ] World changes (monsters move, turns increment)
- [ ] Can read new world state

**Commits:**
1. `feat: Add character creation bypass (narrator_create_char.cc)`
2. `feat: Add world state reader (narrator_state_reader.cc)`
3. `feat: Add turn executor (narrator_turn_loop.cc)`
4. `test: Add MVP integration test`

**Time:** 2-3 hours total

---

## Architecture: Keep It Simple

```
Direct C++ Calls (No UI):

narrator_create_game("name", "human", "fighter")
    ↓ (calls setup_game())
DCSS initializes world
    ↓
narrator_get_world()
    ↓ (reads you, env globals)
Return world state
    ↓
narrator_run_turns(5)
    ↓ (calls game loop 5x)
World evolves
    ↓
narrator_get_world()
    ↓
Return updated state
```

**Files to Create:**
- `narrator_control.h` - public API
- `narrator_control.cc` - implementation
- `narrator_control_test.cc` - tests

**Files to Modify:**
- Makefile - add narrator_control.o to build

---

## Why This Works

1. **DCSS is already modular** - character creation, world generation, turn loop are all separate functions
2. **We don't need to understand everything** - just call existing functions in right order
3. **No UI means instant execution** - no blocking on player input
4. **State is accessible** - `you` and `env` are globals we can query

---

## Next Phase (After MVP)

Once we have "start game + see world" working, we can add:
- Action interpreter (understand prose)
- Command compiler (convert to DCSS commands)
- Narration generator (describe state in prose)

But that's **Phase 2**. For now: just make DCSS run headless.

---

## Files Summary

### narrator_control.h
```cpp
bool narrator_create_game(name, species, job);
narrator_world_state narrator_get_world();
void narrator_run_turns(int n);
bool narrator_is_running();
```

### narrator_control.cc
```cpp
// narrator_create_game: Call setup_game(newgame_def)
// narrator_get_world: Read you, env globals into struct
// narrator_run_turns: Call _input() N times
// narrator_is_running: Check you.is_dead() and crawl_state
```

### narrator_control_test.cc
```cpp
int main() {
    narrator_create_game("Test", "human", "fighter");
    auto state = narrator_get_world();
    assert(state.player_name == "Test");
    narrator_run_turns(5);
    auto state2 = narrator_get_world();
    assert(state2.turn > state.turn);
    return 0;
}
```

---

## Status

- [x] Phase 0: Research complete - understand DCSS architecture
- [ ] MVP Sprint 1: Build character creation bypass
- [ ] MVP Sprint 2: Build world reader
- [ ] MVP Sprint 3: Build turn executor
- [ ] **GOALPOST**: Start game, generate world, execute turns

---

**Estimated Time to Goalpost:** 2-3 hours of focused coding
