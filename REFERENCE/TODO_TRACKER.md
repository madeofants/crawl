# Modification Tracker

Track modifications made to the Crawl engine.

## Template for Each Change
```
### Change: [Name]
- Date: 
- File(s): 
- What Changed: 
- Lines Modified: 
- Reason: 
- Build Status: ✓ Pass / ✗ Fail
- Notes: 
```

---

## Completed Changes
(None yet - document them here as you make them)

---

## In Progress
(Track what you're working on)

---

## Planned Changes
Add your intended modifications here:

1. **Add new species**
   - File: species.cc
   - What: Add [Species Name] with specific stats
   - Status: Not started

2. **Modify job starting stats**
   - File: jobs.cc
   - What: Change [Job Name] stats
   - Status: Not started

3. **Add game mode**
   - File: game-type.h, ng-setup.cc
   - What: Add [Mode Name]
   - Status: Not started

4. **Custom restriction**
   - File: ng-restr.cc
   - What: Ban [Species]/[Job] combo
   - Status: Not started

---

## Build History
Track successful builds:

| Date | Change | Result | Notes |
|------|--------|--------|-------|
| | | | |

---

## Known Build Issues
- None currently

---

## Notes
- Remember: `make` in crawl-ref/source/ automatically generates files
- Python generators: util/species-gen.py, util/job-gen.py
- Don't edit species-groups.h or job-groups.h directly
