# Crawl Narrative Engine - Granular Task Breakdown

## Phase 0: Foundation & Research

### 0.0: Architecture Research
- [ ] Read DCSS main.cc to understand game startup
- [ ] Identify main game loop entry point
- [ ] Find where turn execution happens
- [ ] Document the game loop flow
- [ ] Create diagram: "How DCSS Runs"

### 0.1: State Access Research
- [ ] Find player data struct (player_type or similar)
- [ ] List all player accessible stats: HP, MP, position, inventory
- [ ] Find dungeon/level data struct
- [ ] List all level accessible data: tiles, monsters, items, features
- [ ] Find where game state is stored (global? object?)
- [ ] Document: "How to Read Game State"

### 0.2: Command Execution Research
- [ ] Find input handling code
- [ ] Find where game commands are executed
- [ ] Identify: move, attack, cast, interact commands
- [ ] Find command validation/routing
- [ ] Document: "How Commands Execute"

### 0.3: Initialization Research
- [ ] Trace character creation to game start
- [ ] Find where player is placed in dungeon
- [ ] Find where first turn happens
- [ ] Identify all initialization steps
- [ ] Document: "Character Creation to First Turn"

### 0.4: External Interface Research
- [ ] Check if DCSS has an API layer
- [ ] Check if DCSS has a scripting layer (Lua?)
- [ ] Check for socket/network interface
- [ ] Document what interfaces exist
- [ ] Decide: Direct code linking? IPC? API?

### 0.5: Write Phase 0 Summary
- [ ] Document findings in REFERENCE folder
- [ ] Create "DCSS Internals" reference doc
- [ ] List all source files that matter
- [ ] Create task list for Phase 1

---

## Phase 1: DCSS Integration

### 1.0: Build Executable from Source
- [ ] Verify DCSS compiles cleanly
- [ ] Test the built executable runs
- [ ] Verify character creation works
- [ ] Verify game initializes and accepts input
- [ ] Document build process

### 1.1: State Reader - Basic
- [ ] Find player struct in code
- [ ] Create C++ function to read player HP
- [ ] Create C++ function to read player position
- [ ] Create C++ function to read player inventory
- [ ] Create C++ function to read player stats (str, int, dex)
- [ ] Compile and test each function

### 1.2: State Reader - World
- [ ] Create function to get current level/branch
- [ ] Create function to get current dungeon level
- [ ] Create function to list visible monsters
- [ ] Create function to list visible items
- [ ] Create function to get current visibility map (FOV)
- [ ] Test each function

### 1.3: State Reader - Export
- [ ] Create JSON struct to hold all state
- [ ] Create function to serialize state to JSON
- [ ] Test serialization
- [ ] Document JSON format

### 1.4: Command Executor - Basic
- [ ] Create function to send move command (north/south/east/west)
- [ ] Create function to send wait command
- [ ] Create function to send rest command
- [ ] Test commands execute and state updates
- [ ] Verify monsters take turns

### 1.5: Command Executor - Combat
- [ ] Create function to send attack command
- [ ] Create function to target enemy
- [ ] Test attack resolves correctly
- [ ] Verify damage applied to monster

### 1.6: Command Executor - Inventory
- [ ] Create function to pick up item
- [ ] Create function to drop item
- [ ] Create function to equip item
- [ ] Create function to use item
- [ ] Test inventory operations

### 1.7: Game Initialization
- [ ] Create function to start new game programmatically
- [ ] Create function to set starting species/job
- [ ] Create function to set game difficulty
- [ ] Verify character is created correctly
- [ ] Verify player appears in dungeon

### 1.8: Turn Loop
- [ ] Create master function that executes one turn:
  - [ ] Read current state
  - [ ] Send command
  - [ ] Execute turn
  - [ ] Read new state
  - [ ] Return delta (what changed)
- [ ] Test loop for 10 turns
- [ ] Verify state updates each turn

### 1.9: Testing Suite
- [ ] Create test: new game initializes
- [ ] Create test: player moves
- [ ] Create test: player fights monster
- [ ] Create test: state updates correctly
- [ ] Create test: commands are executed
- [ ] Run all tests and document results

### 1.10: Create Integration API
- [ ] Create header file with all public functions
- [ ] Document parameter types
- [ ] Document return values
- [ ] Create example code showing full loop
- [ ] Write "Integration API" reference doc

---

## Phase 2: Narrator Prototype

### 2.0: Setup Narrator Infrastructure
- [ ] Choose language (Python? C++? Node?)
- [ ] Create basic project structure
- [ ] Create connection to Phase 1 integration layer
- [ ] Verify can call Phase 1 functions

### 2.1: Prompt Generator - Static
- [ ] Create function to describe current location
- [ ] Create function to describe visible enemies
- [ ] Create function to describe player status
- [ ] Create function to list available actions
- [ ] Test with hardcoded game state

### 2.2: Prompt Generator - Dynamic
- [ ] Generate full prompt from game state
- [ ] Handle edge cases (no enemies, low health, etc.)
- [ ] Write 10+ example prompts
- [ ] Test variations

### 2.3: Action Parser - Basic
- [ ] Create parser for: "move north", "attack", "rest"
- [ ] Create parser for "go north", "hit enemy", "wait"
- [ ] Handle command variations
- [ ] Test with 20+ examples

### 2.4: Action Parser - Advanced
- [ ] Parse multi-part actions: "move north and search"
- [ ] Parse targeting: "attack the goblin"
- [ ] Parse weapon/spell: "cast fireball"
- [ ] Handle unclear input
- [ ] Test edge cases

### 2.5: Clarification System
- [ ] Detect ambiguous input (multiple enemies, unclear target)
- [ ] Generate clarifying question
- [ ] Wait for user response
- [ ] Resolve to single action
- [ ] Test with 10+ ambiguous scenarios

### 2.6: Action Executor
- [ ] Take parsed action
- [ ] Convert to Phase 1 command
- [ ] Execute via Phase 1 API
- [ ] Get results back
- [ ] Handle failures gracefully

### 2.7: Result Narrator
- [ ] Describe successful move: "You move north"
- [ ] Describe combat: "You attack the goblin for 5 damage"
- [ ] Describe death: "The orc slays you"
- [ ] Describe item pickup: "You find a sword"
- [ ] Test with various scenarios

### 2.8: Full Loop - Single Turn
- [ ] Chain all components: prompt → input → parse → execute → narrate
- [ ] Test with single manual turn
- [ ] Document flow
- [ ] Create example session

### 2.9: Full Loop - Multiple Turns
- [ ] Run 5 turns end-to-end
- [ ] Verify state consistency
- [ ] Verify narrative makes sense
- [ ] Check for errors/crashes

### 2.10: Create Narrator API
- [ ] Document narrator functions
- [ ] Create example code
- [ ] Write "Narrator System" reference doc

---

## Phase 3: Custom Game Mode

### 3.0: Design Narrative Mode
- [ ] Write design doc: what makes it different from normal DCSS?
- [ ] Decide: simplified dungeon? Difficulty? mechanics changes?
- [ ] List all modifications needed
- [ ] Get approval from team

### 3.1: Add Game Type Enum
- [ ] Add GAME_TYPE_NARRATIVE to game-type.h
- [ ] Compile and verify
- [ ] Test game can select this mode

### 3.2: Create Initialization Code
- [ ] Add init function for narrative mode in ng-setup.cc
- [ ] Set difficulty/branch restrictions
- [ ] Set starting equipment
- [ ] Test mode initializes correctly

### 3.3: Simplify Dungeon for Narrative
- [ ] Reduce dungeon branches (just Dungeon 1-3?)
- [ ] Reduce monster variety
- [ ] Reduce loot variety
- [ ] Test simplified dungeon generates
- [ ] Test is playable

### 3.4: Create Starting Scenario
- [ ] Choose starting location (oasis? camp? tavern?)
- [ ] Write opening narration
- [ ] Place first NPC or creature
- [ ] Test Narrator can describe it

### 3.5: Balance Combat Difficulty
- [ ] Play through encounters manually
- [ ] Adjust monster stats if too hard/easy
- [ ] Test medium-difficulty fights (winnable but challenging)
- [ ] Document balance changes

### 3.6: Test Full Scenario
- [ ] Run Narrator loop for 10 turns in narrative mode
- [ ] Verify all commands work
- [ ] Verify narration is appropriate
- [ ] Check for crashes or errors

### 3.7: Create Scenario Documentation
- [ ] Write "Narrative Mode" reference doc
- [ ] Explain design philosophy
- [ ] Document starting setup
- [ ] Provide example session

---

## Phase 4: Database Layer

### 4.0: Design Database Schema
- [ ] Create NPC table: id, name, description, dialogue_id
- [ ] Create dialogue table: npc_id, dialogue_tree (JSON)
- [ ] Create quest table: id, title, objectives, rewards
- [ ] Create location table: id, description, encounters
- [ ] Create item_flavor table: item_name, description, lore
- [ ] Create player_state table: save_id, state_data (JSON)
- [ ] Document schema

### 4.1: Set Up Database
- [ ] Choose database (SQLite? PostgreSQL?)
- [ ] Create database and tables
- [ ] Insert test data
- [ ] Verify queries work

### 4.2: Create Database Interface
- [ ] Create function to query NPC
- [ ] Create function to get NPC dialogue
- [ ] Create function to get quest details
- [ ] Create function to get location description
- [ ] Create function to get item description
- [ ] Test all queries

### 4.3: Integrate with Narrator
- [ ] Narrator queries NPC name/dialogue before narrating
- [ ] Narrator uses custom item descriptions
- [ ] Narrator includes quest context in prompts
- [ ] Test integrated system

### 4.4: Track Game Progress
- [ ] Create function to update player state in DB
- [ ] Create function to track NPC state changes
- [ ] Create function to track quest progress
- [ ] Create function to save game state
- [ ] Test state persistence

### 4.5: Load/Save Game
- [ ] Create function to save complete game state
- [ ] Create function to load game state
- [ ] Test save/load cycle
- [ ] Verify state is identical

### 4.6: Create Content File Format
- [ ] Define JSON/YAML format for NPC definitions
- [ ] Define format for quest definitions
- [ ] Define format for dialogue trees
- [ ] Write parser to load from files

### 4.7: Populate Example Content
- [ ] Write 5 NPCs with dialogue
- [ ] Write 3 quests
- [ ] Write 10 location descriptions
- [ ] Add custom item descriptions

### 4.8: Test Database System
- [ ] Test content loads correctly
- [ ] Test Narrator uses content
- [ ] Test save/load preserves NPC state
- [ ] Test quest tracking works

---

## Phase 5: Extended Gameplay

### 5.0: NPC Personality System
- [ ] Create personality traits (friendly, hostile, neutral)
- [ ] Create dialogue variations based on personality
- [ ] Create function to adjust NPC state based on player actions
- [ ] Test NPC dialogue changes based on interaction

### 5.1: NPC Dialogue Trees
- [ ] Create branching dialogue system
- [ ] Test dialogue choices affect NPC state
- [ ] Test dialogue choices affect reputation
- [ ] Test Narrator can follow dialogue trees

### 5.2: Quest System - Basic
- [ ] Create quest: "Kill 3 goblins"
- [ ] Track quest progress
- [ ] Detect quest completion
- [ ] Give reward
- [ ] Test end-to-end

### 5.3: Quest System - Complex
- [ ] Create multi-step quest
- [ ] Create quest with choices (different paths)
- [ ] Create quest with consequences
- [ ] Test all variations

### 5.4: Reputation System
- [ ] Track reputation with NPCs
- [ ] Track reputation with factions
- [ ] Modify dialogue based on reputation
- [ ] Modify quest availability based on reputation
- [ ] Test system works

### 5.5: NPC Combat Allies
- [ ] Create NPC that can fight
- [ ] Create function to command NPC in combat
- [ ] Test NPC attacks monsters
- [ ] Test NPC takes damage/dies

### 5.6: Faction System
- [ ] Create faction: Mages Guild
- [ ] Create faction-specific quests
- [ ] Create faction-specific dialogue
- [ ] Test reputation affects faction quests

### 5.7: Test Full Extended Game
- [ ] Run complete session with NPCs, quests, factions
- [ ] Verify all systems work together
- [ ] Test edge cases (reputation conflicts, etc.)

---

## Phase 6: Polish & Advanced

### 6.0: Save Game Format
- [ ] Define save file structure
- [ ] Create function to serialize all game state
- [ ] Create function to deserialize and restore
- [ ] Test save/load 5x in a row

### 6.1: Multiple Playthroughs
- [ ] Test new game after completion
- [ ] Test previous quest state doesn't carry over
- [ ] Test character progression carries over (if desired)
- [ ] Document new game+ rules

### 6.2: Error Handling
- [ ] Add error handling to all major functions
- [ ] Create error logging
- [ ] Test recovery from crashes
- [ ] Document error codes

### 6.3: Performance
- [ ] Profile Narrator response time
- [ ] Profile DCSS command execution time
- [ ] Optimize if > 2 second delay
- [ ] Document performance metrics

### 6.4: Mod Support
- [ ] Create mod loading system
- [ ] Allow custom content files
- [ ] Test loading 3+ custom mods
- [ ] Document mod creation format

### 6.5: Final Testing
- [ ] Run 20-turn session
- [ ] Verify no crashes
- [ ] Verify narrative quality
- [ ] Collect player feedback

---

## Task Count by Phase

- **Phase 0:** 5 tasks
- **Phase 1:** 10 tasks
- **Phase 2:** 10 tasks
- **Phase 3:** 7 tasks
- **Phase 4:** 8 tasks
- **Phase 5:** 7 tasks
- **Phase 6:** 5 tasks

**Total: 52 granular tasks**

Each task should take 1-4 hours to complete.

---

## How to Use This

1. Pick a phase to focus on
2. Work through tasks in order
3. Check off each task as complete
4. When phase is done, move to next
5. Update TODO_TRACKER.md with progress
6. Commit when phase is complete

**Ready to start Phase 0 research?**
