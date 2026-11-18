#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_file(const char *filename, int show_line_num, int *line_num) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror(filename);
        return;
    }

    int c;
    int at_line_start = 1;

    while ((c = fgetc(fp)) != EOF) {
        if (show_line_num && at_line_start) {
            printf("%6d  ", (*line_num)++);
            at_line_start = 0;
        }

        putchar(c);

        if (c == '\n') {
            at_line_start = 1;
        }
    }

    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-n] file1 [file2 ...]\n", argv[0]);
        exit(1);
    }

    int show_line_num = 0;
    int start_index = 1;
    int line_num = 1;

    if (strcmp(argv[1], "-n") == 0) {
        show_line_num = 1;
        start_index = 2;
    }

    if (start_index >= argc) {
        fprintf(stderr, "Usage: %s [-n] file1 [file2 ...]\n", argv[0]);
        exit(1);
    }

    for (int i = start_index; i < argc; i++) {
        print_file(argv[i], show_line_num, &line_num);
    }

    return 0;
}
