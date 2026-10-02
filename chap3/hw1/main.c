#include <stdio.h>
#include <string.h>
#include "copy.h"

#define MAXLINE 100

char lines[5][MAXLINE];
int lengths[5];

int main() {
    int i, j;
    char temp[MAXLINE];
    int temp_len;

    i = 0;
    while (i < 5 && gets(lines[i]) != NULL) {
        lengths[i] = strlen(lines[i]);
        i++;
    }

    for (i = 0; i < 4; i++) {
        for (j = i + 1; j < 5; j++) {
            if (lengths[i] < lengths[j]) {
                temp_len = lengths[i];
                lengths[i] = lengths[j];
                lengths[j] = temp_len;

                copy(lines[i], temp);
                copy(lines[j], lines[i]);
                copy(temp, lines[j]);
            }
        }
    }

    for (i = 0; i < 5; i++) {
        printf("%s\n", lines[i]);
    }

    return 0;
}
