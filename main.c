#include <stdio.h>
#include <string.h>
#include "basic.h"

extern basic_pal_t pal_stdio;

int main() {
    BasicState state;
    char buffer[1024];

    basic_init(&state, &pal_stdio);

    state.pal->print("BASIC 2.0 (Retro-Oriented) - Ready.\n");

    while (1) {
        state.pal->print("READY.\n> ");
        char *input = state.pal->input_line(buffer, sizeof(buffer));
        
        if (!input) break; // EOF

        // Remove newline
        size_t len = strlen(input);
        if (len > 0 && input[len-1] == '\n') {
            input[len-1] = '\0';
        }

        if (strcmp(input, "EXIT") == 0 || strcmp(input, "QUIT") == 0) {
            break;
        }

        basic_eval_line(&state, input);
    }

    return 0;
}
