# Implementation Plan: Narrative Game Engine

**Based on:** Phase 0 Complete Research + Design Philosophy + Narrator Architecture Clarification  
**Workflow:** Plan → Build → Commit → Next Goal  
**Target:** Minimum Viable Narrative Game in DCSS

---

## 🎯 Project Vision

Build a text-based narrative game powered by DCSS where:
- Player interacts via **prose descriptions only** (god-like narrator)
- System handles **consequences** (DCSS simulation engine)
- Database stores **complete history and evidence** (single source of truth)
- Narrator generates **interpretation only** (prose descriptions)

**Core Philosophy:** User has authority over intent. DCSS has authority over consequences. Database has authority over evidence. Narrator has authority over interpretation.

See: `NARRATOR_WORLD_HANDSHAKE.md` for complete architecture and principles.

**Scope:** Start with basic event database + prose generation layer on top of DCSS; grow to weather systems, trade, factions, politics

---

## 📋 Phase Breakdown (High-Level)

| Phase | Duration | Goal | Commits |
|-------|----------|------|---------|
| **Phase 1** | 8-12 hrs | DCSS Integration Layer | 7 |
| **Phase 2** | 6-12 hrs | Narrator Prototype | 8-10 |
| **Phase 3** | 4-6 hrs | Custom Game Mode | 2-4 |
| **Phase 4** | 8-12 hrs | Database Layer | 4-6 |
| **Phase 5** | 12-20 hrs | Extended Gameplay | 8-12 |
| **Phase 6** | 6-10 hrs | Polish & Distribution | 4-8 |
| **TOTAL** | 44-72 hrs | Complete MVP | 33-51 |

---

## 🔧 Phase 1: DCSS Integration Layer (4-8 Hours)

**Goal:** Build wrapper layer that lets code control DCSS and read state

**Architecture: The Narrator Pipeline**
```
Player Prose
    ↓
[ACTION INTERPRETER] ← queries world state
    ↓
    → Understands player intent
    → Identifies related game world objects/locations
    ↓
[CLARIFICATION LOGIC]
    ↓
    → If ambiguous: ask player (e.g., "Which goblin?")
    → If incomplete: ask player (e.g., "Which direction?")
    ↓
[COMMAND COMPILER]
    ↓
    → Converts intent to discrete DCSS commands
    → Examples: "go north" → [CMD_MOVE_UP]
    → Examples: "attack the goblin" → [CMD_MOVE_TO_GOBLIN, CMD_MELEE]
    ↓
[GAME EXECUTOR]
    ↓
    → Executes each command via process_command()
    → Reads world state after each command
    → Returns action results
    ↓
Game State (Updated)
```

### Phase 1 Tasks (Feature→Commit Workflow)

#### Task 1.1: Character Creation Wrapper
**Time:** 1 hour  
**What:** Create function to programmatically create DCSS characters

```
Commit: "feat(phase-1.1): Add narrator_create_character() wrapper

- Add narrator_control.h with character creation function
- Implement setup_game(newgame_def) wrapper
- Support species/job selection from code
- Verify character initializes in world
- Test with multiple species/job combos"
```

**Checklist:**
- [ ] Create `narrator_control.h` header
- [ ] Implement `narrator_create_character(name, species, job)`
- [ ] Wrap `setup_game()` from ng-setup.cc
- [ ] Handle newgame_def struct setup
- [ ] Return bool success/failure
- [ ] Test character appears in world
- [ ] Commit with git

---

#### Task 1.2: World State Query System
**Time:** 2 hours  
**What:** Read all game world state without consuming turns

```
Commit: "feat(phase-1.2): Add narrator_get_world_state() queries

- Add narrator_world.h for state queries
- Read player: position, HP, MP, inventory, effects
- Read environment: tiles, creatures, items
- Create comprehensive world state snapshot
- Add JSON serialization of world
- Test query accuracy across multiple turns"
```

**Checklist:**
- [ ] Create `narrator_world.h` header
- [ ] Implement player queries (position, HP, MP, status)
- [ ] Implement tile/terrain queries
- [ ] Implement creature queries (monsters, positions, types)
- [ ] Implement item queries (location, type, name)
- [ ] Create `narrator_world_state` struct
- [ ] Add JSON export function
- [ ] Test state accuracy
- [ ] Commit with git

---

#### Task 1.3: Action Interpreter
**Time:** 2 hours  
**What:** Parse prose actions against world state

```
Commit: "feat(phase-1.3): Add narrator_interpret_action() parser

- Add narrator_interpreter.h
- Parse prose input: 'attack the goblin', 'go north'
- Query world state to find target entities
- Identify ambiguities ('which goblin?' if 2+ present)
- Generate clarification questions as needed
- Return action intent + any required clarifications"
```

**Checklist:**
- [ ] Create `narrator_interpreter.h` header
- [ ] Implement motion verb parsing (go, walk, move, travel)
- [ ] Implement action verb parsing (attack, cast, take, use)
- [ ] Implement entity matching against world state
- [ ] Add ambiguity detection
- [ ] Create clarification question generator
- [ ] Test with 15+ action examples
- [ ] Commit with git

---

#### Task 1.4: Command Compilation  
**Time:** 1-2 hours  
**What:** Convert action intent to discrete DCSS commands

```
Commit: "feat(phase-1.4): Add narrator_compile_commands() dispatcher

- Add narrator_commands.h
- Map action intent to command sequences
- Example: 'go north' → [CMD_MOVE_UP]
- Example: 'attack goblin at 5,3' → [CMD_MOVE_TO, CMD_MELEE]
- Support: movement, combat, spellcasting, items
- Test command accuracy"
```

**Checklist:**
- [ ] Create `narrator_commands.h` header
- [ ] Map 30+ action types to command sequences
- [ ] Implement pathfinding for movement
- [ ] Implement target selection for combat
- [ ] Implement spell/item querying
- [ ] Test command generation
- [ ] Verify command sequences
- [ ] Commit with git

---

#### Task 1.5: Game Execution Engine
**Time:** 1 hour  
**What:** Execute commands and return results

```
Commit: "feat(phase-1.5): Add narrator_execute_action() runner

- Add narrator_executor.h
- Execute command sequence via process_command()
- Track turn consumption
- Capture action results
- Read world state before/after
- Return action outcome + new state"
```

**Checklist:**
- [ ] Create `narrator_executor.h` header
- [ ] Implement command execution loop
- [ ] Detect turn consumption via you.turn_is_over
- [ ] Capture action success/failure
- [ ] Handle action interruptions
- [ ] Read pre/post state
- [ ] Compile action result
- [ ] Commit with git

---

#### Task 1.6: Complete Pipeline Integration
**Time:** 1 hour  
**What:** Chain all components into one narrator loop

```
Commit: "feat(phase-1.6): Add narrator_process_action() full pipeline

- Create narrator_pipeline.h
- Chain: interpret → compile → execute → report
- Prose → Clarifications → Commands → Results
- Handle error cases and invalid actions
- Test full 5-turn manual scenario"
```

**Checklist:**
- [ ] Create `narrator_pipeline.h` header
- [ ] Implement action processing pipeline
- [ ] Handle player clarifications
- [ ] Handle execution errors
- [ ] Generate result narration
- [ ] Test complete flow
- [ ] Commit with git

---

#### Task 1.7: Integration Tests
**Time:** 1-2 hours  
**What:** Verify complete integration works

```
Commit: "test(phase-1.7): Add narrator integration tests

- Create narrator_integration_test.cc
- Test character creation
- Test world state queries
- Test action interpretation
- Test complete prose→result pipeline
- Test 10-turn game loop
- Verify no regressions"
```

**Checklist:**
- [ ] Create test file
- [ ] Test 1: Character creation
- [ ] Test 2: State queries
- [ ] Test 3: Action interpretation
- [ ] Test 4: Command compilation
- [ ] Test 5: Complete 10-turn loop
- [ ] Run all tests
- [ ] Commit with git

---

## 🎭 Phase 2: Narrator Prototype (6-12 Hours)

**Goal:** Simple AI-mediated loop: state → prompt → action → result

### Phase 2 Tasks

#### Task 2.1: Prompt Generator
**Time:** 2 hours  
**What:** Convert game state to prose description

```
Commit: "feat: Add narrator_generate_prompt() for game description

- Read game state
- Generate prose: location, visible enemies, player status
- Format: 'You are in a dungeon. You see a goblin. You have 20 HP.'
- Support multiple location types
- Test with various game states"
```

**Checklist:**
- [ ] Create `narrator_prompt.h`
- [ ] Implement location description generator
- [ ] Implement enemy description generator
- [ ] Implement status description generator
- [ ] Create main `narrator_generate_prompt()` function
- [ ] Test with 5 different game states
- [ ] Verify prose is readable
- [ ] Commit with git

---

#### Task 2.2: Action Parser
**Time:** 2 hours  
**What:** Convert player prose to game commands

```
Commit: "feat: Add narrator_parse_action() to convert prose to commands

- Parse: 'move north' → CMD_MOVE_UP
- Parse: 'cast fireball' → CMD_CAST_SPELL + spell selection
- Parse: 'pick up the sword' → CMD_PICKUP
- Handle variations: 'go up', 'head north', 'move towards the goblin'
- Return list of commands or clarifying questions"
```

**Checklist:**
- [ ] Create `narrator_parser.h`
- [ ] Implement movement parsing
- [ ] Implement action parsing (cast, use, etc)
- [ ] Implement item parsing
- [ ] Create `narrator_parse_action()` function
- [ ] Test with 20 different input phrases
- [ ] Verify command accuracy
- [ ] Commit with git

---

#### Task 2.3: Complete Loop
**Time:** 2 hours  
**What:** Chain everything together

```
Commit: "feat: Add narrator_game_loop() - complete turn cycle

- Generate prompt from state
- Get action from hardcoded/test input
- Execute action via narrator_execute_command()
- Get new state
- Generate result narration
- Ready for next turn
- Test manual 5-turn scenario"
```

**Checklist:**
- [ ] Create main loop function
- [ ] Generate initial prompt
- [ ] Accept action input
- [ ] Execute command
- [ ] Generate result description
- [ ] Handle turn consumption
- [ ] Handle character death
- [ ] Test 5-turn manual scenario
- [ ] Commit with git

---

## 🎮 Phase 3: Custom Game Mode (4-6 Hours)

**Goal:** Create DCSS game type optimized for narrative play

### Phase 3 Tasks

#### Task 3.1: New Game Type
**Time:** 2 hours  
**What:** Add GAME_TYPE_NARRATIVE to DCSS

```
Commit: "feat: Add GAME_TYPE_NARRATIVE to DCSS

- Add to game-type.h enum
- Add initialization in ng-setup.cc
- Set difficulty/restrictions for narrative play
- Disable real-time pressure mechanics
- Test game boots with new type"
```

**Checklist:**
- [ ] Add GAME_TYPE_NARRATIVE enum
- [ ] Add to game_type_name() function
- [ ] Add initialization in setup_game()
- [ ] Disable turn speed mechanics
- [ ] Set starting difficulty
- [ ] Test character creation with new type
- [ ] Commit with git

---

#### Task 3.2: Balance Tuning
**Time:** 2 hours  
**What:** Tune DCSS for narrative gameplay

```
Commit: "feat: Balance DCSS for narrative gameplay

- Reduce monster damage scaling
- Increase player starting HP
- Adjust spell mana costs
- Reduce poison/duration effect severity
- Make encounters more tactical, less brutal
- Test difficulty feels right"
```

**Checklist:**
- [ ] Reduce monster damage by 20%
- [ ] Increase player starting HP
- [ ] Adjust mana costs
- [ ] Test a few encounters manually
- [ ] Verify not too easy, not too hard
- [ ] Commit with git

---

#### Task 3.3: Opening Scenario
**Time:** 2 hours  
**What:** Create first dungeon encounter

```
Commit: "feat: Create opening narrative scenario

- Create D:1 encounter with single goblin
- Write opening narration
- Set starting equipment/spells
- Test full flow: create→prompt→fight→win
- Verify Narrator handles complete scenario"
```

**Checklist:**
- [ ] Create simple opening encounter
- [ ] Write opening narration
- [ ] Set starting gear
- [ ] Test character vs goblin combat
- [ ] Verify victory detection
- [ ] Test full narrative flow
- [ ] Commit with git

---

## 📊 Phase 4: Database Layer (8-12 Hours)

**Goal:** Store and manage game world content

### Phase 4 Tasks

#### Task 4.1: Database Schema
**Time:** 2 hours  
**What:** Design SQLite database structure

```
Commit: "feat: Create narrative game database schema

- Table: npcs (id, name, description, dialogue)
- Table: locations (id, name, description, encounters)
- Table: items (id, name, description, lore)
- Table: quests (id, name, objectives, rewards)
- Table: factions (id, name, description)
- Create migrations/schema.sql"
```

**Checklist:**
- [ ] Design NPC table structure
- [ ] Design location table structure
- [ ] Design item table structure
- [ ] Design quest table structure
- [ ] Design faction table structure
- [ ] Create migrations/setup script
- [ ] Initialize test database
- [ ] Commit with git

---

#### Task 4.2: Database Interface
**Time:** 2-3 hours  
**What:** Create query functions

```
Commit: "feat: Add database query interface

- Add narrator_db.h with query functions
- Implement get_npc_dialogue(npc_id)
- Implement get_location_description(loc_id)
- Implement get_item_description(item_id)
- Implement get_quest_objectives(quest_id)
- Test all queries"
```

**Checklist:**
- [ ] Create narrator_db.h
- [ ] Implement SQLite connection
- [ ] Implement query functions
- [ ] Add error handling
- [ ] Test with sample data
- [ ] Verify performance
- [ ] Commit with git

---

#### Task 4.3: Narrator Integration
**Time:** 2-3 hours  
**What:** Use database for narration

```
Commit: "feat: Integrate database with Narrator

- Narrator queries database for descriptions
- NPC dialogue comes from database
- Location descriptions from database
- Item descriptions personalized
- Test full flow with database content"
```

**Checklist:**
- [ ] Modify prompt generator to query DB
- [ ] Add NPC dialogue queries
- [ ] Add location descriptions
- [ ] Add item descriptions
- [ ] Test narration with DB content
- [ ] Verify accuracy
- [ ] Commit with git

---

#### Task 4.4: Content Authoring
**Time:** 2 hours  
**What:** Make it easy to add content

```
Commit: "feat: Add content authoring helpers

- Create JSON import/export for database content
- Create tools to add NPCs, locations, items
- Document content format
- Test importing sample content"
```

**Checklist:**
- [ ] Create JSON content format
- [ ] Implement JSON→database import
- [ ] Create sample content file
- [ ] Test import/export
- [ ] Document format
- [ ] Commit with git

---

## 🌍 Phase 5: Extended Gameplay (12-20 Hours)

**Goal:** Add depth: NPCs, quests, factions, consequences

### Phase 5 Tasks (8-12 commits)

- Task 5.1: NPC Dialogue System (2-3 hours)
- Task 5.2: Quest Tracking (2-3 hours)
- Task 5.3: Faction System (2-3 hours)
- Task 5.4: Custom Spells/Items (2-3 hours)
- Task 5.5: World State Persistence (2-3 hours)

---

## ✨ Phase 6: Polish & Distribution (6-10 Hours)

**Goal:** Clean up, document, package for release

### Phase 6 Tasks (4-8 commits)

- Task 6.1: Save/Load System (2-3 hours)
- Task 6.2: Mod Support (2-3 hours)
- Task 6.3: Documentation (1-2 hours)
- Task 6.4: Release Build (1-2 hours)

---

## 📝 Execution Strategy

### For Each Task:
1. **Read this plan** - Understand what needs to be built
2. **Create feature branch** - `git checkout -b feature/task-name`
3. **Implement feature** - Code, test, verify
4. **Run full test** - Verify nothing broke
5. **Commit with message** - Include task number and summary
6. **Merge to main** - `git merge feature/task-name`
7. **Update status** - Mark task done in todo list

### Commit Message Format:
```
[PHASE-TASK]: Descriptive title

- What was added
- Why it matters
- Key changes
- Test results

Closes: Phase 1.1
```

---

## 🎯 Success Criteria

### Phase 1: Integration Complete
- [ ] Can create character from code ✓
- [ ] Can read all game state ✓
- [ ] Can execute commands ✓
- [ ] Can run 10-turn loop ✓

### Phase 2: Prototype Works
- [ ] Prompt generation is readable ✓
- [ ] Action parsing works ✓
- [ ] Full loop is executable ✓
- [ ] 5-turn scenario runs perfectly ✓

### Phase 3: Game Mode Ready
- [ ] New game type boots ✓
- [ ] Difficulty feels right ✓
- [ ] Opening scenario plays well ✓

### Phase 4: Database Works
- [ ] Can query all content types ✓
- [ ] Narration uses DB content ✓
- [ ] Content is easy to add ✓

### Phase 5: Full Gameplay
- [ ] NPCs have personality ✓
- [ ] Quests are trackable ✓
- [ ] Choices have consequences ✓

### Phase 6: Release Ready
- [ ] Game saves/loads ✓
- [ ] Can create mods ✓
- [ ] Documentation complete ✓

---

## 📚 Files to Create

### Phase 1
- `narrator_control.h/.cc` - Character creation
- `narrator_state.h/.cc` - State queries
- `narrator_commands.h/.cc` - Command dispatch
- `narrator_test.cc` - Tests

### Phase 2
- `narrator_prompt.h/.cc` - Prose generation
- `narrator_parser.h/.cc` - Action parsing
- `narrator_loop.h/.cc` - Main loop

### Phase 3
- Modified `ng-setup.cc` - New game type
- Modified balance constants
- Encounter definition files

### Phase 4
- `narrator_db.h/.cc` - Database interface
- `migrations/schema.sql` - Database schema
- `narrator_content.h/.cc` - Content authoring

### Phase 5+
- NPC system files
- Quest system files
- Faction system files
- Custom content files

---

## ⏱️ Time Budget

| Phase | Min | Max | Realistic |
|-------|-----|-----|-----------|
| 1 | 4h | 8h | 6h |
| 2 | 6h | 12h | 9h |
| 3 | 4h | 6h | 5h |
| 4 | 8h | 12h | 10h |
| 5 | 12h | 20h | 16h |
| 6 | 6h | 10h | 8h |
| **Total** | **40h** | **68h** | **54h** |

*Realistic estimate: 1.5-2 weeks at 5-6 hours per day*

---

## 🔄 How to Use This Plan

1. **Print or bookmark this page**
2. **Start Phase 1, Task 1.1** - Read instructions
3. **Create feature branch** for the task
4. **Follow checklist** to completion
5. **Commit to git** with phase/task reference
6. **Move to next task** when current is done
7. **Update progress** in PROJECT_STATUS.md

---

## 📞 Questions/Decisions Needed

Before starting Phase 1.1:
1. Git workflow - Should I create feature branches? ✓ (Yes, per design philosophy)
2. Testing - Should tests be in separate files or inline? → Decide when we hit 1.5
3. Narrator AI - Hardcoded logic or external API? → Decide before Phase 2

---

**Created:** 2026-04-16  
**Status:** Ready for Phase 1 Implementation  
**Next:** Begin Phase 1.1: Character Creation Wrapper
