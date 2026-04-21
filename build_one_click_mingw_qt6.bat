@echo off
setlocal EnableExtensions

REM One-click build/deploy for OpenFIRE-App-cn (Qt 6 + MinGW + Ninja)
REM Usage:
REM   build_one_click_mingw_qt6.bat
REM   build_one_click_mingw_qt6.bat --no-clean

set "ROOT=%~dp0"
if "%ROOT:~-1%"=="\" set "ROOT=%ROOT:~0,-1%"

set "QT_ROOT=D:\qt\6.5.3\mingw_64"
set "MINGW_BIN=D:\qt\Tools\mingw1120_64\bin"
set "CMAKE_EXE=C:\Program Files\CMake\bin\cmake.exe"
set "BUILD_DIR=%ROOT%\build\cli-mingw-release"
set "EXE_PATH=%BUILD_DIR%\OpenFIREapp.exe"

set "DO_CLEAN=1"
if /I "%~1"=="--no-clean" set "DO_CLEAN=0"

echo [INFO] Project root: %ROOT%
echo [INFO] Build dir   : %BUILD_DIR%
echo [INFO] Qt root     : %QT_ROOT%
echo.

if not exist "%CMAKE_EXE%" (
  echo [ERROR] CMake not found: %CMAKE_EXE%
  exit /b 1
)
if not exist "%QT_ROOT%\bin\windeployqt.exe" (
  echo [ERROR] windeployqt not found: %QT_ROOT%\bin\windeployqt.exe
  exit /b 1
)
if not exist "%MINGW_BIN%\g++.exe" (
  echo [ERROR] MinGW g++ not found: %MINGW_BIN%\g++.exe
  exit /b 1
)

set "PATH=%QT_ROOT%\bin;%MINGW_BIN%;C:\tools;%PATH%"

where ninja >nul 2>&1
if errorlevel 1 (
  echo [ERROR] ninja not found in PATH. Expected at C:\tools\ninja.exe
  exit /b 1
)

if "%DO_CLEAN%"=="1" (
  if exist "%BUILD_DIR%" (
    echo [INFO] Cleaning old build dir...
    rmdir /s /q "%BUILD_DIR%"
    if errorlevel 1 (
      echo [ERROR] Failed to remove build dir: %BUILD_DIR%
      exit /b 1
    )
  )
)

echo [STEP] Configuring...
"%CMAKE_EXE%" -S "%ROOT%" -B "%BUILD_DIR%" -G Ninja ^
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_PREFIX_PATH="%QT_ROOT%" ^
  -DCMAKE_C_COMPILER="%MINGW_BIN%\gcc.exe" ^
  -DCMAKE_CXX_COMPILER="%MINGW_BIN%\g++.exe"
if errorlevel 1 (
  echo [ERROR] CMake configure failed.
  exit /b 1
)

echo [STEP] Building...
"%CMAKE_EXE%" --build "%BUILD_DIR%" -j
if errorlevel 1 (
  echo [ERROR] Build failed.
  exit /b 1
)

if not exist "%EXE_PATH%" (
  echo [ERROR] Executable not found: %EXE_PATH%
  exit /b 1
)

echo [STEP] Deploying Qt runtime (windeployqt)...
"%QT_ROOT%\bin\windeployqt.exe" "%EXE_PATH%"
if errorlevel 1 (
  echo [ERROR] windeployqt failed.
  exit /b 1
)

echo [STEP] Sync translation qm files to i18n folder...
if not exist "%BUILD_DIR%\i18n" mkdir "%BUILD_DIR%\i18n"
copy /y "%BUILD_DIR%\AppTranslations_*.qm" "%BUILD_DIR%\i18n\" >nul 2>&1

echo.
echo [OK] Build + deploy completed.
echo [OK] Run: %EXE_PATH%
echo [TIP] Use --no-clean for incremental rebuild.
exit /b 0

