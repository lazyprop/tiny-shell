#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

#include "parsing.c"

#define MAX_INP_SIZE 1024
#define MAX_ARG_COUNT 10
#define MAX_OUTP_SIZE 4096

void tsh_launch(char** args) {
    int stdout_link[2];
    pipe(stdout_link);

    pid_t pid = fork();

    if (pid == 0) {

        close(stdout_link[0]);

        dup2(stdout_link[1], STDOUT_FILENO);

        int e = execvp(args[0], args);
        if (e == -1) {
            printf("tsh: %s: %s\n", args[0], strerror(errno));
        }

        exit(0);

    } else if (pid < 0) {

        printf("tsh: fork() failed");
        exit(1);

    } else {

        close(stdout_link[1]);

        char out_buf[MAX_OUTP_SIZE] = {};
        read(stdout_link[0], out_buf, MAX_OUTP_SIZE);
        fputs(out_buf, stdout);

        waitpid(pid, NULL, WUNTRACED);

    }
}


void tsh_cd(char** args) {
    if (args[1] == NULL) {
        fprintf(stderr, "tsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("tsh");
        }
    }
}

int main() {
    while (1) {
        printf("> ");

        // TODO: no max input size
        char inp[MAX_INP_SIZE];
        fgets(inp, MAX_INP_SIZE, stdin);

        char** args = tokenize(inp);

        if (args[0] == NULL) {
            continue;
        } else if (strcmp(args[0], "cd") == 0) {
            tsh_cd(args);
        } else if (strcmp(args[0], "exit") == 0) {
            exit(0);
        } else {
            tsh_launch(args);
        }
    }
}
