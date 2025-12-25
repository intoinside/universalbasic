@echo off
echo Building for Commodore 128...
cl65 -O -t c128 -D __C128__ -I include -o basic128.prg src/core/tokenizer.c src/core/parser.c src/core/program.c src/core/runtime.c src/platform/c128/pal_c128.c main.c
if %ERRORLEVEL% EQU 0 (
    echo Build successful! basic128.prg created.
) else (
    echo Build failed!
)
