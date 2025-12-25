#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "basic.h"

static const char *current_input;
static int current_pos;

void tokenizer_init(const char *input) {
    current_input = input;
    current_pos = 0;
}

static int is_whitespace(char c) {
    return c == ' ' || c == '\t';
}

void tokenizer_next(Token *token) {
    char c;
    int i;
    
    token->type = TOKEN_UNKNOWN;
    token->string_value[0] = '\0';
    token->number_value = 0;

    // Skip whitespace
    while (current_input[current_pos] && is_whitespace(current_input[current_pos])) {
        current_pos++;
    }

    if (current_input[current_pos] == '\0') {
        token->type = TOKEN_EOF;
        return;
    }

    c = current_input[current_pos];

    if (isdigit(c)) {
        token->type = TOKEN_NUMBER;
        i = 0;
        while (isdigit(current_input[current_pos]) || current_input[current_pos] == '.') {
            token->string_value[i++] = current_input[current_pos++];
            if (i >= TOKEN_MAX_STR - 1) break; 
        }
        token->string_value[i] = '\0';
#if defined(__C64__) || defined(__C128__)
        token->number_value = atoi(token->string_value);
#else
        token->number_value = atof(token->string_value);
#endif
    } else if (c == '"') {
        token->type = TOKEN_STRING;
        current_pos++; // Skip opening quote
        i = 0;
        while (current_input[current_pos] && current_input[current_pos] != '"') {
             token->string_value[i++] = current_input[current_pos++];
             if (i >= TOKEN_MAX_STR - 1) break;
        }
        token->string_value[i] = '\0';
        if (current_input[current_pos] == '"') current_pos++;
    } else if (isalpha(c)) {
        // Keyword or Identifier
        i = 0;
        while (isalpha(current_input[current_pos])) {
             token->string_value[i++] = toupper(current_input[current_pos++]);
             if (i >= TOKEN_MAX_STR - 1) break;
        }
        token->string_value[i] = '\0';

        if (strcmp(token->string_value, "PRINT") == 0) token->type = TOKEN_PRINT;
        else if (strcmp(token->string_value, "LET") == 0) token->type = TOKEN_LET;
        else if (strcmp(token->string_value, "IF") == 0) token->type = TOKEN_IF;
        else if (strcmp(token->string_value, "THEN") == 0) token->type = TOKEN_THEN;
        else if (strcmp(token->string_value, "GOTO") == 0) token->type = TOKEN_GOTO;
        else if (strcmp(token->string_value, "INPUT") == 0) token->type = TOKEN_INPUT;
        else if (strcmp(token->string_value, "LIST") == 0) token->type = TOKEN_LIST;
        else if (strcmp(token->string_value, "RUN") == 0) token->type = TOKEN_RUN;
        else if (strcmp(token->string_value, "END") == 0) token->type = TOKEN_END;
        else if (strcmp(token->string_value, "NEW") == 0) token->type = TOKEN_NEW;
        else token->type = TOKEN_IDENTIFIER;

    } else {
         // Single character tokens
         if (c == '=') token->type = TOKEN_EQUALS;
         else if (c == '+') token->type = TOKEN_PLUS;
         else if (c == '-') token->type = TOKEN_MINUS;
         else if (c == '\n' || c == '\r') token->type = TOKEN_CR;
         
         token->string_value[0] = c;
         token->string_value[1] = '\0';
         current_pos++;
    }
}
