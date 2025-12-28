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
    state->for_stack_ptr = 0;
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
    int old_for_depth;
    state->jump_target = NULL;

    while (current && !state->should_exit) {
        if (state->pal->yield) state->pal->yield();
        /* Save FOR depth before executing line */
        old_for_depth = state->for_stack_ptr;
        
        /* Execute line */
        basic_eval_line(state, current->source);
        
        /* Update loop_start if FOR was pushed */
        if (state->for_stack_ptr > old_for_depth) {
            state->for_stack[state->for_stack_ptr - 1].loop_start = current->next;
        }
        
        if (state->jump_target) {
            current = state->jump_target;
            state->jump_target = NULL;
        } else {
            current = current->next;
        }
    }
    state->should_exit = 0;
    state->for_stack_ptr = 0; /* Clear FOR stack on program end */
}
