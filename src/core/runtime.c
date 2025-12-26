#include <stdlib.h>
#include <string.h>
#include "basic.h"

void basic_init(BasicState *state, basic_pal_t *pal) {
    int i;
    state->pal = pal;
    state->program_head = NULL;
    state->should_exit = 0;
    for (i = 0; i < 26 * 37; i++) {
        state->variables[i] = 0;
        state->str_variables[i] = NULL;
    }
}

BasicNumber *basic_get_var(BasicState *state, const char *name) {
    int index = 0;
    int first = 0;
    int second = -1;
    
    // First char (A-Z)
    if (name[0] >= 'A' && name[0] <= 'Z') {
        first = name[0] - 'A';
    } else {
        return NULL; // Invalid
    }

    // Second char (Optional: A-Z or 0-9)
    if (name[1] != '\0' && name[1] != '$') {
        if (name[1] >= 'A' && name[1] <= 'Z') {
            second = name[1] - 'A'; // 0-25
        } else if (name[1] >= '0' && name[1] <= '9') {
            second = 26 + (name[1] - '0'); // 26-35
        }
    }

    // Calculate index:
    // Block size per letter = 1 (single) + 26 (letters) + 10 (digits) = 37
    index = first * 37;
    if (second != -1) {
        index += 1 + second;
    }

    return &state->variables[index];
}

char **basic_get_str_var(BasicState *state, const char *name) {
    int index = 0;
    int first = 0;
    int second = -1;
    size_t len = strlen(name);
    
    // Must end with $
    if (len == 0 || name[len-1] != '$') return NULL;

    // First char (A-Z)
    if (name[0] >= 'A' && name[0] <= 'Z') {
        first = name[0] - 'A';
    } else {
        return NULL; // Invalid
    }

    // Second char (Optional: A-Z or 0-9) - check index 1 if it is NOT the $
    if (len > 2) {
       // e.g., A1$
       if (name[1] >= 'A' && name[1] <= 'Z') {
           second = name[1] - 'A'; // 0-25
       } else if (name[1] >= '0' && name[1] <= '9') {
           second = 26 + (name[1] - '0'); // 26-35
       }
    }

    // Calculate index:
    // Block size per letter = 1 (single) + 26 (letters) + 10 (digits) = 37
    index = first * 37;
    if (second != -1) {
        index += 1 + second;
    }

    return &state->str_variables[index];
}

void basic_run(BasicState *state) {
    ProgramLine *current = state->program_head;
    state->jump_target = NULL;

    while (current && !state->should_exit) {
        // Execute line
        // NOTE: This re-tokenizes every time, which is inefficient but simple for now
        // A real interpreter might tokenize once and store tokens (Phase 2 optimization)
        
        // We need to bypass the line number parsing of eval_line, 
        // so we call specific logic or just use eval_line but treat it as direct commands
        
        // Quick hack: Just interpret the source using eval_line logic 
        // essentially treating the body of the line as a command
        basic_eval_line(state, current->source);
        
        if (state->jump_target) {
            current = state->jump_target;
            state->jump_target = NULL; // Clear jump
        } else {
            current = current->next;
        }
    }
    state->should_exit = 0; // Reset for next run
}
