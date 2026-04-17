# 📚 Crawl Game Reference Library - Index

## Welcome to Your Game Engine Analysis Package

This folder contains a complete analysis of the **Crawl Roguelike's menu and character creation systems**, ready to serve as a reference for building your own game engine.

---

## 📄 Documents Overview

### 1. **ANALYSIS_SUMMARY.md** ⭐ START HERE
**Reading Time:** 5 minutes  
**Best For:** Quick overview, design principles, next steps

Contains:
- Project overview
- Core findings summary (menu system, character flow, data model)
- 5 implementation patterns
- Design principles for your game
- Analysis statistics

👉 **Start here if you have 5 minutes**

---

### 2. **QUICK_REFERENCE_CARD.md** 🎯 KEEP ON YOUR DESK
**Reading Time:** 2 minutes per section (skim-able)  
**Best For:** Quick lookups while coding, copy-paste snippets

Contains:
- 30-second character creation flow
- Key data structure (newgame_def)
- 5 core functions
- Random modes table
- Species and job lists
- Validation rules
- UI components
- Code snippets

👉 **Bookmark this for quick lookups**

---

### 3. **crawl_reference.md** 📖 COMPREHENSIVE TECHNICAL GUIDE
**Reading Time:** 30 minutes  
**Best For:** Deep understanding, implementation details, all the context

Contains:
- 12 comprehensive sections
- Menu system architecture (1.1-1.3)
- Character creation flow (2.1-2.6)
- Data structures and relationships
- Species and jobs organization
- Restriction and validation system
- Game setup process (5 stages)
- Menu options and special selections
- Seeding and game configuration
- Preference persistence
- Key files locations and purposes
- Design patterns and best practices
- Implementation notes

👉 **Read this for complete understanding**

---

### 4. **crawl_architecture_diagrams.md** 🎨 VISUAL REFERENCE
**Reading Time:** 15 minutes (browse diagrams)  
**Best For:** Understanding flows, architecture relationships, visual learners

Contains:
- 10 ASCII flowcharts and diagrams:
  1. Character creation call flow
  2. Menu system architecture (class hierarchy)
  3. Character selection process (filtering logic)
  4. Game definition data model
  5. Species & job grouping structure
  6. Restriction validation flow
  7. Seed & pregeneration options UI
  8. Game setup pipeline
  9. Menu letter assignment pattern
  10. Platform-specific rendering

👉 **Print or reference for visual understanding**

---

## 🗺️ Quick Navigation Guide

### By Time Available:

**I have 2 minutes:**
→ Read QUICK_REFERENCE_CARD.md (sections 1-3)

**I have 5 minutes:**
→ Read ANALYSIS_SUMMARY.md

**I have 15 minutes:**
→ Browse crawl_architecture_diagrams.md (skim diagrams 1-3)

**I have 30 minutes:**
→ Read crawl_reference.md sections 1-5

**I have 1-2 hours:**
→ Read everything, take notes

---

### By Development Phase:

**PLANNING PHASE:**
- Read: ANALYSIS_SUMMARY.md
- Reference: crawl_architecture_diagrams.md (Diagrams 1-3, 4, 5)
- Bookmark: QUICK_REFERENCE_CARD.md

**DESIGN PHASE:**
- Deep read: crawl_reference.md sections 1-6
- Study: crawl_architecture_diagrams.md (all 10)
- Reference: QUICK_REFERENCE_CARD.md (patterns)

**IMPLEMENTATION PHASE:**
- Keep open: QUICK_REFERENCE_CARD.md
- Reference: crawl_reference.md (specific sections as needed)
- Consult: Snippets at bottom of reference for copy-paste code patterns

**OPTIMIZATION PHASE:**
- Review: Design patterns (crawl_reference.md section 11)
- Check: Implementation notes (crawl_reference.md section 12)

---

### By Task:

**"How does character creation work?"**
→ crawl_architecture_diagrams.md Diagram 1 + crawl_reference.md Section 2

**"What is newgame_def?"**
→ crawl_architecture_diagrams.md Diagram 4 + QUICK_REFERENCE_CARD.md

**"How do I implement the menu system?"**
→ crawl_reference.md Section 1 + crawl_architecture_diagrams.md Diagram 2

**"What are the species and jobs?"**
→ QUICK_REFERENCE_CARD.md (species/jobs sections) + crawl_architecture_diagrams.md Diagram 5

**"How do restrictions work?"**
→ crawl_reference.md Section 4 + crawl_architecture_diagrams.md Diagram 6

**"How does game setup happen?"**
→ crawl_reference.md Section 5 + crawl_architecture_diagrams.md Diagram 8

**"What code patterns should I use?"**
→ crawl_reference.md Section 11 + QUICK_REFERENCE_CARD.md (patterns section)

**"I need a quick code snippet"**
→ QUICK_REFERENCE_CARD.md (bottom section)

---

## 📊 Document Statistics

| Document | Lines | Size | Sections |
|----------|-------|------|----------|
| ANALYSIS_SUMMARY.md | 300+ | 8KB | 13 |
| QUICK_REFERENCE_CARD.md | 320+ | 8KB | 18 |
| crawl_reference.md | 550+ | 16KB | 12 |
| crawl_architecture_diagrams.md | 680+ | 21KB | 10 diagrams |
| **TOTAL** | **~1850** | **~53KB** | **53 sections** |

---

## 🎯 Key Concepts Summary

### The Five Pillars of Crawl's Character Creation

1. **Data (newgame_def)**
   - Single struct holds all configuration
   - Serializable for persistence
   - Supports randomization flags

2. **Filtering (Cascading)**
   - Species menu filtered by job choice
   - Job menu filtered by species choice
   - Prevents invalid combos at UI level

3. **Validation (Centralized)**
   - job_allowed() function checks compatibility
   - Restriction levels: CC_BANNED, CC_RESTRICTED, CC_ALLOWED
   - Game-type-specific rules (sprint, tutorial, etc.)

4. **UI Framework (Abstract)**
   - Widget-based with platform-specific rendering
   - MenuButton, OuterMenu, Menu components
   - Auto-assigned keyboard shortcuts

5. **Setup Pipeline (Orchestrated)**
   - Player initialization → Items → Stats → Dungeon
   - Clear separation of concerns
   - Extensible for additional setup stages

---

## 🔍 Cross-Reference Index

### By File Examined:

| Original File | Best Doc | Sections |
|---|---|---|
| newgame.cc | crawl_reference.md | 2, 6-9 |
| newgame.h | crawl_reference.md | 2-4 |
| newgame-def.h | crawl_reference.md | 3; QUICK_REF |
| ng-setup.cc | crawl_reference.md | 5 |
| menu.h | crawl_reference.md | 1; Diagrams |
| outer-menu.h | crawl_reference.md | 1; Diagrams |
| ng-restr.cc | crawl_reference.md | 2.4 |
| ng-init.cc | crawl_reference.md | 5.2 |

### By Topic:

| Topic | Doc | Location |
|---|---|---|
| Menu Architecture | crawl_reference.md | 1.1-1.3 |
| UI Components | crawl_architecture_diagrams.md | Diagram 2 |
| Character Flow | crawl_architecture_diagrams.md | Diagram 1 |
| Data Structure | crawl_architecture_diagrams.md | Diagram 4 |
| Species/Jobs | crawl_architecture_diagrams.md | Diagram 5 |
| Validation | crawl_architecture_diagrams.md | Diagram 6 |
| Game Setup | crawl_architecture_diagrams.md | Diagram 8 |
| Code Patterns | crawl_reference.md | 11 |
| Implementation Tips | crawl_reference.md | 12 |

---

## 💡 Implementation Strategy

### Step 1: Understand (Week 1)
1. Read ANALYSIS_SUMMARY.md (5 min)
2. Skim crawl_architecture_diagrams.md (15 min)
3. Deep read crawl_reference.md sections 1-4 (30 min)

### Step 2: Design (Week 1-2)
1. Define your species/job system
2. Create data structure similar to newgame_def
3. Sketch UI mockups
4. Map out restriction rules

### Step 3: Build Foundation (Week 2-3)
1. Create game_def struct (data layer)
2. Build widget/button framework (UI layer)
3. Implement validation function (logic layer)
4. Wire together orchestrator function

### Step 4: Implement Features (Week 3-4)
1. Species/job selection menus
2. Name input screen
3. Starting items/skills
4. Preference saving
5. Random generation modes

### Step 5: Polish (Week 4-5)
1. Add all UI polish (colors, alignment, focus)
2. Platform-specific rendering (SDL, Web, Console)
3. Keyboard shortcuts and hotkeys
4. Help system and tooltips

---

## 🎓 Learning Outcomes

After studying these documents, you'll understand:

✅ How to organize complex choices (species, jobs) into semantic groups  
✅ How to filter options dynamically based on previous selections  
✅ How to validate combinations against restriction rules  
✅ How to build a widget-based UI framework  
✅ How to support multiple platforms (SDL, Web, ASCII)  
✅ How to structure game configuration as persistent data  
✅ How to implement cascading menu systems  
✅ How to generate starting equipment and stats  
✅ How to manage game modes (normal, sprint, tutorial, custom seed)  
✅ How to support randomization with multiple difficulty levels  

---

## 📝 Notes & Tips

**Pro Tips:**
- Keep QUICK_REFERENCE_CARD.md open while coding
- Print Diagram 1 (character creation flow) and put on your desk
- Use the code snippets at the bottom of QUICK_REFERENCE_CARD.md as templates
- Cross-reference crawl_reference.md sections when you hit implementation questions
- Study Section 11 (Design Patterns) before you start coding

**Common Mistakes to Avoid:**
- Don't make restriction checking happen only at confirmation (filter at display time)
- Don't hardcode species/jobs - use groups for flexibility
- Don't separate data layer from presentation too much (simple is better)
- Don't forget preference persistence (users want remembered choices)
- Don't skip validation (validate at UI level AND at setup time)

---

## 🔗 External References

**Original Repository:**
- https://github.com/crawl/crawl

**Your Fork:**
- https://github.com/madeofants/crawl

**Local Clone:**
- C:\GameDemo\crawl

**Session Workspace:**
- C:\Users\Danie\.copilot\session-state\d1796052-b428-47d4-a5ef-f6df48067d65\

---

## 📞 Document Maintenance

**Last Updated:** 2026-04-16  
**Analysis Scope:** Menu system & character creation subsystems  
**Code Quality:** ~2000 lines of production code analyzed  
**Coverage:** Complete flow from character selection through game initialization  

**Future Updates Needed:**
- Add more examples after you build your own system
- Document your custom patterns and deviations
- Record lessons learned during implementation

---

## 🚀 Let's Build!

You now have everything you need to understand how Crawl handles character creation and menus. These documents represent:

- **53+ reference sections** organized by topic
- **10 architecture diagrams** showing flows and relationships
- **5+ design patterns** ready to use
- **Multiple entry points** based on how much time you have

Choose a document above and get started. Happy coding! 🎮

---

**Next Action:** Open ANALYSIS_SUMMARY.md or QUICK_REFERENCE_CARD.md →
