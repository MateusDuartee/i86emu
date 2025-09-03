@echo off
setlocal

set BUILD_DIR=build

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo Error: vswhere.exe not found. Install Visual Studio or vswhere.
    exit /b 1
)

for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do (
    set "VS_PATH=%%i"
)

if not defined VS_PATH (
    echo Error: Visual Studio not found.
    exit /b 1
)

call "%VS_PATH%\VC\Auxiliary\Build\vcvars64.bat"

set GENERATOR=Visual Studio 17 2022
set ARCHITECTURE=x64

echo Using generator: %GENERATOR%
echo Architecture: %ARCHITECTURE%

cmake -B %BUILD_DIR% -S . -G "%GENERATOR%" -A %ARCHITECTURE% -DCMAKE_BUILD_TYPE=Release
cmake --build %BUILD_DIR% --config Release

endlocal
