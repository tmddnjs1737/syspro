#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAXLINE 1024
#define MAXARG 64

int main() {
    char buf[MAXLINE];
    char *args[MAXARG];
    int status;
    pid_t pid;

    while (1) {
        printf("Pls input cmd : ");
        fflush(stdout);
        
        if (fgets(buf, MAXLINE, stdin) == NULL) {
            break;
        }
        
        buf[strcspn(buf, "\n")] = '\0';
        if (strlen(buf) == 0) continue;

        if (strcmp(buf, "Exit") == 0 || strcmp(buf, "exit") == 0) {
            printf("Exit\n");
            break;
        }

        int background = 0;
        int i = 0;
        char *saved_ptr;
        
        char *token = strtok_r(buf, " ", &saved_ptr);
        while (token != NULL) {
            args[i++] = token;
            token = strtok_r(NULL, " ", &saved_ptr);
        }
        args[i] = NULL;

        if (args[0] == NULL) continue;

        if (i > 0 && strcmp(args[i - 1], "&") == 0) {
            background = 1;
            args[i - 1] = NULL;
        }

        printf("[%d] Parent process start\n", getpid());

        pid = fork();
        if (pid < 0) {
            perror("fork error");
            continue;
        } else if (pid == 0) {
            execvp(args[0], args);
            fprintf(stderr, "%s: No such file or directory\n", args[0]);
            exit(127);
        } else {
            if (!background) {
                pid_t child = wait(&status);
                printf("[%d] child process %d end\n", getpid(), child);
                if (WIFEXITED(status)) {
                    printf("end code %d\n", WEXITSTATUS(status));
                }
            } else {
                printf("[%d] child process start\n", pid);
            }
        }
        printf("SUCCESS\n");
    }
    return 0;
}
