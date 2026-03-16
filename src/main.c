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
    int scroll = 0;
    int ch = 0;

    while (1) {
        FILE *files = popen("ls -1ap", "r");
        char *line = NULL;
        size_t len = 0;
        char *entries[MAX_FILES];
        int count = 0;

        while (fileGetLine(&line, &len, files) != -1) {
            line[strcspn(line, "\n")] = '\0';
            entries[count++] = strdup(line);
        }
        free(line);
        pclose(files);

        if (cursor >= count) cursor = count - 1;
        if (cursor < 0)      cursor = 0;

        int rows, cols;
        getmaxyx(stdscr, rows, cols);
        int visible = rows - 1;

        if (cursor < scroll)
            scroll = cursor;
        if (cursor >= scroll + visible)
            scroll = cursor - visible + 1;
        clear();

        char cwd[PATH_MAX];
        getcwd(cwd, PATH_MAX);
        mvprintw(0, 0, "%s", cwd);
        for (int i = 0; i < visible && (scroll + i) < count; i++) {
            int entry = scroll + i;
            if (entry == cursor)
                attron(A_REVERSE);
            mvprintw(i + 1, 0, "%s", entries[entry]);
            if (entry == cursor)
                attroff(A_REVERSE);
        }

        refresh();
        ch = getch();
        switch (ch) {
        case 'q':
            endwin();
            exit(0);
        case 'h':
            chdir("..");
            cursor = 0;
            scroll = 0;
            break;
        case 'j':
            if (cursor < count - 1) cursor++;
            break;
        case 'k':
            if (cursor > 0) cursor--;
            break;
        case 'l': {
            char *name = entries[cursor];
            int nlen = strlen(name);
            if (nlen > 0 && name[nlen - 1] == '/') {
                chdir(name);
                cursor = 0;
                scroll = 0;
            }
            break;
        }
        case 'g':
            cursor = 0;
            break;
        case 'G':
            cursor = count - 1;
            break;
        }
        for (int i = 0; i < count; i++)
            free(entries[i]);
    }

    endwin();
    return 0;
}
