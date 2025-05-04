@echo off

REM add -mwindows flag if you're on Windows & you don't want to see the command prompt

if "%~1"=="" (
    echo Usage: build.bat source_file.c
    exit /b 1
)

set "SRC=%~1"
set "SRC2=%~2"
set "SRC3=%~3"
set "EXE=build/wordle.exe"

if exist "%EXE%" (
    del "%EXE%"
    echo %EXE% deleted
)

gcc "%SRC%" "%SRC2%" "%SRC3%" -o "%EXE%" -Iinclude -Llib -lSDL3 -lSDL3_ttf -lSDL3_image

if %errorlevel% == 0 (
    echo Running %EXE%
    "%EXE%"
) else (
    echo Compilation failed.
)