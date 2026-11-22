#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAXLINE 1024
#define MAXARG 128

void execute_line(char *cmd);

int main() {
    char buf[MAXLINE];

    while (1) {
        printf("[shell] ");
        fflush(stdout);

        if (fgets(buf, MAXLINE, stdin) == NULL) {
            break;
        }
        
        buf[strcspn(buf, "\n")] = '\0';
        
        if (strlen(buf) == 0) continue;
        if (strcmp(buf, "exit") == 0) break;

        char *saved_cmd;
        char *single_cmd = strtok_r(buf, ";", &saved_cmd);
        while (single_cmd != NULL) {
            while (*single_cmd == ' ') single_cmd++;
            execute_line(single_cmd);
            single_cmd = strtok_r(NULL, ";", &saved_cmd);
        }
    }
    return 0;
}

void execute_line(char *cmd) {
    char *args[MAXARG];
    char *infile = NULL, *outfile = NULL;
    int background = 0;
    int i = 0;

    int len = strlen(cmd);
    if (len > 0 && cmd[len - 1] == '&') {
        background = 1;
        cmd[len - 1] = '\0';
    }

    char *saved_ptr;
    char *token = strtok_r(cmd, " \t", &saved_ptr);
    while (token != NULL) {
        if (strcmp(token, ">") == 0) {
            outfile = strtok_r(NULL, " \t", &saved_ptr);
        } else if (strcmp(token, "<") == 0) {
            infile = strtok_r(NULL, " \t", &saved_ptr);
        } else {
            args[i++] = token;
        }
        token = strtok_r(NULL, " \t", &saved_ptr);
    }
    args[i] = NULL;
    if (args[0] == NULL) return;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
        return;
    } else if (pid == 0) {
        if (infile != NULL) {
            int fd_in = open(infile, O_RDONLY);
            if (fd_in < 0) { perror("open infile error"); exit(1); }
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }
        if (outfile != NULL) {
            int fd_out = open(outfile, O_CREAT | O_TRUNC | O_WRONLY, 0660);
            if (fd_out < 0) { perror("open outfile error"); exit(1); }
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }

        execvp(args[0], args);
        perror("execvp error");
        exit(1);
    } else {
        if (!background) {
            int status;
            waitpid(pid, &status, 0);
        } else {
            printf("[Background PID %d]\n", pid);
        }
    }
}
