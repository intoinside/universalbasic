@echo off
echo Building for Commodore 64...
cl65 -O -t c64 -D __C64__ -I include -o basic.prg src/core/tokenizer.c src/core/parser.c src/core/program.c src/core/runtime.c src/platform/c64/pal_c64.c main.c
if %ERRORLEVEL% EQU 0 (
    echo Build successful! basic.prg created.
) else (
    echo Build failed!
)
