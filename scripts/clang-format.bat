@echo off
pushd %~dp0\..\
call clang-format -i src/*.cpp src/*.h
popd
PAUSE