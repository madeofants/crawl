# Start a New Coding Session

Use this checklist to get up to speed when starting a new session.

## 1. Orient Yourself (2 mins)
- Source code: `C:\GameDemo\crawl\crawl-ref\source\`
- Reference docs: `C:\GameDemo\crawl\REFERENCE\`
- Read: `PROJECT_STATUS.md` to understand where we are

## 2. Refresh Your Memory (5 mins)
Open `WORKING_REFERENCE.md` and review:
- Character Creation Flow (top section)
- Key File Locations table
- Common Changes examples

## 3. Pick Your Task (var)
Examples:
- Add a new species
- Modify job starting stats
- Change starting equipment
- Add a new game mode
- Create a restriction

## 4. Find the Code
Use the "Key File Locations" table in `WORKING_REFERENCE.md` to know which file to edit.

Common locations:
```
Species stuff:      C:\GameDemo\crawl\crawl-ref\source\species.cc
Job stuff:          C:\GameDemo\crawl\crawl-ref\source\jobs.cc
Restrictions:       C:\GameDemo\crawl\crawl-ref\source\ng-restr.cc
Game modes:         C:\GameDemo\crawl\crawl-ref\source\game-type.h
Menu logic:         C:\GameDemo\crawl\crawl-ref\source\newgame.cc
Game init:          C:\GameDemo\crawl\crawl-ref\source\ng-setup.cc
```

## 5. Make Your Change
Edit the file. Use `MODDERS_QUICK_START.md` for how-to examples if needed.

## 6. Build
```bash
cd C:\GameDemo\crawl\crawl-ref\source
make
```

If errors: Check the error message, look at the file, try again.

## 7. Test
Run the game and verify your change works.

## 8. Document What You Did
Update `PROJECT_STATUS.md` "Next Steps" section or add notes to `WORKING_REFERENCE.md` if you find new info.

---

## Quick Facts to Remember
- `make` automatically runs code generators (Python scripts)
- Generated files: `species-groups.h`, `job-groups.h` (don't edit)
- Enum to know: `CC_UNRESTRICTED` (restriction level)
- Key function: `_setup_generic()` in ng-setup.cc (game initialization)
- Build location: `crawl-ref/source/`

## If You're Stuck
1. Check `WORKING_REFERENCE.md` again
2. Look at `CORRECTED_REFERENCE.md` for more detail
3. Check `MODDERS_QUICK_START.md` for examples
4. Audit reports have info about the system structure

## Next Session Template
```
Session Date: ____
Focus: ____
Files Modified: ____
Changes Made: ____
Build Status: ____
Notes: ____
```
