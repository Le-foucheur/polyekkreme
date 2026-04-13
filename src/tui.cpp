#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include "tui.h"
#include "maestro.h"

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

TUI::TUI(int w, int h) {
    this->width = w;
    this->heith = h;
    this->wall = false;
    this->screen = (char *)malloc(sizeof(char) * width * heith);
    for (int i = 0; i < width * heith; i++)
    {
        pos_char(i, ' ');
    }
}

TUI::TUI(int w, int h, int ofset) {
    this->width = w;
    this->heith = h;
    this->wall = false;
    this->offset = ofset;
    this->screen = (char *)malloc(sizeof(char) * width * heith);
    for (int i = 0; i < width * heith; i++)
    {
        pos_char(i, ' ');
    }
}

TUI::TUI(int w, int h, bool mur) {
    this->width = w;
    this->heith = h;
    this->wall = mur;
    this->offset = 0;
    this->screen = (char *)malloc(sizeof(char) * width * heith);
    for (int i = 0; i < width * heith; i++)
    {
        pos_char(i, ' ');
        if (mur && i % width == width - 1) {
            pos_char(i, '|');
        }
    }
}

/*      GET     */

int TUI::w() {
    return width;
}

int TUI::h() {
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

void TUI::pos_str(int i, std::string s) {
    int tmp = x(i);
    for (auto c : s) {
        if (c != '\n') {
            pos_char(i, c);
            i++;
        } else {
            i += tmp + width - x(i);
        }
    }
}

void TUI::pos_str(int x, int y, std::string s) {
    pos_str(y * width + x, s);
}



int TUI::x(int i) {
    return i % width;
}

int TUI::y(int i) {
    return i / width;
}

void TUI::print_screen() {
    printf("\x1b[1;1H");
    printf("\x1b[%dC", offset);
    for(int i = 0; i < width * heith; i++) {
        fprintf(stdout, "%c", screen[i]);
        if (i % width == width - 1 && i != width * heith - 1)
        {
            printf("\n");
            printf("\x1b[%dC", offset);
        }
    }
}

void TUI::add_info(bool pendule, Maestro m) {
    std::string s = "pas: " + std::to_string(m.dt()) + "\ntemps / temps maxium: " + std::to_string(m.t()) + " / " + std::to_string(m.tmax()) + "\nnb pendule / nb max: " + std::to_string(m.nb_p()) + " / " + std::to_string(m.nb_pmax());
    if (pendule) {
        Pendule** list = m.get_pendule();
        for (int i = 0; i < m.nb_p(); i++)
        {
            s += "\n\nid: " + std::to_string(list[i]->id());
            s += "\n      masse: " + std::to_string(list[i]->m());
            s += "\n      coord cart: " + std::to_string(list[i]->x()) + ", " + std::to_string(list[i]->y());
            s += "\n      coord pol: " + std::to_string(list[i]->r()) + ", " + std::to_string(list[i]->theta());
            s += "\n      vitesse angulaire: " + std::to_string(list[i]->omega());
        }
    }
    pos_str(1, 1, s);
}

void TUI::screen_clean() {
    for (int i = 0; i < width * heith; i++){
        pos_char(i, ' ');
    }
}