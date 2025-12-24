#ifndef BASIC_H
#define BASIC_H

#include "pal.h"

// Token types
typedef enum {
    TOKEN_EOF,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_IDENTIFIER,
    TOKEN_PRINT,
    TOKEN_LET,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_GOTO,
    TOKEN_INPUT,
    TOKEN_LIST,
    TOKEN_RUN,
    TOKEN_END,
    TOKEN_NEW,
    TOKEN_EQUALS,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_CR, // Carriage return / End of line
    TOKEN_UNKNOWN
} TokenType;

#ifdef __C64__
    typedef int BasicNumber;
    #define BASIC_NUMBER_FMT "%d"
    #define TOKEN_MAX_STR 80
#else
    typedef double BasicNumber;
    #define BASIC_NUMBER_FMT "%g"
    #define TOKEN_MAX_STR 256
#endif

typedef struct {
    TokenType type;
    char string_value[TOKEN_MAX_STR];
    BasicNumber number_value;
} Token;

// Program Line Structure
typedef struct ProgramLine {
    int line_number;
    char *source;
    struct ProgramLine *next;
} ProgramLine;

// Interpreter State
typedef struct {
    basic_pal_t *pal;
    ProgramLine *program_head;
    int should_exit;
} BasicState;

// Core Functions
void basic_init(BasicState *state, basic_pal_t *pal);
void basic_run(BasicState *state);
void basic_eval_line(BasicState *state, const char *line);

// Component Interfaces
// Tokenizer
void tokenizer_init(const char *input);
void tokenizer_next(Token *token);

// Program Storage
void program_add_line(BasicState *state, int line_number, const char *source);
void program_clear(BasicState *state);
void program_list(BasicState *state);

// Parser/Interpreter
void parser_exec(BasicState *state);

#endif
