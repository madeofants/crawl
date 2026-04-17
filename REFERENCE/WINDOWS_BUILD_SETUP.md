# Windows Build Setup for DCSS

DCSS uses GNU Make. On Windows, you need to install a Unix-like environment.

## Option 1: MSYS2 (Recommended)
Easiest option for Windows. Includes gcc, make, Python.

### Install MSYS2
1. Download from: https://www.msys2.org/
2. Run installer, follow defaults
3. Launch "MSYS2 MinGW 64-bit" terminal
4. Update packages:
```bash
pacman -Syu
```
(May need to run twice, restart terminal when prompted)

5. Install build tools:
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make python
```

6. Verify installation:
```bash
gcc --version
make --version
python --version
```

All three should output version info.

### Build DCSS with MSYS2
```bash
cd C:\GameDemo\crawl\crawl-ref\source
make
```

---

## Option 2: Visual Studio Build Tools
More traditional Windows approach. Heavier install.

### Install Visual Studio Build Tools
1. Download from: https://visualstudio.microsoft.com/downloads/
2. Choose "Desktop development with C++"
3. Install (takes 10+ mins)
4. Open "Developer Command Prompt for VS"
5. Navigate to source:
```bash
cd C:\GameDemo\crawl\crawl-ref\source
```

6. Check if nmake exists:
```bash
nmake /?
```

If it works, try to build (but DCSS is designed for GNU Make, may have issues):
```bash
nmake -f Makefile
```

---

## Option 3: WSL2 (Windows Subsystem for Linux)
If you already have WSL2 set up.

### Install build tools in WSL2
```bash
sudo apt update
sudo apt install build-essential python3
```

### Navigate to source
```bash
cd /mnt/c/GameDemo/crawl/crawl-ref/source
make
```

---

## Recommended: Use MSYS2

MSYS2 is:
- ✅ Easiest on Windows
- ✅ Designed for GNU Make projects
- ✅ Includes everything needed
- ✅ Only 200 MB download

**Do this:**
1. Install MSYS2
2. Update and install gcc/make/python
3. Open MSYS2 terminal
4. Navigate to source
5. Run `make`

---

## After Installation

Once you have make/gcc installed:

```bash
# Open MSYS2 terminal (if using that)
# Navigate to source
cd C:\GameDemo\crawl\crawl-ref\source

# Compile (takes 5-10 mins first time)
make

# Should create crawl.exe
ls *.exe
```

---

## Troubleshooting

**"gcc: not found"**
- Make sure build tools installed
- Restart terminal after installation
- Verify with: `gcc --version`

**"Error: Permission denied"**
- Make sure directory is writable
- Try: `chmod 755 crawl-ref/source`

**"Error: undefined reference to..."**
- Means missing library
- Make sure all dependencies installed
- Try: `make clean && make`

**"Make takes forever"**
- First build is slow (5-10 mins is normal)
- Subsequent builds faster
- Use parallel jobs: `make -j4`

---

**Let me know when MSYS2 is installed and working, then we'll compile DCSS.**
