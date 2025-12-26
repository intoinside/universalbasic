#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if !defined(__C64__) && !defined(__C128__)
#include <math.h>
#endif
#include "basic.h"

// Forward declarations
BasicNumber expr_parse_expression(BasicState *state, Token *token);

void tokenizer_peek(Token *token) {
    // Current tokenizer implementation is stateful and doesn't support peek/pushback easily without buffer manipulation.
    // For this simple Basic, we will just use the current token and advance.
    // Ideally tokenizer should be decoupled. 
    // Here we assume 'token' passed to functions contains the *current* token to process.
    // And functions call tokenizer_next to advance *after* consuming.
}

// Factor: Number, Variable, (Expr), Function(Expr), Unary -
BasicNumber expr_parse_factor(BasicState *state, Token *token) {
    BasicNumber result = 0;
    
    if (token->type == TOKEN_NUMBER) {
        result = token->number_value;
        tokenizer_next(token);
    } else if (token->type == TOKEN_IDENTIFIER) {
        BasicNumber *var = basic_get_var(state, token->string_value);
        if (var) result = *var;
        tokenizer_next(token);
    } else if (token->type == TOKEN_MINUS) {
        tokenizer_next(token);
        result = -expr_parse_factor(state, token);
    } else if (token->type == TOKEN_LPAREN) {
        tokenizer_next(token);
        result = expr_parse_expression(state, token);
        if (token->type == TOKEN_RPAREN) {
            tokenizer_next(token);
        }
    } else if (token->type >= TOKEN_SIN && token->type <= TOKEN_INT) {
        // Functions
        TokenType func = token->type;
        tokenizer_next(token);
        if (token->type == TOKEN_LPAREN) {
            tokenizer_next(token);
            result = expr_parse_expression(state, token);
            if (token->type == TOKEN_RPAREN) {
                tokenizer_next(token);
            }
            // Apply function
            // Apply function
            switch (func) {
#if !defined(__C64__) && !defined(__C128__)
                case TOKEN_SIN: result = (BasicNumber)sin((double)result); break;
                case TOKEN_COS: result = (BasicNumber)cos((double)result); break;
                case TOKEN_TAN: result = (BasicNumber)tan((double)result); break;
                case TOKEN_ATN: result = (BasicNumber)atan((double)result); break;
                case TOKEN_SQRT: result = (BasicNumber)sqrt((double)result); break;
                case TOKEN_LOG: result = (BasicNumber)log((double)result); break;
                case TOKEN_EXP: result = (BasicNumber)exp((double)result); break;
                case TOKEN_ABS: result = (BasicNumber)fabs((double)result); break;
                case TOKEN_INT: result = (BasicNumber)((long)result); break;
#else
                // C64/C128 Integer Math Fallback (Stubs or Integer approximations)
                case TOKEN_ABS: if (result < 0) result = -result; break;
                case TOKEN_INT: break; // Already int
                case TOKEN_SQRT: {
                    // Simple Integer SQRT
                    int s = 0;
                    while ((s+1)*(s+1) <= result) s++;
                    result = s;
                    break;
                }
                default: 
                    // Other Trig functions return 0 for now in Integer mode
                    result = 0; 
                    break;
#endif
            }
        }
    }
    return result;
}

// Term: Factor * / ^ Factor
BasicNumber expr_parse_term(BasicState *state, Token *token) {
    BasicNumber result = expr_parse_factor(state, token);
    TokenType op;
    BasicNumber rhs;
    
    while (token->type == TOKEN_ASTERISK || token->type == TOKEN_SLASH || token->type == TOKEN_CARET) {
        op = token->type;
        tokenizer_next(token);
        rhs = expr_parse_factor(state, token);
        if (op == TOKEN_ASTERISK) result *= rhs;
        else if (op == TOKEN_SLASH) {
            if (rhs != 0) result /= rhs; else result = 0; // Div by zero protection
        }
        else if (op == TOKEN_CARET) {
#if !defined(__C64__) && !defined(__C128__)
             result = (BasicNumber)pow((double)result, (double)rhs);
#else
             // Integer Power
             BasicNumber base = result;
             BasicNumber exp = rhs;
             BasicNumber res = 1;
             while (exp > 0) {
                 if (exp % 2 == 1) res *= base;
                 base *= base;
                 exp /= 2;
             }
             result = res;
#endif
        }
    }
    return result;
}

// Expression: Term + - Term
BasicNumber expr_parse_expression(BasicState *state, Token *token) {
    BasicNumber result = expr_parse_term(state, token);
    TokenType op;
    BasicNumber rhs;

    while (token->type == TOKEN_PLUS || token->type == TOKEN_MINUS) {
        op = token->type;
        tokenizer_next(token);
        rhs = expr_parse_term(state, token);
        if (op == TOKEN_PLUS) result += rhs;
        else if (op == TOKEN_MINUS) result -= rhs;
    }
    return result;
}

BasicNumber evaluate_expression(BasicState *state) {
    // Wrapper for cases where we need to start parsing an expression from current parser state
    // But since our parser implementation inside eval_line uses local 'token', 
    // it's tricky to share state without passing the token back and forth.
    // For now, only usage this inside parser if we refactor or usage global/state token.
    // Actually, let's just make the expr functions take the Token* from the caller.
    return 0; 
}

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
            } else {
                // Try printing expression
                BasicNumber val = expr_parse_expression(state, &token);
                char buf[64];
                sprintf(buf, BASIC_NUMBER_FMT, val);
                state->pal->print(buf);
                state->pal->newline();
            }
        } else if (token.type == TOKEN_LET || token.type == TOKEN_IDENTIFIER) {
            // Assignment
            Token var_token;
            BasicNumber *target_ptr = NULL;

            if (token.type == TOKEN_LET) {
                tokenizer_next(&token); // Skip LET, get Identifier
            }
            var_token = token; // Save identifier token structure
            
            if (token.type == TOKEN_IDENTIFIER) {
                tokenizer_next(&token); // Get =
                if (token.type == TOKEN_EQUALS) {
                    tokenizer_next(&token); // Get Value
                    // Evaluate Expression
                    target_ptr = basic_get_var(state, var_token.string_value);
                    if (target_ptr) {
                        *target_ptr = expr_parse_expression(state, &token);
                    }
                }
            }
        } else if (token.type == TOKEN_LIST) {
            program_list(state);
        } else if (token.type == TOKEN_RUN) {
            basic_run(state);
        } else if (token.type == TOKEN_GOTO) {
             // GOTO
             BasicNumber target_line;
             ProgramLine *dest;
             
             tokenizer_next(&token); // Skip GOTO
             target_line = expr_parse_expression(state, &token);
             
             dest = program_find_line(state, (int)target_line);
             if (dest) {
                 state->jump_target = dest;
             } else {
                 state->pal->print_error("?UNDEF'D STATEMENT ERROR");
             }
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
