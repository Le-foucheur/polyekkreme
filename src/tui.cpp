#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include "tui.h"

TUI::TUI() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    this->width = w.ws_col;
    this->heith = w.ws_row;
    this->screen = (char*)malloc(sizeof(char) * width * heith);
    for (int i = 0; i < width * heith; i++){
        pos_char(i, ' ');
    }
}

TUI::~TUI() {
    free(screen);
}

/*      GET     */

unsigned short TUI::w() {
    return width;
}

unsigned short TUI::h() {
    return heith;
}

char* TUI::ecran() {
    return screen;
}

/*      SET     */

void TUI::pos_char(int i, char c) {
    if (i >= width * heith){
        fprintf(stderr, "dépassement de l’écrant %d > %d",i , width * heith);
        exit(1);
    }
    screen[i] = c;
}

void TUI::pos_char(int x, int y, char c) {
    if (x >= width)
    {
        fprintf(stderr, "dépassement de l’écrant %d > %d", x, width );
        exit(1);
    } else if (y >= heith){
        fprintf(stderr, "dépassement de l’écrant %d > %d", y, heith );
        exit(1);
    }
    screen[y * width + x] = c;
}



int TUI::x(int i) {
    return i % width;
}

int TUI::y(int i) {
    return i / width;
}

void TUI::print_screen() {
    for(int i = 0; i < width * heith; i++) {
        fprintf(stdout, "%c", screen[i]);
    }
}