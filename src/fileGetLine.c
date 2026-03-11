#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "fileGetLine.h"

long int fileGetLine(char **lineptr, size_t *n, FILE *stream)
{
    if (!lineptr || !n || !stream) return -1;
    if (*lineptr == NULL || *n == 0) {
        *n = 128;
        *lineptr = malloc(*n);
        if (!*lineptr) return -1;
    }
    size_t pos = 0;

    while (1) {
        int c = fgetc(stream);
        if (c == EOF) {
            if (pos == 0) return -1;
            break;
        }

        if (pos + 1 >= *n) {
            size_t newSize = *n * 2;
            char *temp = realloc(*lineptr, newSize);
            if (!temp) return -1;
            *lineptr = temp;
            *n = newSize;
        }

        (*lineptr)[pos++] = (char)c;
        if (c == '\n') break;
    }

    (*lineptr)[pos] = '\0';
    return (long int)pos;
}

