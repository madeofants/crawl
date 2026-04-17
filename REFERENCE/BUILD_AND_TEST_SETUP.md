# Build & Test Setup

## Pre-Phase-0: Get DCSS Compiling

### Build Step 1: Verify Prerequisites
```bash
# Check you have the build tools
gcc --version
make --version
python --version  # Python 3.x needed for generators
```

### Build Step 2: Navigate to Source
```bash
cd C:\GameDemo\crawl\crawl-ref\source
```

### Build Step 3: Initial Compile
```bash
make
```

**If it fails:**
- [ ] Note the error message
- [ ] Search for that error in DCSS docs/forums
- [ ] Check Windows-specific build notes
- [ ] Try: `make clean && make`
- [ ] Check if you need to install additional packages

**If it succeeds:**
- [ ] A binary should be created (probably `crawl.exe` or similar)
- [ ] Note the location of the binary
- [ ] Test it runs: `./crawl` or `.\crawl.exe`

### Build Step 4: Document Build Output Location
Once compiling works, note:
```
Binary Location: ____________
Build Time: ____________
Build Command: make
Clean Build Command: make clean && make
```

---

## Testing Workflow

### The Problem
- Compile takes 3-5 minutes
- Small changes mean waiting 5+ mins to test
- Need faster feedback loop

### The Solution: Test Harness

We'll create a small C++ program that:
1. Calls DCSS functions directly (no full game startup)
2. Tests specific functionality fast
3. Requires only 30 seconds to compile

### Quick Test Loop (Target: 1 min per test)

```
Edit source file (30 secs)
  ↓
Run test harness (30 secs compile + run)
  ↓
See results (instant)
  ↓
Iterate
```

vs.

```
Edit source file (30 secs)
  ↓
Full build (5 mins)
  ↓
Launch game (30 secs)
  ↓
Manually test (2-10 mins)
  ↓
See results (after 10+ mins)
```

---

## Phase 0.0: Build Infrastructure

### 0.0.1: Compile First Build
- [ ] Navigate to crawl-ref/source
- [ ] Run `make clean`
- [ ] Run `make` and wait
- [ ] Verify executable is created
- [ ] Document location and any errors

### 0.0.2: Test Executable
- [ ] Run the executable
- [ ] Go through character creation
- [ ] Move around for a few turns
- [ ] Verify game works
- [ ] Exit cleanly

### 0.0.3: Create Test Harness Template
- [ ] Create file: `crawl-ref/source/test_harness.cc`
- [ ] Add includes for player.h, dungeon.h, etc.
- [ ] Create main() function that calls DCSS functions
- [ ] Add to Makefile so it compiles when you run `make test`
- [ ] Verify it compiles

### 0.0.4: First Test Harness - Read Player HP
```cpp
// test_harness.cc
#include "player.h"
#include <iostream>

int main() {
    // Initialize DCSS minimally
    // Read player.hp
    // Print value
    // Exit
    return 0;
}
```
- [ ] Create this test
- [ ] Verify it compiles
- [ ] Verify it runs

### 0.0.5: Create Testing Guide
- [ ] Document test harness setup
- [ ] Document how to add new tests
- [ ] Document expected output format
- [ ] Write example tests

---

## Fast Testing Strategy

### When Testing Phase 1 (Integration)

**Don't do this:**
1. Edit phase 1 code
2. Rebuild entire DCSS
3. Run full game
4. Manually test

**Do this instead:**
1. Edit phase 1 code
2. Compile test harness (30 secs)
3. Test specific function (instant)
4. See results

### Test Categories

**Category 1: Unit Tests** (fastest - test individual functions)
```cpp
// Test: Can read player HP
test_player_hp();  // Returns HP value, assert on it

// Test: Can read position
test_player_position();  // Returns x,y, assert on it
```
Time: 30 secs compile + run

**Category 2: Integration Tests** (medium - test components together)
```cpp
// Test: Create game, read state
test_create_game_and_read_state();

// Test: Execute move command
test_execute_move();
```
Time: 1-2 mins

**Category 3: Full Game Tests** (slowest - full game scenario)
```cpp
// Test: New game → move 10 times → read state
test_full_scenario();
```
Time: 3-5 mins (only run before commits)

---

## Makefile Setup

Add this to crawl-ref/source/Makefile:

```makefile
# Test harness compilation
test: test_harness.cc
	$(CXX) $(CXXFLAGS) -o test_harness test_harness.cc [object_files] [libraries]
	./test_harness

# Quick rebuild of test only (no full recompile)
test-quick:
	$(CXX) $(CXXFLAGS) -c test_harness.cc
	$(CXX) $(CXXFLAGS) -o test_harness test_harness.o [object_files] [libraries]
	./test_harness
```

---

## Compile Workflow During Development

### Scenario 1: Testing State Reader
```bash
# Edit source/state_reader.cc
vim state_reader.cc

# Compile just the test
make test-quick

# See results immediately (30 secs)

# If working, do full rebuild later
make clean && make
```

### Scenario 2: Testing Command Executor
```bash
# Edit source/command_exec.cc
vim command_exec.cc

# Quick test
make test-quick  # (30 secs)

# Iterate

# Once happy, full build
make  # (5 mins)

# Test in actual game
./crawl
```

### Scenario 3: Before Commit
```bash
# Do full rebuild
make clean && make  # (5 mins)

# Test in actual game (10+ mins)
./crawl

# If all good, commit
git commit -m "Phase 1.3: Read player inventory"
```

---

## Commands You'll Use Often

```bash
# Navigate to source
cd C:\GameDemo\crawl\crawl-ref\source

# Quick test (30 secs)
make test-quick

# Full rebuild (5 mins)
make clean && make

# Run game
./crawl

# Run specific test
./test_harness

# Search for a file
find . -name "player.h"

# Search for a symbol
grep -r "struct player" .

# View a section of code
head -50 player.h

# Count lines in a file
wc -l player.cc
```

---

## Build Troubleshooting

**Problem: "make: command not found"**
- Solution: Make sure you have gcc/make installed
- Windows: Use MinGW or Visual C++ build tools

**Problem: "undefined reference to..."**
- Solution: Missing library or object file in Makefile
- Check Makefile has all needed components

**Problem: "cannot find -lz" or similar**
- Solution: Missing dependency
- Install zlib-dev or similar

**Problem: Build works but test_harness won't compile**
- Solution: Missing DCSS headers in test file
- Add `#include "AppHdr.h"` at top of test_harness.cc

**Problem: "error: 'player' is not a type"**
- Solution: Need to include correct header
- Add `#include "player.h"` and verify path

---

## Success Criteria for Build Setup

✅ DCSS compiles cleanly  
✅ Game runs and is playable  
✅ Test harness compiles in < 1 minute  
✅ Can add new tests to harness quickly  
✅ Can iterate on code + test without full rebuild  

---

## Next: Dependency Check

Before starting Phase 0, verify you have:

- [ ] C++ compiler (gcc or MSVC)
- [ ] Make tool
- [ ] Python 3.x
- [ ] Git (optional but recommended)
- [ ] Text editor or IDE

Missing any? Let me know and we'll get them set up.

---

**Ready to compile? Start with "Build Step 1" above.**
