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
            } else if (token.type == TOKEN_IDENTIFIER) {
                // Print variable
                BasicNumber *var_ptr = basic_get_var(state, token.string_value);
                if (var_ptr) {
                    char buf[64];
                    sprintf(buf, BASIC_NUMBER_FMT, *var_ptr);
                    state->pal->print(buf);
                    state->pal->newline();
                }
            }
        } else if (token.type == TOKEN_LET || token.type == TOKEN_IDENTIFIER) {
            // Assignment
            Token var_token;
            BasicNumber *target_ptr = NULL;
            BasicNumber *source_ptr = NULL;

            if (token.type == TOKEN_LET) {
                tokenizer_next(&token); // Skip LET, get Identifier
            }
            var_token = token; // Save identifier token structure
            
            if (token.type == TOKEN_IDENTIFIER) {
                tokenizer_next(&token); // Get =
                if (token.type == TOKEN_EQUALS) {
                    tokenizer_next(&token); // Get Value
                    target_ptr = basic_get_var(state, var_token.string_value);
                    
                    if (target_ptr) {
                        if (token.type == TOKEN_NUMBER) {
                            *target_ptr = token.number_value;
                        } else if (token.type == TOKEN_IDENTIFIER) {
                             // Simple copy from another variable
                             source_ptr = basic_get_var(state, token.string_value);
                             if (source_ptr) {
                                 *target_ptr = *source_ptr;
                             }
                        }
                    }
                }
            }
        } else if (token.type == TOKEN_LIST) {
            program_list(state);
        } else if (token.type == TOKEN_RUN) {
            basic_run(state);
        } else if (token.type == TOKEN_NEW) {
            program_clear(state);
        } else if (token.type == TOKEN_END) {
             // No-op
        }
        
        tokenizer_next(&token);
    }
}

void parser_exec(BasicState *state) {
    // This function might be used for executing a single statement from the program
    // Currently, logic is embedded in basic_run and basic_eval_line
    (void)state;
}
