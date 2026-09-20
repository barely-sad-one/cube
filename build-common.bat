@echo off
setlocal EnableDelayedExpansion

:: parameters
:: 1: UNIT_NAME;      e.g. cube.core
:: 2: SRC_DIR;        e.g. \
:: 3: MAIN_SRC;       e.g. core_inc.cpp (unity builds)
:: 4: TARGET_TYPE;    e.g. exe | dll | static
:: 5: BUILD_TYPE;     e.g. debug | release (default: debug)
:: 6: EXTRA_DEFS;     e.g /DCUBE_EXPORT (optional)
:: 7: EXTRA_LIBS;     e.g "User32.lib" (optional)
:: 8: LIB_PATH;       (optional)
::                    - one or more space sperated token
::                    - a unit name (e.g. cube.core)
::                    - a raw path (contains \ or :)
:: 8: INCLUDE_PATH;  (optional)

set "UNIT_NAME=%~1"
set "SRC_DIR=%~2"
set "MAIN_SRC=%~3"
set "TARGET_TYPE=%~4"
set "BUILD_TYPE=%~5"
set "EXTRA_DEFS=%~6"
set "EXTRA_LIBS=%~7"
set "LIB_PATH=%~8"
set "INCLUDE_PATH=%~9"


if "%UNIT_NAME%"=="" (echo [error] UNIT_NAME missing & exit /b 1)
if "%SRC_DIR%"=="" (echo [error] SRC_DIR missing & exit /b 1)
if "%MAIN_SRC%"=="" (echo [error] MAIN_SRC missing & exit /b 1)
if "%TARGET_TYPE%"=="" (echo [error] TARGET_TYPE missing & exit /b 1)
if "%BUILD_CONFIG%"=="" set "BUILD_CONFIG=debug"

set "ROOT_DIR=%~dp0"
set "BUILD_DIR=%ROOT_DIR%build\%UNIT_NAME%\%BUILD_CONFIG%"
if not exist "%BUILD_DIR%" (
  echo [info] Creating build folder: "%BUILD_DIR%"
  mkdir "%BUILD_DIR%"
)

if /i "%BUILD_CONFIG%"=="debug" (
  set "CFG_FLAGS=/Zi /Od /MD /DNDEBUG /D_DEBUG"
) else (
  set "CFG_FLAGS=/O2 /Oi /MD /DNDEBUG /GL"
  set "LNK_FLAGS=/LTCG /OPT:REF /OPT:ICF"
)

set "CXXFLAGS=/nologo /W4 /std:c++20 %CFG_FLAGS%"
if /i "%TARGET_TYPE%"=="exe" (
  set "DEFS=%EXTRA_DEFS%"
) else (
  set "DEFS=/DCUBE_EXPORT %EXTRA_DEFS%"
)

set "OBJ=%BUILD_DIR%\%UNIT_NAME%.obj"
set "PDB=%BUILD_DIR%\%UNIT_NAME%.pdb"

set "LIBPATH_ARGS="
for %%P in (%LIB_PATH%) do (
    set "TOKEN=%%~P"
    set "RESOLVED="
    echo !TOKEN! | findstr /c:"\\" /c:":" >nul
    if errorlevel 1 (
        :: no \ or : found -> treat as a unit name
        set "RESOLVED=%ROOT_DIR%build\!TOKEN!\%BUILD_CONFIG%"
    ) else (
        :: already looks like a path -> use as-is
        set "RESOLVED=!TOKEN!"
    )
    set "LIBPATH_ARGS=!LIBPATH_ARGS! /LIBPATH:"!RESOLVED!""
)

set "INCLUDE_ARGS=/I. %INCLUDE_PATH%"

echo [info] Building %UNIT_NAME% [%TARGET_TYPE%, %BUILD_CONFIG%]
echo [info] Source:  %SRC_DIR%\%MAIN_SRC%

if /i "%TARGET_TYPE%"=="static" goto :build_static
if /i "%TARGET_TYPE%"=="dll"    goto :build_dll
if /i "%TARGET_TYPE%"=="exe"    goto :build_exe

echo [error] Unknown TARGET_TYPE: %TARGET_TYPE%
exit /b 1

:build_static
cl %CXXFLAGS% %DEFS% %INCLUDE_ARGS% /c "%SRC_DIR%\%MAIN_SRC%" /Fo:"%OBJ%" /Fd:"%PDB%"
if errorlevel 1 goto :fail
lib /nologo %LNK_FLAGS% "%OBJ%" /OUT:"%BUILD_DIR%\%UNIT_NAME%.lib"
if errorlevel 1 goto :fail
goto :ok

:build_dll
cl %CXXFLAGS% %DEFS% %INCLUDE_ARGS% "%SRC_DIR%\%MAIN_SRC%" /LD ^
   /Fo:"%OBJ%" /Fd:"%PDB%" /Fe:"%BUILD_DIR%\%UNIT_NAME%.dll" ^
   /link %LNK_FLAGS% %LIBPATH_ARGS% %EXTRA_LIBS% /OUT:"%BUILD_DIR%\%UNIT_NAME%.dll"
if errorlevel 1 goto :fail
goto :ok

:build_exe
cl %CXXFLAGS% %DEFS% %INCLUDE_ARGS% "%SRC_DIR%\%MAIN_SRC%" ^
   /Fo:"%OBJ%" /Fd:"%PDB%" /Fe:"%BUILD_DIR%\%UNIT_NAME%.exe" ^
   /link %LNK_FLAGS% %LIBPATH_ARGS% %EXTRA_LIBS% /OUT:"%BUILD_DIR%\%UNIT_NAME%.exe"
if errorlevel 1 goto :fail
goto :ok

:fail
echo [error] Build failed: %UNIT_NAME%
exit /b 1

:ok
echo [info] Build succeeded: %UNIT_NAME%
exit /b 0
