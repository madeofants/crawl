# Crawl Narrative Engine - Complete Roadmap

## Overview
A text-based narrative game where an AI Narrator mediates between player prose input and DCSS game engine simulation.

**Player sees:** Prose prompts and descriptions  
**Under the hood:** DCSS running combat, mechanics, dungeon generation  
**In between:** Narrator converting intent → actions → narration

---

## Phase 0: Foundation (Planning & Analysis)
*Goal: Understand what we're building and what's possible*

### 0.1: Finalize Game Architecture
- [ ] Define the complete flow: input → narrator → DCSS → output
- [ ] Document the interface between Narrator and DCSS
- [ ] Determine: What state needs to be accessible from DCSS?
- [ ] Determine: How do we send commands to DCSS?
- [ ] Decision: API approach? Direct code? External process?

### 0.2: Scope First Year of Gameplay
- [ ] Define what the first year of gameplay looks like
- [ ] Example: character creation → dungeon level 1-3 → simple encounters
- [ ] List required DCSS systems for minimum viable game
- [ ] List custom systems needed (NPC state, dialogue, quest tracking)

### 0.3: Research DCSS Programmatic Access
- [ ] How to call DCSS from external code?
- [ ] How to initialize a game state?
- [ ] How to read current game state?
- [ ] How to send game commands?
- [ ] What APIs or interfaces exist?

### 0.4: Define Content Model
- [ ] What goes in the database?
- [ ] Structure: NPC definitions, dialogue trees, environmental descriptions, custom items
- [ ] How does Narrator access database?
- [ ] How does Narrator update game state between turns?

### 0.5: Create Technical Reference for Integration
- [ ] Document DCSS internal architecture for programmatic access
- [ ] Document APIs/interfaces we'll use
- [ ] Document the state bridge we need to build
- [ ] Update REFERENCE folder with this info

---

## Phase 1: DCSS Integration Layer
*Goal: Be able to run DCSS from code and read/write game state*

### 1.1: Build State Reader
- [ ] Extract current player state (HP, mana, position, inventory)
- [ ] Extract dungeon state (visible tiles, monsters, items)
- [ ] Extract game state (turn count, game mode, difficulty)
- [ ] Create data structure to hold all this

### 1.2: Build Command Executor
- [ ] Send movement commands to DCSS
- [ ] Send combat commands to DCSS
- [ ] Send inventory manipulation commands
- [ ] Verify commands execute correctly

### 1.3: Build Initialization Pipeline
- [ ] Programmatically create a new game
- [ ] Programmatically create a character (using character creation system)
- [ ] Programmatically set game options
- [ ] Verify game starts and is controllable

### 1.4: Build Turn Loop
- [ ] Execute one game turn
- [ ] Read updated state
- [ ] Repeat
- [ ] Verify state updates correctly

### 1.5: Create Integration Tests
- [ ] Test character creation
- [ ] Test movement
- [ ] Test combat
- [ ] Test item pickup
- [ ] Test state consistency

---

## Phase 2: Narrator Prototype
*Goal: Simple loop of AI reading state and generating prompts*

### 2.1: Build Prompt Generator
- [ ] Read game state
- [ ] Generate English description of current situation
- [ ] Handle: location, visible enemies, items, health status
- [ ] Test with hardcoded scenarios

### 2.2: Build Action Parser
- [ ] Take player prose input
- [ ] Convert to list of possible game actions
- [ ] Return choices in machine-readable format
- [ ] Test with examples

### 2.3: Build Clarification System
- [ ] Detect ambiguous player input
- [ ] Generate clarifying questions
- [ ] Get player response
- [ ] Resolve to single action
- [ ] Test edge cases

### 2.4: Build Action Executor
- [ ] Take resolved action
- [ ] Convert to DCSS commands
- [ ] Execute via Phase 1 layer
- [ ] Return results

### 2.5: Build Result Narrator
- [ ] Read updated game state
- [ ] Generate prose description of what happened
- [ ] Handle: success/failure, damage taken, items gained, deaths
- [ ] Test with various scenarios

### 2.6: Complete Prototype Loop
- [ ] Chain 2.1-2.5 together
- [ ] Player input → Narrator → DCSS → Narrator → Output
- [ ] Test with manual scenarios
- [ ] Identify breakpoints

---

## Phase 3: Custom Game Mode
*Goal: Create a game mode optimized for narrative gameplay*

### 3.1: Design Narrative Game Mode
- [ ] Define how it differs from standard DCSS
- [ ] Simplified dungeon? Focused storyline? Modified difficulty?
- [ ] What makes it unique?
- [ ] Document the design

### 3.2: Create Game Type in DCSS
- [ ] Add new game_type enum value
- [ ] Add initialization code in ng-setup.cc
- [ ] Configure starting dungeon, difficulty, restrictions
- [ ] Build and test

### 3.3: Tune Game Balance for Narrative
- [ ] Adjust difficulty for prose-based play (not real-time)
- [ ] Simplify or remove time-pressure mechanics
- [ ] Focus on encounter design over gear progression
- [ ] Test with manual play

### 3.4: Create Starting Scenario
- [ ] Write opening narration
- [ ] Set up first encounter
- [ ] Verify Narrator can handle it
- [ ] Test full flow

---

## Phase 4: Database Layer
*Goal: Store and manage narrative content and game context*

### 4.1: Design Database Schema
- [ ] NPCs: definition, state, dialogue
- [ ] Quests: objectives, progress, rewards
- [ ] Locations: descriptions, encounters, secrets
- [ ] Items: flavor descriptions, lore
- [ ] Events: historical events, lore hooks
- [ ] Player context: memories, relationships, achievements

### 4.2: Build Database Interface
- [ ] Query NPC dialogue
- [ ] Update NPC state
- [ ] Track quest progress
- [ ] Store player choices/history
- [ ] Retrieve location descriptions
- [ ] Get item descriptions

### 4.3: Integrate Database with Narrator
- [ ] Narrator queries for NPC dialogue
- [ ] Narrator personalizes descriptions based on database
- [ ] Narrator tracks player choices
- [ ] Narrator remembers past events

### 4.4: Create Content Authoring Tools
- [ ] Easy way to write NPC dialogue
- [ ] Easy way to define quests
- [ ] Easy way to write location descriptions
- [ ] Tools for game designers (non-programmers)

---

## Phase 5: Extended Gameplay
*Goal: Expand beyond simple dungeon crawling to a full game experience*

### 5.1: NPC System
- [ ] NPCs have personalities
- [ ] NPCs remember player interactions
- [ ] NPCs can give quests
- [ ] NPCs can fight alongside player
- [ ] Dialogue branching based on player choices

### 5.2: Quest System
- [ ] Multiple quest types (kill, fetch, explore, protect)
- [ ] Quest chains and consequences
- [ ] Rewards tied to narrative impact
- [ ] Quest state persists

### 5.3: Faction System
- [ ] Reputation tracking
- [ ] Faction-specific dialogue
- [ ] Faction-specific quests
- [ ] Consequences for faction actions

### 5.4: Magic/Custom Spells
- [ ] Thematic spell names and descriptions
- [ ] Spell effects tied to narrative
- [ ] Custom spell schools for narrative game mode
- [ ] Spell learning through quests/dialogue

### 5.5: Custom Items & Artifacts
- [ ] Thematic item names and descriptions
- [ ] Unique artifacts with lore
- [ ] Item acquisition through quests
- [ ] Item descriptions in narrative

---

## Phase 6: Advanced Systems
*Goal: Polish and expand for deeper gameplay*

### 6.1: Difficulty Scaling
- [ ] Adjust combat based on player skill
- [ ] Dynamic encounter scaling
- [ ] Narrative pacing adjustment

### 6.2: World State Persistence
- [ ] Save/load game
- [ ] Remember world changes
- [ ] Track NPC and faction changes
- [ ] Maintain quest state

### 6.3: Multiple Playthroughs
- [ ] New game+ modes
- [ ] Alternate storylines
- [ ] Consequences carry between runs

### 6.4: Mod Support
- [ ] Allow custom content creation
- [ ] Custom NPC/quest definition language
- [ ] Community content integration

---

## Dependency Chart

```
Phase 0 (Planning)
    ↓
Phase 1 (DCSS Integration) ← Must complete before anything else
    ↓
Phase 2 (Narrator Prototype) ← Uses Phase 1
    ↓
Phase 3 (Custom Game Mode) ← Uses Phase 1 & 2
    ↓
Phase 4 (Database) ← Uses Phase 2 & 3
    ↓
Phase 5 (Extended Gameplay) ← Uses Phase 4
    ↓
Phase 6 (Polish & Advanced) ← Uses Phase 5
```

---

## Milestones (Commits)

| Milestone | Phase | Goal |
|-----------|-------|------|
| **0.1: Architecture Defined** | 0 | Know what we're building and how |
| **1.1: DCSS Controllable** | 1 | Can run game from code |
| **1.2: State Readable** | 1 | Can read game state |
| **2.1: Prototype Runs** | 2 | Simple Narrator loop works |
| **2.2: Full Loop** | 2 | Prose in → game action → prose out |
| **3.1: Custom Mode** | 3 | New game type boots and runs |
| **4.1: Database Works** | 4 | Can store/retrieve content |
| **4.2: Integrated** | 4 | Narrator uses database |
| **5.1: NPCs Talk** | 5 | Simple NPC dialogue works |
| **5.2: Quests Work** | 5 | Can track and complete quests |
| **6.1: World Saves** | 6 | Game state persists |
| **6.2: Mods Work** | 6 | Custom content can be created |

---

## Success Criteria by Phase

**Phase 0:** Documentation complete, team agrees on approach  
**Phase 1:** Can create character, move, read state, execute commands  
**Phase 2:** Can play simple scenario end-to-end via prose  
**Phase 3:** Custom game mode boots and runs  
**Phase 4:** Database queries work, content can be authored  
**Phase 5:** NPCs have personality, quests have consequences  
**Phase 6:** Game is saveable and replayable with mods  

---

## Time Estimates (Rough)

- Phase 0: 4-8 hours (planning, research, documentation)
- Phase 1: 20-40 hours (integration, testing)
- Phase 2: 15-25 hours (prototype, iteration)
- Phase 3: 10-15 hours (custom mode, tuning)
- Phase 4: 20-30 hours (database design, implementation, integration)
- Phase 5: 30-50 hours (features, testing, balance)
- Phase 6: 15-30 hours (polish, mods, distribution)

**Total: ~115-200 hours of work**

---

## Next Steps

1. Review this roadmap - does it match your vision?
2. Adjust phases as needed
3. Pick an end date for Phase 0 (planning)
4. Create reference documentation for Phase 1 requirements
5. Start Phase 0.1 (finalize architecture)

**Where do you want to start?**
