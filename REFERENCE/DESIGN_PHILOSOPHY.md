# Crawl Mod Project - Design Philosophy

## What We're Building

**A text-based narrative game engine powered by AI.**

DCSS runs the simulation underneath. The player never sees the traditional roguelike UI. Instead:

1. **Narrator (AI Agent)** reads the game world state
2. **Narrator prompts the player** with prose describing the situation
3. **Player responds with prose actions** ("I walk north and search for treasure")
4. **Narrator clarifies** if needed ("Do you want to search cautiously or quickly?")
5. **Prose → Game Actions** - Narrator converts player intent to DCSS commands
6. **Game executes** - DCSS runs combat, mechanics, updates world
7. **Narrator narrates results** - Describes what happened in prose

**Stats, inventory, mechanics: all under the hood. User only sees text.**

---

## The Stack

- **DCSS Engine** - Simulation, combat, mechanics, dungeon generation
- **Database** - Store game context, NPC dialogue, custom content
- **Narrator API** - AI agent that handles prose ↔ game action translation
- **Game State Bridge** - Get current world state, send commands to DCSS

---

## What This Means for Modding

We're using DCSS as an engine, not a game. Our modifications enable:
- Custom game types/modes for narrative gameplay
- Custom starting configurations
- Custom items/species/jobs for thematic reasons
- Ability to control DCSS programmatically

We're NOT making a better roguelike. We're making a narrative game that happens to use DCSS internals.

---

## Core Rules

### 1. Practicality Over Perfection
- We're modding a game as a hobby, not shipping enterprise software
- Documentation should be useful for coding, not comprehensive or polished
- Don't spend hours perfecting docs when 20 mins of working code is better
- "Good enough" that works beats "perfect" that doesn't exist

### 2. No Years of Planning
- Keep planning to hours, not months
- Make decisions quickly and adjust as we go
- If something isn't working, change it
- Heavy upfront analysis is fine, heavy upfront planning is not

### 3. Working Reference Over Perfect Reference
- Docs should be used while coding, not read once and archived
- Include exactly what we need, nothing extra
- If you find yourself saying "this doc should have...", add it
- Kill docs that aren't used; keep docs that help

### 4. Facts, Not Assumptions
- Every claim in docs must be verifiable in source code
- If we're not sure, mark it as "TODO: verify"
- Wrong info is worse than no info
- When we find errors, fix them immediately

### 5. Quick Iteration
- Make change → Build → Test → Done
- No 24-step approval processes
- No excessive code review for hobby mods
- If the build works, it works

### 6. AI + Human Partnership
- AI handles: generating docs, searching code, organizing info, routine tasks
- Human handles: deciding what to build, creative decisions, testing changes
- Collaborate, don't segregate
- AI should ask for clarity when uncertain

### 7. Modular Over Monolithic
- Each doc has one purpose (working reference, quick commands, progress tracking)
- Don't put everything in one massive file
- Easy to find what you need
- Easy to update one piece without touching others

### 8. Pragmatic File Organization
- All reference docs in one folder (`REFERENCE/`)
- Source code stays in original location (`crawl-ref/source/`)
- Session notes/progress in the project folder
- Git repo is separate; our docs aren't committed (unless we choose to)

---

## Early Goals (In Order)

1. **Understand DCSS character creation** - How to initialize a character, what we can control
2. **Run DCSS programmatically** - Execute game, read state, send commands from code
3. **Prototype Narrator loop** - AI reads state, generates prompt, player responds, execute action
4. **Custom game mode** - Create a narrative-focused game type in DCSS
5. **State management** - Track game context across turns (NPC memory, quest state, etc.)
6. **Content database** - Store narrative content, descriptions, flavors
7. **Full integration** - Complete game loop with Narrator

---

## Documentation Priority

For THIS project, priority is:
1. How to call DCSS from code (not documentation we have yet)
2. Game state access and command interface
3. What character creation controls (already documented)
4. How to add custom game types (for narrative modes)
5. Everything else

---

## Project Flow: Plan → Build → Commit → Next Goal

This is how we work on each feature/goal:

```
1. PLAN (5-30 mins)
   - Define what we're building
   - Ask AI: "Here's the goal. What's the plan?"
   - Settle on approach
   - Document in TODO_TRACKER.md or simple notes

2. BUILD (var)
   - Make the code changes
   - Test the build
   - Verify it works
   - Update docs if needed (WORKING_REFERENCE, etc.)

3. COMMIT
   - Save progress to git (or just note in TODO_TRACKER)
   - Update TODO_TRACKER with "Completed: ..."
   - Update PROJECT_STATUS.md if it's a major milestone

4. NEXT GOAL POST
   - Pick the next thing to work on
   - Start at PLAN again
   - Move forward
```

**Key:** Each cycle should be a few hours max. Complete, commit, move on.

---

## When to Do What

| Task | Approach |
|------|----------|
| Start a new goal | Plan first (5-30 mins), then build |
| Make a code change | Edit, test, it's part of build phase |
| Find a bug in docs | Fix it immediately, don't wait |
| Learn something new | Add it to WORKING_REFERENCE.md |
| Need new doc? | Create it, keep it under 5 KB unless necessary |
| Stuck on something? | Ask AI, check docs, try a different approach |
| Running low on time? | Save to git, update TODO_TRACKER, pause |

---

## Rules for Documentation

1. **Keep it short** - If a doc is over 10 KB, consider splitting it
2. **Make it actionable** - Each section should help you do something
3. **Link to source code** - Tell people where to find the actual code
4. **Use examples** - Show, don't just tell
5. **Mark unknowns** - If we're not sure about something, say so
6. **Date major changes** - Keep SESSION notes with dates

---

## Rules for Code Changes

1. **One thing at a time** - One feature/fix per change
2. **Test before considering it done** - Actually run the build and verify
3. **Document what you changed** - Add entry to TODO_TRACKER.md
4. **Keep commits clean** - If using git, one feature = one commit
5. **Don't break existing stuff** - If the build breaks, fix it

---

## Rules for AI Agent (Me)

1. **No unnecessary formality** - Skip the "mission-critical", "production-ready" language
2. **Ask when unclear** - If unsure what you want, ask instead of guessing
3. **Keep docs practical** - Always think "will this actually help while coding?"
4. **Respect the hobbyist nature** - No 9-15 hour audit projects unless explicitly asked
5. **Organize things** - Put files where they belong, clean up old planning docs
6. **Verify claims** - Every fact should trace back to source code
7. **Suggest, don't decide** - Propose options, let you choose
8. **Keep summaries short** - 50-100 words when explaining status

---

## What Success Looks Like

✅ You can make a code change in 30 mins or less with docs/tools handy  
✅ Docs are actually used while coding (not ornamental)  
✅ Build works after changes  
✅ It's fun, not a burden  
✅ New sessions can get up to speed in 5 mins  

---

## What Failure Looks Like

❌ Docs exist but aren't useful for coding  
❌ Hours spent perfecting documentation  
❌ Can't find where to make a change  
❌ Build breaks and takes hours to fix  
❌ Forget what we were doing between sessions  
❌ AI creates 20 documents when 2 would suffice  

---

## How to Change These Rules

If a rule isn't working:
1. Point it out
2. We discuss why it's not working
3. Change it or drop it
4. Update this file
5. Move on

These rules serve us, we don't serve them.

---

## Quick Summary

**This project is:**
- Practical, not perfect
- Collaborative (human + AI)
- Fast-moving, not over-planned
- Focused on working code
- Guided by facts, not assumptions
- Fun first, process second

**Work like (per goal):**
1. Plan it (quick)
2. Build it (test it)
3. Commit it
4. Next goal

**Per session:**
- Make changes
- Build and test
- Document progress
- Know what's next

**No:**
- Endless planning
- Formal processes
- Perfect documentation
- Waiting for permission
- Over-engineering

---

**TL;DR:** Make something work, document what you did, keep it practical. That's it.
