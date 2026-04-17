@echo off
REM Compile DCSS using MSYS2
REM This script finds MSYS2 and runs make

setlocal enabledelayedexpansion

REM Common MSYS2 installation locations
set MSYS2_PATHS=^
  C:\msys64\^
  C:\msys32\^
  C:\tools\msys64\^
  %LOCALAPPDATA%\msys64\

for %%P in (%MSYS2_PATHS%) do (
  if exist "%%P\usr\bin\make.exe" (
    echo Found MSYS2 at: %%P
    set MSYS2_HOME=%%P
    goto found
  )
)

echo ERROR: MSYS2 not found in standard locations
echo Install MSYS2 or add to PATH manually
pause
exit /b 1

:found
set PATH=!MSYS2_HOME!usr\bin;!MSYS2_HOME!mingw64\bin;!PATH!

echo Compiling DCSS...
cd /d C:\GameDemo\crawl\crawl-ref\source
make -j4

if !errorlevel! equ 0 (
  echo.
  echo SUCCESS: Build complete
  echo Looking for executable...
  dir *.exe
) else (
  echo.
  echo ERROR: Build failed (error code !errorlevel!)
)

pause
