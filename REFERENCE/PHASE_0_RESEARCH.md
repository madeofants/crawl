# Phase 0 Research - DCSS Internals Analysis

**Goal:** Understand DCSS architecture so we can build an integration layer that lets code programmatically create characters, read game state, and execute commands.

**Status:** Starting Phase 0.1

---

## Phase 0.1: Game Startup & Main Loop

### Research Task: Understand how DCSS starts and runs
**Files to examine:**
- `main.cc` (entry point)
- `end.cc` (game end conditions) 
- `ouch.cc` (death handling)
- `tags.cc` (save/load system)

**Questions to answer:**
1. What's the entry point? (`main()` in main.cc)
2. What happens between startup and first turn?
3. How does the game loop work?
4. Where can we inject code to execute commands programmatically?

**Expected findings:**
- Main game loop structure
- Where character creation happens vs game start
- How input is processed each turn
- Where state updates happen

---

## Phase 0.2: Game State Access

### Research Task: Understand how to read/write game state
**Files to examine:**
- `you.h` (player/character structure)
- `player.h` (character data)
- `level_state.h` (dungeon level state)
- `env.cc` (environment/world state)
- `act-aim.cc` (action/targeting - example of state usage)

**Questions to answer:**
1. What's the main data structure holding player info? (you.*)
2. How do we read player position, HP, inventory, etc?
3. How do we read monster/dungeon state?
4. What's mutable vs read-only?

**Expected findings:**
- Main player data structure (likely `you` global)
- How to access monsters, items, dungeon layout
- Permission model for code that needs to modify state

---

## Phase 0.3: Command Execution

### Research Task: Understand how commands are processed
**Files to examine:**
- `input.cc` (input handling)
- `command.cc` (command execution)
- `cmd-*.cc` (specific command implementations)
- `process_command()` or equivalent function

**Questions to answer:**
1. How does player input become a command?
2. How are commands validated and executed?
3. Can we call command functions directly from code?
4. What about async/wait handling?

**Expected findings:**
- Command dispatch mechanism
- How to trigger actions from code
- Validation/permission checks
- Whether we can skip player input layer

---

## Phase 0.4: Character Creation Integration

### Research Task: Understand full character creation flow
**Files to examine:**
- `newgame.cc` (UI - we've already studied this)
- `ng-setup.cc` (initialization - we've already studied this)
- `player-setup.cc` (if exists)
- Trace the full call stack: menu → validation → data setup

**Questions to answer:**
1. Can we programmatically create characters without UI?
2. Do we need to call the UI, or bypass it?
3. What data validation happens?
4. How do we inject custom species/jobs?

**Expected findings:**
- Functions that can be called from code for character creation
- Validation rules we need to respect
- Data structures for character definition
- Points where we can hook custom content

---

## Phase 0.5: External Interface Analysis

### Research Task: Understand if DCSS has a scripting/API layer
**Files to examine:**
- `clua.h` / `clua.cc` (Lua integration - if used)
- `dgn.cc` (dungeon generation - might have scripting hooks)
- Any documentation in `README.md` about modding

**Questions to answer:**
1. Is Lua already integrated? (for scripts)
2. Can we use that for our integration?
3. Are there any existing APIs for external control?
4. What's the architecture for plugins/mods?

**Expected findings:**
- Whether we need to invent an API from scratch
- Whether we can use existing scripting layers
- Best practices for extending DCSS

---

## Findings Storage

As you research each phase, document findings here:

### Phase 0.1 Findings
(To be filled in as we research)

### Phase 0.2 Findings  
(To be filled in as we research)

### Phase 0.3 Findings
(To be filled in as we research)

### Phase 0.4 Findings
(To be filled in as we research)

### Phase 0.5 Findings
(To be filled in as we research)

---

## Next Steps

1. Start with Phase 0.1 - examine main.cc and understand the game startup
2. Document what you find in this file
3. Each finding becomes a note for Phase 1 (Integration Layer design)
4. Once all 5 phases researched, write PHASE_0_SUMMARY.md with overall architecture

---

**Created:** 2025 (Session start for narrative engine project)
**Purpose:** Hobbyist modding reference for DCSS integration
**Scope:** Research only - no code changes yet
