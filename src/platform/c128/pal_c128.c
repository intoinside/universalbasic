#include <stdio.h>
#include <string.h>
#include "pal.h"

// C128 implementation using standard I/O (Kernal)

static void c128_print(const char *s) {
    if (s) printf("%s", s);
}

static void c128_print_error(const char *s) {
    // 28 = Red, 156 = Light Blue (Default) - Standard CBM codes
    if (s) printf("%c%s%c\n", 28, s, 154); 
}

static void c128_newline(void) {
    putchar('\n');
}

static int c128_get_char(void) {
    return getchar();
}

static char *c128_input_line(char *buffer, int size) {
    return fgets(buffer, size, stdin);
}

basic_pal_t pal_c128 = {
    .print = c128_print,
    .print_error = c128_print_error,
    .newline = c128_newline,
    .get_char = c128_get_char,
    .input_line = c128_input_line,
    .yield = NULL
};
