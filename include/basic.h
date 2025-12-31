#ifndef BASIC_H
#define BASIC_H

#include "pal.h"

#define APP_VERSION "1.0.1"

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
    TOKEN_LOAD,
    TOKEN_SAVE,
    TOKEN_REM,
    TOKEN_EQUALS,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_ASTERISK,
    TOKEN_SLASH,
    TOKEN_CARET,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    // Comparison operators
    TOKEN_LT,        // <
    TOKEN_GT,        // >
    TOKEN_LE,        // <=
    TOKEN_GE,        // >=
    TOKEN_NE,        // <>
    // Loop keywords
    TOKEN_FOR,
    TOKEN_TO,
    TOKEN_STEP,
    TOKEN_NEXT,
    // Functions
    TOKEN_SIN,
    TOKEN_COS,
    TOKEN_TAN,
    TOKEN_ATN,
    TOKEN_SQRT,
    TOKEN_LOG,
    TOKEN_EXP,
    TOKEN_ABS,
    TOKEN_INT,
    TOKEN_CR, // Carriage return / End of line
    TOKEN_UNKNOWN
} TokenType;

#if defined(__C64__) || defined(__C128__)
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

// FOR loop context
#define MAX_FOR_LOOPS 10
typedef struct {
    char var_name[3];           // Variable name (e.g., "I", "J")
    BasicNumber end_value;      // TO value
    BasicNumber step_value;     // STEP value (default 1)
    ProgramLine *loop_start;    // Line after FOR statement
} ForLoopContext;

// Interpreter State
typedef struct {
    basic_pal_t *pal;
    ProgramLine *program_head;
    int should_exit;
    BasicNumber variables[26 * 37]; // Support for A-Z + (A-Z)(A-Z0-9)
    char *str_variables[26 * 37];
    struct ProgramLine *jump_target;
    ForLoopContext for_stack[MAX_FOR_LOOPS];
    int for_stack_ptr;          // Current FOR loop depth
} BasicState;
// Core Functions
void basic_init(BasicState *state, basic_pal_t *pal);
void basic_run(BasicState *state);
void basic_eval_line(BasicState *state, const char *line);
BasicNumber *basic_get_var(BasicState *state, const char *name);
char **basic_get_str_var(BasicState *state, const char *name);
BasicNumber evaluate_expression(BasicState *state);

// Component Interfaces
// Tokenizer
void tokenizer_init(const char *input);
void tokenizer_next(Token *token);

// Program Storage
void program_add_line(BasicState *state, int line_number, const char *source);
void program_clear(BasicState *state);
void program_list(BasicState *state);
ProgramLine *program_find_line(BasicState *state, int line_number);
int program_save(BasicState *state, const char *filename);
int program_load(BasicState *state, const char *filename);

// Parser/Interpreter
void parser_exec(BasicState *state);

#endif
