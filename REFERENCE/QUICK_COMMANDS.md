# Quick Commands Reference

Copy/paste commands for common tasks.

## Build
```bash
cd C:\GameDemo\crawl\crawl-ref\source
make
```

## Clean Build
```bash
cd C:\GameDemo\crawl\crawl-ref\source
make clean
make
```

## Find a Function
```bash
cd C:\GameDemo\crawl\crawl-ref\source
findstr /R "_choose_species" *.cc
```

## View a File
```bash
type C:\GameDemo\crawl\crawl-ref\source\newgame.cc
```

## Quick View of Source Structure
```bash
dir C:\GameDemo\crawl\crawl-ref\source\ | findstr "\.cc$"
```

## Find References to an Enum
```bash
findstr /R "JOB_FIGHTER" C:\GameDemo\crawl\crawl-ref\source\*.cc
```

## Navigate to Source
```bash
cd C:\GameDemo\crawl\crawl-ref\source
```

## Navigate to Reference
```bash
cd C:\GameDemo\crawl\REFERENCE
```

## Open Working Reference
```bash
start C:\GameDemo\crawl\REFERENCE\WORKING_REFERENCE.md
```

## Open Source Directory in Explorer
```bash
explorer C:\GameDemo\crawl\crawl-ref\source\
```

---

## Git Commands (if needed)

### Check Status
```bash
cd C:\GameDemo\crawl
git status
```

### View Recent Commits
```bash
cd C:\GameDemo\crawl
git log --oneline -10
```

### Commit Changes
```bash
cd C:\GameDemo\crawl
git add .
git commit -m "Your message here"
```

### View Diff
```bash
cd C:\GameDemo\crawl
git diff
```

---

## Searching Files

### Find all species definitions
```bash
findstr /n "^species_type" C:\GameDemo\crawl\crawl-ref\source\species.cc
```

### Find all job definitions
```bash
findstr /n "^job_type" C:\GameDemo\crawl\crawl-ref\source\jobs.cc
```

### Find function definitions
```bash
findstr /n "^[a-z_]*(.*)$" C:\GameDemo\crawl\crawl-ref\source\newgame.cc
```

---

## PowerShell Quick File Search
```powershell
# Search in all .cc files for a pattern
Get-ChildItem "C:\GameDemo\crawl\crawl-ref\source\*.cc" | 
Select-String "pattern_here" | 
Select-Object Filename, LineNumber, Line
```

---

## Visual Studio Code Quick Open
If you have VS Code:
```bash
code C:\GameDemo\crawl\crawl-ref\source
```

Then use Ctrl+P to search for files, Ctrl+F to search text.
