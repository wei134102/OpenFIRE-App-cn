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
set "BUILD_DIR=%ROOT%\build\cli-mingw-work"
set "DIST_DIR=%ROOT%\build\cli-mingw-release"
set "EXE_PATH=%BUILD_DIR%\OpenFIREapp.exe"
set "DIST_EXE=%DIST_DIR%\OpenFIREapp.exe"

set "DO_CLEAN=1"
if /I "%~1"=="--no-clean" set "DO_CLEAN=0"

echo [INFO] Project root: %ROOT%
echo [INFO] Build work  : %BUILD_DIR%
echo [INFO] Release dir : %DIST_DIR%
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
    echo [INFO] Cleaning old build work dir...
    rmdir /s /q "%BUILD_DIR%"
    if errorlevel 1 (
      echo [ERROR] Failed to remove build dir: %BUILD_DIR%
      exit /b 1
    )
  )
  if exist "%DIST_DIR%" (
    echo [INFO] Cleaning old release dir...
    rmdir /s /q "%DIST_DIR%"
    if errorlevel 1 (
      echo [ERROR] Failed to remove release dir: %DIST_DIR%
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

echo [STEP] Preparing release directory...
if not exist "%DIST_DIR%" mkdir "%DIST_DIR%"
copy /y "%EXE_PATH%" "%DIST_EXE%" >nul
if errorlevel 1 (
  echo [ERROR] Failed to copy executable to release dir.
  exit /b 1
)

echo [STEP] Copying required runtime DLLs...
for %%F in (
  Qt6Core.dll
  Qt6Gui.dll
  Qt6Widgets.dll
  Qt6Network.dll
  Qt6SerialPort.dll
  Qt6Svg.dll
  Qt6SvgWidgets.dll
  D3Dcompiler_47.dll
  opengl32sw.dll
) do (
  if exist "%QT_ROOT%\bin\%%F" copy /y "%QT_ROOT%\bin\%%F" "%DIST_DIR%\" >nul
)
for %%F in (
  libgcc_s_seh-1.dll
  libstdc++-6.dll
  libwinpthread-1.dll
) do (
  if exist "%MINGW_BIN%\%%F" copy /y "%MINGW_BIN%\%%F" "%DIST_DIR%\" >nul
)

echo [STEP] Copying required Qt plugins...
if not exist "%DIST_DIR%\platforms" mkdir "%DIST_DIR%\platforms"
copy /y "%QT_ROOT%\plugins\platforms\qwindows.dll" "%DIST_DIR%\platforms\" >nul
if exist "%QT_ROOT%\plugins\styles\qwindowsvistastyle.dll" (
  if not exist "%DIST_DIR%\styles" mkdir "%DIST_DIR%\styles"
  copy /y "%QT_ROOT%\plugins\styles\qwindowsvistastyle.dll" "%DIST_DIR%\styles\" >nul
)
if exist "%QT_ROOT%\plugins\imageformats\qjpeg.dll" (
  if not exist "%DIST_DIR%\imageformats" mkdir "%DIST_DIR%\imageformats"
  copy /y "%QT_ROOT%\plugins\imageformats\q*.dll" "%DIST_DIR%\imageformats\" >nul
)
if exist "%QT_ROOT%\plugins\tls\qschannelbackend.dll" (
  if not exist "%DIST_DIR%\tls" mkdir "%DIST_DIR%\tls"
  copy /y "%QT_ROOT%\plugins\tls\q*.dll" "%DIST_DIR%\tls\" >nul
)

echo [STEP] Running windeployqt as fallback...
"%QT_ROOT%\bin\windeployqt.exe" --release --no-translations --dir "%DIST_DIR%" "%DIST_EXE%" >nul 2>&1

echo [STEP] Sync translation qm files to i18n folder...
if not exist "%DIST_DIR%\i18n" mkdir "%DIST_DIR%\i18n"
copy /y "%BUILD_DIR%\AppTranslations_*.qm" "%DIST_DIR%\i18n\" >nul 2>&1

echo.
echo [OK] Build + minimal release completed.
echo [OK] Run: %DIST_EXE%
echo [TIP] Work files are in: %BUILD_DIR%
echo [TIP] Use --no-clean for incremental rebuild.
exit /b 0

