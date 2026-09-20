@echo off

set "CONFIG=debug"
set "MODE=exe"

set "EXTRA_DEFS=/DCUBE_EXPORT /DCUBE_ARENA_FREE_LIST /DCUBE_MEMORY_TRACK"
set "EXTRA_LIBS=User32.lib Shlwapi.lib"
set "LIB_PATHS="
set "INCLUDE_PATHS="

pushd "%~dp0"
call "%~dp0build-common.bat" cube.runtime "%~dp0cube.runtime" main.cpp %MODE% %CONFIG% "%EXTRA_DEFS%" "%EXTRA_LIBS%" "%LIB_PATHS%" "%INCLUDE_PATHS%"
popd



