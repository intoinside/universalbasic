#include <stdio.h>
#include <string.h>
#include "pal.h"

// C64 implementation using standard I/O (Kernal)
// This ensures native scrolling and line editing work.

static void c64_print(const char *s) {
    if (s) printf("%s", s);
}

static void c64_print_error(const char *s) {
    // 28 = Red, 156 = Light Blue (Default)
    if (s) printf("%c%s%c\n", 28, s, 154); 
}

static void c64_newline(void) {
    putchar('\n');
}

static int c64_get_char(void) {
    return getchar();
}

static char *c64_input_line(char *buffer, int size) {
    // fgets uses proper Kernal INPUT routines with editing
    return fgets(buffer, size, stdin);
}

basic_pal_t pal_c64 = {
    .print = c64_print,
    .print_error = c64_print_error,
    .newline = c64_newline,
    .get_char = c64_get_char,
    .input_line = c64_input_line,
    .yield = NULL
};
