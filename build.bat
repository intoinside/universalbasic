@echo off
echo Building universalbasic...
if not exist bin mkdir bin
gcc -Iinclude -Wall -Wextra -std=c99 src/core/tokenizer.c src/core/parser.c src/core/program.c src/core/runtime.c src/platform/stdio/pal_stdio.c main.c -o basic.exe
if %ERRORLEVEL% EQU 0 (
    echo Build successful! Run with: .\basic.exe
) else (
    echo Build failed!
)
