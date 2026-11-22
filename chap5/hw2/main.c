#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXLINE 100
#define MAXLEN  100

char savedText[MAXLINE][MAXLEN];

int main(int argc, char *argv[])
{
    int fd, total = 0, col = 0;
    char buf;

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

    for (int i = total - 1; i >= 0; i--)
        printf("%s\n", savedText[i]);

    return 0;
}
