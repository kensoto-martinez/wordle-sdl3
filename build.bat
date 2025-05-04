@echo off

REM add -mwindows flag if you don't need the command prompt

set "EXE=build/wordle.exe"

if exist "%EXE%" (
    del "%EXE%"
    echo %EXE% deleted
)

gcc src/main.c src/game_functions.c src/config.c -o "%EXE%" -Iinclude -Llib -lSDL3 -lSDL3_ttf -lSDL3_image -mwindows

if %errorlevel% == 0 (
    echo Running %EXE%
    "%EXE%"
) else (
    echo Compilation failed.
)