#ifndef PAL_H
#define PAL_H

typedef struct {
    void (*print)(const char *s);
    void (*print_error)(const char *s);
    void (*newline)(void);
    int  (*get_char)(void);
    char *(*input_line)(char *buffer, int size);
} basic_pal_t;

#endif
