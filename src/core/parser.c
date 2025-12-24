#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "basic.h"

void basic_eval_line(BasicState *state, const char *line) {
    Token token;
    tokenizer_init(line);
    tokenizer_next(&token);

    if (token.type == TOKEN_NUMBER) {
        // Line number processing
        int line_num = (int)token.number_value;
        const char *remaining = strchr(line, ' ');
        if (remaining) {
            // Store line
            while (*remaining == ' ') remaining++;
            program_add_line(state, line_num, remaining);
        } else {
             // Handle deletion if just line number (not fully implemented yet logic wise, but this is the hook)
        }
        return;
    }

    // Direct mode execution or statement execution
    while (token.type != TOKEN_EOF && token.type != TOKEN_CR) {
        if (token.type == TOKEN_PRINT) {
            tokenizer_next(&token);
            if (token.type == TOKEN_STRING) {
                state->pal->print(token.string_value);
                state->pal->newline();
            } else if (token.type == TOKEN_NUMBER) {
                char buf[64];
                sprintf(buf, BASIC_NUMBER_FMT, token.number_value);
                state->pal->print(buf);
                state->pal->newline();
            }
        } else if (token.type == TOKEN_LIST) {
            program_list(state);
        } else if (token.type == TOKEN_RUN) {
            basic_run(state);
        } else if (token.type == TOKEN_NEW) {
            program_clear(state);
        } else if (token.type == TOKEN_END) {
             // No-op in direct mode, stops run in program mode
        }
        
        tokenizer_next(&token);
    }
}

void parser_exec(BasicState *state) {
    // This function might be used for executing a single statement from the program
    // Currently, logic is embedded in basic_run and basic_eval_line
    (void)state;
}
