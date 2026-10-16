#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXLINE 100
#define MAXLEN  100

char savedText[MAXLINE][MAXLEN];

int main(int argc, char *argv[])
{
    int fd, total = 0, col = 0;
    char buf;
    char input[100];

    if (argc < 2) {
        fprintf(stderr, "usage: %s filename\n", argv[0]);
        return 1;
    }
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        perror("open");
        return 1;
    }

    while (read(fd, &buf, 1) > 0) {
        if (buf == '\n') {
            savedText[total][col] = '\0';
            total++;
            col = 0;
        } else {
            savedText[total][col++] = buf;
        }
    }
    if (col > 0) {
        savedText[total][col] = '\0';
        total++;
    }
    close(fd);

    printf("File read success\n");
    printf("Total Line : %d\n", total);
    printf("You can choose 1 ~ %d Line\n", total);
    printf("Pls 'Enter' the line to select : ");
    scanf("%99s", input);

    if (strcmp(input, "*") == 0) {
        for (int i = 0; i < total; i++)
            printf("%s\n", savedText[i]);
    }
    else if (strchr(input, '-')) {
        int s, e;
        sscanf(input, "%d-%d", &s, &e);
        for (int i = s; i <= e && i <= total; i++)
            if (i >= 1) printf("%s\n", savedText[i - 1]);
    }
    else if (strchr(input, ',')) {
        char *tok = strtok(input, ",");
        while (tok) {
            int n = atoi(tok);
            if (n >= 1 && n <= total)
                printf("%s\n", savedText[n - 1]);
            tok = strtok(NULL, ",");
        }
    }
    else {
        int n = atoi(input);
        if (n >= 1 && n <= total)
            printf("%s\n", savedText[n - 1]);
    }
    return 0;
}
