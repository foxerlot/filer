#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include "fileGetLine.h"

#define MAX_FILES 1024

int main(void)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int cursor = 0;
    int ch = 0;

    while (1) {
        FILE *files = popen("ls -1ap", "r");

        char *line = NULL;
        size_t len = 0;
        long int nread;

        char *entries[MAX_FILES];
        int count = 0;

        while ((nread = fileGetLine(&line, &len, files)) != -1) {
            line[strcspn(line, "\n")] = '\0';
            entries[count++] = strdup(line);
        }
        free(line);
        pclose(files);

        if (cursor >= count) cursor = count - 1;
        if (cursor < 0) cursor = 0;

        clear();
        char cwd[PATH_MAX];
        getcwd(cwd, PATH_MAX);
        mvprintw(0, 0, "%s", cwd);

        for (int i = 0; i < count; i++) {
            if (i == cursor)
                attron(A_REVERSE);
            mvprintw(i + 1, 0, "%s", entries[i]);
            if (i == cursor)
                attroff(A_REVERSE);
        }

        refresh();
        ch = getch();

        switch (ch) {
        case 'q':
            endwin();
            exit(0);
            break;
        case 'h':
            chdir("..");
            cursor = 0;
            break;
        case 'j':
            if (cursor < count - 1) cursor++;
            break;
        case 'k':
            if (cursor > 0) cursor--;
            break;
        case 'l': {
            char *name = entries[cursor];
            int len = strlen(name);
            if (len > 0 && name[len - 1] == '/') {
                chdir(name);
                cursor = 0;
            }
            break;
        }
        }

        for (int i = 0; i < count; i++)
            free(entries[i]);
    }

    endwin();
    return 0;
}
