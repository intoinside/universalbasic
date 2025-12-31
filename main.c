#include <stdio.h>
#include <string.h>
#include "basic.h"

#ifdef __C64__
extern basic_pal_t pal_c64;
#define CURRENT_PAL pal_c64
#elif defined(__C128__)
extern basic_pal_t pal_c128;
#define CURRENT_PAL pal_c128
#else
extern basic_pal_t pal_stdio;
#define CURRENT_PAL pal_stdio
#endif

int main() {
    BasicState state;
    static char buffer[256];
    char *input;
    size_t len;

    basic_init(&state, &CURRENT_PAL);
    state.pal->print("UNIVERSAL BASIC 2.0\n");
    state.pal->print("Version: ");
    state.pal->print(APP_VERSION);
    state.pal->print("\nReady.\n");

    while (1) {
        state.pal->print("READY.\n> ");
        input = state.pal->input_line(buffer, sizeof(buffer));
        
        if (!input) break; // EOF

        // Remove newline
        len = strlen(input);
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
