#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basic.h"

void program_add_line(BasicState *state, int line_number, const char *source) {
    ProgramLine *new_line = (ProgramLine *)malloc(sizeof(ProgramLine));
    new_line->line_number = line_number;
    new_line->source = strdup(source);
    new_line->next = NULL;

    ProgramLine *current = state->program_head;
    ProgramLine *prev = NULL;

    // Remove existing line if present
    while (current) {
        if (current->line_number == line_number) {
            if (prev) {
                prev->next = current->next;
            } else {
                state->program_head = current->next;
            }
            free(current->source);
            free(current);
            current = (prev) ? prev->next : state->program_head;
            break; // Should only be one
        }
        prev = current;
        current = current->next;
    }

    // Insert new line (sorted)
    current = state->program_head;
    prev = NULL;
    while (current && current->line_number < line_number) {
        prev = current;
        current = current->next;
    }

    if (prev) {
        prev->next = new_line;
    } else {
        state->program_head = new_line;
    }
    new_line->next = current;
}

void program_clear(BasicState *state) {
    ProgramLine *current = state->program_head;
    while (current) {
        ProgramLine *next = current->next;
        free(current->source);
        free(current);
        current = next;
    }
    state->program_head = NULL;
}

void program_list(BasicState *state) {
    ProgramLine *current = state->program_head;
    char buffer[512];
    while (current) {
        sprintf(buffer, "%d %s", current->line_number, current->source);
        state->pal->print(buffer);
        state->pal->newline();
        current = current->next;
    }
}
