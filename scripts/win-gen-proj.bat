@echo off
pushd %~dp0\..\
call vendor\premake\premake5.exe export-compile-commands vs2022
popd
PAUSE