#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "basic.h"

void program_add_line(BasicState *state, int line_number, const char *source) {
    ProgramLine *current;
    ProgramLine *prev;
    ProgramLine *new_line;
    
    new_line = (ProgramLine *)malloc(sizeof(ProgramLine));
    new_line->line_number = line_number;
    new_line->source = malloc(strlen(source) + 1);
    strcpy(new_line->source, source);
    new_line->next = NULL;

    current = state->program_head;
    prev = NULL;

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
    ProgramLine *next; 
    while (current) {
        next = current->next;
        free(current->source);
        free(current);
        current = next;
    }
    state->program_head = NULL;
}

void program_list(BasicState *state) {
    ProgramLine *current = state->program_head;
    char buffer[16];
    while (current) {
        sprintf(buffer, "%d ", current->line_number);
        state->pal->print(buffer);
        state->pal->print(current->source);
        state->pal->newline();
        current = current->next;
    }
}

ProgramLine *program_find_line(BasicState *state, int line_number) {
    ProgramLine *current = state->program_head;
    while (current) {
        if (current->line_number == line_number) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int program_save(BasicState *state, const char *filename) {
    FILE *fp;
    ProgramLine *current;
    
    fp = fopen(filename, "w");
    if (!fp) {
        return 0; /* Error */
    }
    
    current = state->program_head;
    while (current) {
        fprintf(fp, "%d %s\n", current->line_number, current->source);
        current = current->next;
    }
    
    fclose(fp);
    return 1; /* Success */
}

int program_load(BasicState *state, const char *filename) {
    FILE *fp;
    static char line[256]; /* Static to avoid stack overflow on C64/C128 */
    int line_num;
    char *source_start;
    
    fp = fopen(filename, "r");
    if (!fp) {
        return 0; /* Error */
    }
    
    /* Clear existing program */
    program_clear(state);
    
    while (fgets(line, sizeof(line), fp)) {
        /* Remove trailing newline */
        line[strcspn(line, "\r\n")] = '\0';
        
        /* Parse line number */
        line_num = atoi(line);
        if (line_num > 0) {
            /* Find start of source code (after line number and space) */
            source_start = strchr(line, ' ');
            if (source_start) {
                source_start++; /* Skip the space */
                program_add_line(state, line_num, source_start);
            }
        }
    }
    
    fclose(fp);
    return 1; /* Success */
}
