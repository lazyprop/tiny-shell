#include <string.h>
#include <stdlib.h>

#define MAX_ARG_COUNT 10

char** tokenize(char* inp) {
    char* delims = " \n\r\t";
    char** tokens = malloc(sizeof(char*) * MAX_ARG_COUNT);

    char* cur_tok = strtok(inp, delims);
    for (int i = 0; i < MAX_ARG_COUNT && cur_tok != NULL; i++) {
        tokens[i] = cur_tok;
        cur_tok = strtok(NULL, delims);
    }

    return tokens;
}

char** make_pipeline(char* str) {
    char** 
}

