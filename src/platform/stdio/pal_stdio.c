#include <stdio.h>
#include <string.h>
#include "pal.h"

static void stdio_print(const char *s) {
    if (s) printf("%s", s);
}

static void stdio_print_error(const char *s) {
    if (s) fprintf(stderr, "%s\n", s);
}

static void stdio_newline(void) {
    printf("\n");
}

static int stdio_get_char(void) {
    return getchar();
}

static char *stdio_input_line(char *buffer, int size) {
    return fgets(buffer, size, stdin);
}

basic_pal_t pal_stdio = {
    .print = stdio_print,
    .print_error = stdio_print_error,
    .newline = stdio_newline,
    .get_char = stdio_get_char,
    .input_line = stdio_input_line,
    .yield = NULL
};
