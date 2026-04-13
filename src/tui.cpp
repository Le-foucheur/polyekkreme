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
    this->wall = false;
    this->offset = 0;
    this->screen = (std::string*)malloc(sizeof(std::string) * width * heith);
    for (int i = 0; i < width * heith; i++){
        pos_char(i, " ");
    }
}

TUI::~TUI() {
    //free(screen);
}

TUI::TUI(int w, int h) {
    this->width = w;
    this->heith = h;
    this->wall = false;
    this->offset = 0;
    this->screen = (std::string *)malloc(sizeof(std::string) * width * heith);
    for (int i = 0; i < width * heith; i++)
    {
        pos_char(i, " ");
    }
}

TUI::TUI(int w, int h, int ofset) {
    this->width = w;
    this->heith = h;
    this->wall = false;
    this->offset = ofset;
    this->screen = (std::string *)malloc(sizeof(std::string) * width * heith);
    for (int i = 0; i < width * heith; i++)
    {
        pos_char(i, " ");
    }
}

TUI::TUI(int w, int h, bool mur) {
    this->width = w;
    this->heith = h;
    this->wall = mur;
    this->offset = 0;
    this->screen = (std::string *)malloc(sizeof(std::string) * width * heith);
    for (int i = 0; i < width * heith; i++)
    {
        pos_char(i, " ");
        if (mur && i % width == width - 1) {
            pos_char(i, "│");
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

std::string* TUI::ecran() {
    return screen;
}

/*      SET     */

void TUI::pos_char(int i, std::string c) {
    if (i >= width * heith){
        fprintf(stderr, "dépassement de l’écrant %d > %d",i , width * heith);
        exit(1);
    }
    screen[i] = c;
}

void TUI::pos_char(int x, int y, std::string c) {
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
            std::string tmp2;
            tmp2.push_back(c);
            pos_char(i, tmp2);
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
        printf("%s", screen[i].c_str());
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
        pos_char(i, " ");
    }
}

/*###########################################################################TUI PENDULE###########################################################################*/

TUI_PENDULE::TUI_PENDULE() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    this->width = w.ws_col;
    this->heith = w.ws_row;
    this->wall = false;
    this->offset = 0;
    this->screen = (std::string *)malloc(sizeof(std::string) * width * heith);

    this->sub_width = 2 * width;
    this->sub_heith = 3 * heith;
    this->sous_screen = (bool *)malloc(sizeof(bool) * sub_width * sub_heith);
    for (int i = 0; i < width * heith; i++)
    {
        pos_char(i, " ");
    }
    for (int i = 0; i < sub_width * sub_heith; i++)
    {
        pos_px(i, false);
    }
}

TUI_PENDULE::TUI_PENDULE(int w, int h, int ofset) {

    this->width = w;
    this->heith = h;
    this->wall = false;
    this->offset = ofset;
    this->screen = (std::string *)malloc(sizeof(std::string) * width * heith);

    this->sub_width = 2 * width;
    this->sub_heith = 3 * heith;
    this->sous_screen = (bool *)malloc(sizeof(bool) * sub_width * sub_heith);
    for (int i = 0; i < width * heith; i++)
    {
        pos_char(i, " ");
    }
    for (int i = 0; i < sub_width * sub_heith; i++)
    {
        pos_px(i, false);
    }
}

TUI_PENDULE::~TUI_PENDULE() {
    free(screen);
    free(sous_screen);
}

/*      GET     */

bool TUI_PENDULE::at(int x, int y) {
    return sous_screen[y * sub_width + x];
}


/*      SET     */

void TUI_PENDULE::pos_px(int i, bool px)
{
    sous_screen[i] = px;
}

void TUI_PENDULE::pos_px(int x, int y, bool px)
{
    pos_px(y * sub_width + x, px);
}

void TUI_PENDULE::transfere_sub_to_screen() {
    for (int i = 0; i < width * heith; i++) {
        // les x,y de l’ecran
        int x = i % width;
        int y = i / width;

        // les x,y du sous ecran de la case haut gauche de l’ecran
        int sub_x = 2 * x;
        int sub_y = 3 * y;

        bool hg = at(sub_x, sub_y); // pixel haut gauche
        bool hd = at(sub_x+1, sub_y); // pixel haut droit
        bool cg = at(sub_x, sub_y+1); // pixel centre gauche
        bool cd = at(sub_x+1, sub_y+1); // pixel centre droit
        bool bg = at(sub_x, sub_y+2); // pixel bas gauche
        bool bd = at(sub_x+1, sub_y+2); // pixel bas droit

        //POURQUOI J’AI PAS FAIS UN COMPTEUR BINAIRE + UN TABLEAU DE TAILLE 64 ?????? JE ME HAIS
        if (hg == true && hd == true && cg == true && cd == true && bg == true && bd == true) {
            pos_char(x, y, "█");
        }
        if (hg == false && hd == true && cg == true && cd == true && bg == true && bd == true)
        {
            pos_char(x, y, "🬻");
        }
        if (hg == true && hd == false && cg == true && cd == true && bg == true && bd == true)
        {
            pos_char(x, y, "🬺");
        }
        if (hg == false && hd == false && cg == true && cd == true && bg == true && bd == true)
        {
            pos_char(x, y, "🬹");
        }
        if (hg == true && hd == true && cg == false && cd == true && bg == true && bd == true)
        {
            pos_char(x, y, "🬸");
        }
        if (hg == false && hd == true && cg == false && cd == true && bg == true && bd == true)
        {
            pos_char(x, y, "🬷");
        }
        if (hg == true && hd == false && cg == false && cd == true && bg == true && bd == true)
        {
            pos_char(x, y, "🬶");
        }
        if (hg == false && hd == false && cg == false && cd == true && bg == true && bd == true)
        {
            pos_char(x, y, "🬵");
        }
        if (hg == true && hd == true && cg == true && cd == false && bg == true && bd == true)
        {
            pos_char(x, y, "🬴");
        }
        if (hg == false && hd == true && cg == true && cd == false && bg == true && bd == true)
        {
            pos_char(x, y, "🬳");
        }
        if (hg == true && hd == false && cg == true && cd == false && bg == true && bd == true)
        {
            pos_char(x, y, "🬲");
        }
        if (hg == false && hd == false && cg == true && cd == false && bg == true && bd == true)
        {
            pos_char(x, y, "🬱");
        }
        if (hg == true && hd == true && cg == false && cd == false && bg == true && bd == true)
        {
            pos_char(x, y, "🬰");
        }
        if (hg == false && hd == true && cg == false && cd == false && bg == true && bd == true)
        {
            pos_char(x, y, "🬯");
        }
        if (hg == true && hd == false && cg == false && cd == false && bg == true && bd == true)
        {
            pos_char(x, y, "🬮");
        }
        if (hg == false && hd == false && cg == false && cd == false && bg == true && bd == true)
        {
            pos_char(x, y, "🬭");
        }
        if (hg == true && hd == true && cg == true && cd == true && bg == false && bd == true)
        {
            pos_char(x, y, "🬬");
        }
        if (hg == false && hd == true && cg == true && cd == true && bg == false && bd == true)
        {
            pos_char(x, y, "🬫");
        }
        if (hg == true && hd == false && cg == true && cd == true && bg == false && bd == true)
        {
            pos_char(x, y, "🬪");
        }
        if (hg == false && hd == false && cg == true && cd == true && bg == false && bd == true)
        {
            pos_char(x, y, "🬩");
        }
        if (hg == true && hd == true && cg == false && cd == true && bg == false && bd == true)
        {
            pos_char(x, y, "🬨");
        }
        if (hg == false && hd == true && cg == false && cd == true && bg == false && bd == true)
        {
            pos_char(x, y, "▐");
        }
        if (hg == true && hd == false && cg == false && cd == true && bg == false && bd == true)
        {
            pos_char(x, y, "🬧");
        }
        if (hg == false && hd == false && cg == false && cd == true && bg == false && bd == true)
        {
            pos_char(x, y, "🬦");
        }
        if (hg == true && hd == true && cg == true && cd == false && bg == false && bd == true)
        {
            pos_char(x, y, "🬥");
        }
        if (hg == false && hd == true && cg == true && cd == false && bg == false && bd == true)
        {
            pos_char(x, y, "🬤");
        }
        if (hg == true && hd == false && cg == true && cd == false && bg == false && bd == true)
        {
            pos_char(x, y, "🬣");
        }
        if (hg == false && hd == false && cg == true && cd == false && bg == false && bd == true)
        {
            pos_char(x, y, "🬢");
        }
        if (hg == true && hd == true && cg == false && cd == false && bg == false && bd == true)
        {
            pos_char(x, y, "🬡");
        }
        if (hg == false && hd == true && cg == false && cd == false && bg == false && bd == true)
        {
            pos_char(x, y, "🬠");
        }
        if (hg == true && hd == false && cg == false && cd == false && bg == false && bd == true)
        {
            pos_char(x, y, "🬟");
        }
        if (hg == false && hd == false && cg == false && cd == false && bg == false && bd == true)
        {
            pos_char(x, y, "🬞");
        }
        if (hg == true && hd == true && cg == true && cd == true && bg == true && bd == false)
        {
            pos_char(x, y, "🬝");
        }
        if (hg == false && hd == true && cg == true && cd == true && bg == true && bd == false)
        {
            pos_char(x, y, "🬜");
        }
        if (hg == true && hd == false && cg == true && cd == true && bg == true && bd == false)
        {
            pos_char(x, y, "🬛");
        }
        if (hg == false && hd == false && cg == true && cd == true && bg == true && bd == false)
        {
            pos_char(x, y, "🬚");
        }
        if (hg == true && hd == true && cg == false && cd == true && bg == true && bd == false)
        {
            pos_char(x, y, "🬙");
        }
        if (hg == false && hd == true && cg == false && cd == true && bg == true && bd == false)
        {
            pos_char(x, y, "🬘");
        }
        if (hg == true && hd == false && cg == false && cd == true && bg == true && bd == false)
        {
            pos_char(x, y, "🬗");
        }
        if (hg == false && hd == false && cg == false && cd == true && bg == true && bd == false)
        {
            pos_char(x, y, "🬖");
        }
        if (hg == true && hd == true && cg == true && cd == false && bg == true && bd == false)
        {
            pos_char(x, y, "🬕");
        }
        if (hg == false && hd == true && cg == true && cd == false && bg == true && bd == false)
        {
            pos_char(x, y, "🬔");
        }
        if (hg == true && hd == false && cg == true && cd == false && bg == true && bd == false)
        {
            pos_char(x, y, "▌");
        }
        if (hg == false && hd == false && cg == true && cd == false && bg == true && bd == false)
        {
            pos_char(x, y, "🬓");
        }
        if (hg == true && hd == true && cg == false && cd == false && bg == true && bd == false)
        {
            pos_char(x, y, "🬒");
        }
        if (hg == false && hd == true && cg == false && cd == false && bg == true && bd == false)
        {
            pos_char(x, y, "🬑");
        }
        if (hg == true && hd == false && cg == false && cd == false && bg == true && bd == false)
        {
            pos_char(x, y, "🬐");
        }
        if (hg == false && hd == false && cg == false && cd == false && bg == true && bd == false)
        {
            pos_char(x, y, "🬏");
        }
        if (hg == true && hd == true && cg == true && cd == true && bg == false && bd == false)
        {
            pos_char(x, y, "🬎");
        }
        if (hg == false && hd == true && cg == true && cd == true && bg == false && bd == false)
        {
            pos_char(x, y, "🬍");
        }
        if (hg == true && hd == false && cg == true && cd == true && bg == false && bd == false)
        {
            pos_char(x, y, "🬌");
        }
        if (hg == false && hd == false && cg == true && cd == true && bg == false && bd == false)
        {
            pos_char(x, y, "🬋");
        }
        if (hg == true && hd == true && cg == false && cd == true && bg == false && bd == false)
        {
            pos_char(x, y, "🬊");
        }
        if (hg == false && hd == true && cg == false && cd == true && bg == false && bd == false)
        {
            pos_char(x, y, "🬉");
        }
        if (hg == true && hd == false && cg == false && cd == true && bg == false && bd == false)
        {
            pos_char(x, y, "🬈");
        }
        if (hg == false && hd == false && cg == false && cd == true && bg == false && bd == false)
        {
            pos_char(x, y, "🬇");
        }
        if (hg == true && hd == true && cg == true && cd == false && bg == false && bd == false)
        {
            pos_char(x, y, "🬆");
        }
        if (hg == false && hd == true && cg == true && cd == false && bg == false && bd == false)
        {
            pos_char(x, y, "🬅");
        }
        if (hg == true && hd == false && cg == true && cd == false && bg == false && bd == false)
        {
            pos_char(x, y, "🬄");
        }
        if (hg == false && hd == false && cg == true && cd == false && bg == false && bd == false)
        {
            pos_char(x, y, "🬃");
        }
        if (hg == true && hd == true && cg == false && cd == false && bg == false && bd == false)
        {
            pos_char(x, y, "🬂");
        }
        if (hg == false && hd == true && cg == false && cd == false && bg == false && bd == false)
        {
            pos_char(x, y, "🬁");
        }
        if (hg == true && hd == false && cg == false && cd == false && bg == false && bd == false)
        {
            pos_char(x, y, "🬀");
        }
         
    }
}