#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

static void sigchld_handler(int sig)
{
    int saved = errno;
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
    errno = saved;
}

static void run_command(char *cmd)
{
    char *args[MAX_ARGS];
    char *infile = NULL, *outfile = NULL;
    int argc = 0, background = 0;
    size_t len;
    char *tok, *save;

    len = strlen(cmd);
    while (len > 0 && (cmd[len - 1] == ' ' || cmd[len - 1] == '\t' || cmd[len - 1] == '\n'))
        cmd[--len] = '\0';
    if (len > 0 && cmd[len - 1] == '&') {
        background = 1;
        cmd[--len] = '\0';
    }

    char buf[MAX_LINE * 3];
    size_t j = 0;
    for (size_t i = 0; cmd[i] && j < sizeof(buf) - 4; i++) {
        if (cmd[i] == '<' || cmd[i] == '>') {
            buf[j++] = ' ';
            buf[j++] = cmd[i];
            buf[j++] = ' ';
        } else {
            buf[j++] = cmd[i];
        }
    }
    buf[j] = '\0';

    for (tok = strtok_r(buf, " \t\n", &save); tok; tok = strtok_r(NULL, " \t\n", &save)) {
        if (strcmp(tok, "<") == 0) {
            infile = strtok_r(NULL, " \t\n", &save);
            if (!infile) { fprintf(stderr, "syntax error: '<' 뒤에 파일 필요\n"); return; }
        } else if (strcmp(tok, ">") == 0) {
            outfile = strtok_r(NULL, " \t\n", &save);
            if (!outfile) { fprintf(stderr, "syntax error: '>' 뒤에 파일 필요\n"); return; }
        } else if (argc < MAX_ARGS - 1) {
            args[argc++] = tok;
        }
    }
    args[argc] = NULL;
    if (argc == 0)
        return;

    if (strcmp(args[0], "exit") == 0)
        exit(0);
    if (strcmp(args[0], "cd") == 0) {
        const char *dir = args[1] ? args[1] : getenv("HOME");
        if (!dir || chdir(dir) < 0)
            perror("cd");
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }
    if (pid == 0) {

        signal(SIGINT, SIG_DFL);

        if (infile) {
            int fd = open(infile, O_RDONLY);
            if (fd < 0) { perror(infile); _exit(1); }
            dup2(fd, STDIN_FILENO);
            close(fd);
        } else if (background) {

            int fd = open("/dev/null", O_RDONLY);
            if (fd >= 0) { dup2(fd, STDIN_FILENO); close(fd); }
        }
        if (outfile) {
            int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) { perror(outfile); _exit(1); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        execvp(args[0], args);
        perror(args[0]);
        _exit(127);
    }

    if (background) {
        printf("[background] pid %d\n", pid);
    } else {
        int status;
        while (waitpid(pid, &status, 0) < 0 && errno == EINTR)
            ;
    }
}

int main(void)
{
    char line[MAX_LINE];
    struct sigaction sa;

    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
    sigaction(SIGCHLD, &sa, NULL);
    signal(SIGINT, SIG_IGN);

    while (1) {
        printf("[shell] ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) {
            if (feof(stdin)) { printf("\n"); break; }
            clearerr(stdin);
            continue;
        }

        char *save, *cmd;
        for (cmd = strtok_r(line, ";", &save); cmd; cmd = strtok_r(NULL, ";", &save))
            run_command(cmd);
    }
    return 0;
}

