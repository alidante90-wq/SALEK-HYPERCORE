# SALEK HYPERCORE VST3 — Local Windows Build Guide

## Prerequisites

Install these tools on Windows:

### 1. **Git for Windows**
```
Download: https://git-scm.com/download/win
Install with default options
```

### 2. **Visual Studio 2022 Community** (or Build Tools)
```
Download: https://visualstudio.microsoft.com/downloads/
Select: Desktop development with C++
Include: MSVC v143, Windows SDK, CMake tools
```

### 3. **CMake** (standalone)
```
Download: https://cmake.org/download/
OR: Install via Visual Studio installer
Verify: Open PowerShell and run:
  cmake --version
```

## Build Steps (Windows PowerShell)

### Clone and Build

```powershell
# Clone the repository
git clone https://github.com/alidante90-wq/SALEK-HYPERCORE.git
cd SALEK-HYPERCORE

# Create build directory
mkdir build
cd build

# Configure CMake (generates Visual Studio project)
# This automatically downloads JUCE from GitHub
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release

# Build in Release mode
cmake --build . --config Release --parallel

# The VST3 plugin will be located at:
# build\SalekHypercore_artefacts\Release\VST3\SALEK HYPERCORE.vst3
```

### Alternative: One-liner Build

```powershell
cd SALEK-HYPERCORE
cmake -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release --parallel
```

## Verify the Build

After the build completes, check that the VST3 exists:

```powershell
# List generated VST3
Get-ChildItem -Path build -Recurse -Filter *.vst3

# Should output something like:
# build\SalekHypercore_artefacts\Release\VST3\SALEK HYPERCORE.vst3
```

## Install VST3 Plugin

Windows VST3 plugins are typically installed in:

```
C:\Program Files\Common Files\VST3
```

### Copy VST3 to Standard Location

```powershell
$vst3Path = Get-ChildItem -Path "build" -Recurse -Filter "*.vst3" | Select-Object -First 1
Copy-Item $vst3Path.FullName "C:\Program Files\Common Files\VST3\" -Recurse -Force
```

## Test in DAW

1. **Open your DAW** (FL Studio, Ableton Live, Cubase, Reaper, etc.)
2. **Scan for plugins** (usually in Settings → Plugins → Rescan)
3. **Find "SALEK HYPERCORE"** in the VST3 section
4. **Create an instrument track** and load it
5. **Test basic functionality**:
   - MIDI input (play keys)
   - Sound output (should hear the synth)
   - Editor window opens

## Troubleshooting

### CMake not found
```powershell
# If cmake command fails, add it to PATH or use full path:
& "C:\Program Files\CMake\bin\cmake.exe" --version
```

### JUCE Download Fails
- Check internet connection
- JUCE will be downloaded to: `build\_deps\juce-src`
- If stuck, delete `build` folder and retry

### Build Fails with Compiler Errors
- Ensure Visual Studio 2022 is installed (not just Build Tools if you want IDE)
- Try deleting `build` folder and rebuilding
- Check that you're in the correct repository directory

### VST3 Not Found After Build
- Check `build\SalekHypercore_artefacts\Release\VST3\`
- Build output should show if compilation succeeded
- If no VST3, check CMake configure output for errors

## Clean Build

To start fresh:

```powershell
cd SALEK-HYPERCORE
Remove-Item -Path build -Recurse -Force
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release --parallel
```

## GitHub Actions

The same build runs automatically on GitHub Actions when you:
- Push to `main`, `master`, or `develop` branch
- Modify files in `Source/` or `CMakeLists.txt`
- Manually trigger via "Run workflow" in Actions tab

The artifact is available for download as **SALEK-HYPERCORE-Windows-VST3**.
