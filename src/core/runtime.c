#include <stdlib.h>
#include "basic.h"

void basic_init(BasicState *state, basic_pal_t *pal) {
    state->pal = pal;
    state->program_head = NULL;
    state->should_exit = 0;
}

void basic_run(BasicState *state) {
    ProgramLine *current = state->program_head;
    while (current && !state->should_exit) {
        // Execute line
        // NOTE: This re-tokenizes every time, which is inefficient but simple for now
        // A real interpreter might tokenize once and store tokens (Phase 2 optimization)
        
        // We need to bypass the line number parsing of eval_line, 
        // so we call specific logic or just use eval_line but treat it as direct commands
        
        // Quick hack: Just interpret the source using eval_line logic 
        // essentially treating the body of the line as a command
        basic_eval_line(state, current->source);
        
        current = current->next;
    }
    state->should_exit = 0; // Reset for next run
}
