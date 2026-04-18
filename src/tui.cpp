#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include "tui.h"
#include "maestro.h"

const std::string caractere[256] = {" ", "𜺨", "𜺫", "🮂", "𜴀", "▘", "𜴁", "𜴂", "𜴃", "𜴄", "▝", "𜴅", "𜴆", "𜴇", "𜴈", "▀", "𜴉", "𜴊", "𜴋", "𜴌", "🯦", "𜴍", "𜴎", "𜴏", "𜴐", "𜴑", "𜴒", "𜴓", "𜴔", "𜴕", "𜴖", "𜴗", "𜴘", "𜴙", "𜴚", "𜴛", "𜴜", "𜴝", "𜴞", "𜴟", "🯧", "𜴠", "𜴡", "𜴢", "𜴣", "𜴤", "𜴥", "𜴦", "𜴧", "𜴨", "𜴩", "𜴪", "𜴫", "𜴬", "𜴭", "𜴮", "𜴯", "𜴰", "𜴱", "𜴲", "𜴳", "𜴴", "𜴵", "🮅", "𜺣", "𜴶", "𜴷", "𜴸", "𜴹", "𜴺", "𜴻", "𜴼", "𜴽", "𜴾", "𜴿", "𜵀", "𜵁", "𜵂", "𜵃", "𜵄", "▖", "𜵅", "𜵆", "𜵇", "𜵈", "▌", "𜵉", "𜵊", "𜵋", "𜵌", "▞", "𜵍", "𜵎", "𜵏", "𜵐", "▛", "𜵑", "𜵒", "𜵓", "𜵔", "𜵕", "𜵖", "𜵗", "𜵘", "𜵙", "𜵚", "𜵛", "𜵜", "𜵝", "𜵞", "𜵟", "𜵠", "𜵡", "𜵢", "𜵣", "𜵤", "𜵥", "𜵦", "𜵧", "𜵨", "𜵩", "𜵪", "𜵫", "𜵬", "𜵭", "𜵮", "𜵯", "𜵰", "𜺠", "𜵱", "𜵲", "𜵳", "𜵴", "𜵵", "𜵶", "𜵷", "𜵸", "𜵹", "𜵺", "𜵻", "𜵼", "𜵽", "𜵾", "𜵿", "𜶀", "𜶁", "𜶂", "𜶃", "𜶄", "𜶅", "𜶆", "𜶇", "𜶈", "𜶉", "𜶊", "𜶋", "𜶌", "𜶍", "𜶎", "𜶏", "▗", "𜶐", "𜶑", "𜶒", "𜶓", "▚", "𜶔", "𜶕", "𜶖", "𜶗", "▐", "𜶘", "𜶙", "𜶚", "𜶛", "▜", "𜶜", "𜶝", "𜶞", "𜶟", "𜶠", "𜶡", "𜶢", "𜶣", "𜶤", "𜶥", "𜶦", "𜶧", "𜶨", "𜶩", "𜶪", "𜶫", "▂", "𜶬", "𜶭", "𜶮", "𜶯", "𜶰", "𜶱", "𜶲", "𜶳", "𜶴", "𜶵", "𜶶", "𜶷", "𜶸", "𜶹", "𜶺", "𜶻", "𜶼", "𜶽", "𜶾", "𜶿", "𜷀", "𜷁", "𜷂", "𜷃", "𜷄", "𜷅", "𜷆", "𜷇", "𜷈", "𜷉", "𜷊", "𜷋", "𜷌", "𜷍", "𜷎", "𜷏", "𜷐", "𜷑", "𜷒", "𜷓", "𜷔", "𜷕", "𜷖", "𜷗", "𜷘", "𜷙", "𜷚", "▄", "𜷛", "𜷜", "𜷝", "𜷞", "▙", "𜷟", "𜷠", "𜷡", "𜷢", "▟", "𜷣", "▆", "𜷤", "𜷥", "█"};

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
        return;
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
    if (i < width * heith){
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
    std::string ecran = "\x1b[1;1H\x1b[" + std::to_string(offset) + "C";
    int taille = width * heith;
    for(int i = 0; i < taille; i++) {
        ecran += screen[i];
        if (i % width == width - 1 && i != taille - 1)
        {
            ecran += "\n\x1b[" + std::to_string(offset) + "C";
        }
    }
    printf("%s", ecran.c_str());
}

void TUI::add_info(bool pendule, Maestro m, double fps) {
    Pendule** list = m.get_pendule();

    std::string s = "\nfps = " + std::to_string((int)round(fps)) + "\n\npas: " + std::to_string(m.dt()) + "\ntemps / temps maxium: " + std::to_string(m.t()) + " / " + std::to_string(m.tmax()) + "\nnb pendule / nb max: " + std::to_string(m.nb_p()) + " / " + std::to_string(m.nb_pmax());
    if (m.nb_p() == 2 ){

        double m1 = list[0]->m();
        double l1 = list[0]->r();
        double o1 = list[0]->omega();
        double m2 = list[1]->m();
        double l2 = list[1]->r();
        double o2 = list[1]->omega();
        double D = list[0]->theta() - list[1]->theta();

        double Ec = 1. / 2. * m1 * l1 * l1 * o1 * o1 + 1./2. * m2 * (l1 * l1 * o1 * o1 + l2 * l2 * o2 * o2 + 2 * l1 * l2 * o1 * o2 * cos(D));
        double Ep = 9.81 * (m1 * list[0]->y() + m2 * list[1]->y());
        s += "\n\nEp = " + std::to_string(Ep) + "\nEc = " + std::to_string(Ec) + "\nEm = " + std::to_string(Ec + Ep);
    }
    if (pendule) {
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
        if (wall && i % width == width - 1) {
            pos_char(i, "│");
        }
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
    this->sub_heith = 4 * heith;
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
    this->sub_heith = 4 * heith;
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
    if(i > sub_heith * sub_width || i < 0) {
        return;
    }
    sous_screen[i] = px;
}

void TUI_PENDULE::pos_px(int x, int y, bool px)
{
    pos_px(y * sub_width + x, px);
}

void TUI_PENDULE::circle(int x, int y){
    pos_px(x - 1, y - 1, true);
    pos_px(x, y - 1, true);
    pos_px(x + 1, y - 1, true);
    pos_px(x - 1, y, true);
    pos_px(x, y, true);
    pos_px(x + 1, y, true);
    pos_px(x - 1, y + 1, true);
    pos_px(x, y + 1, true);
    pos_px(x + 1, y + 1, true);
    pos_px(x, y - 2, true);
    pos_px(x - 2, y, true);
    pos_px(x, y + 2, true);
    pos_px(x + 2, y, true);
}

void TUI_PENDULE::petit_circle(int x, int y){
    pos_px(x, y - 1, true);
    pos_px(x - 1, y, true);
    pos_px(x, y, true);
    pos_px(x + 1, y, true);
    pos_px(x, y + 1, true);
    pos_px(x - 1, y - 1, true);
    pos_px(x - 1, y + 1, true);
    pos_px(x + 1, y - 1, true);
    pos_px(x + 1, y + 1, true);
}

//void TUI_PENDULE::ligne(int x1, int y1, int x2, int y2) {
//    if (y1 > y2)
//    int y = y1;
//    int dx = x2 - x1;
//    int dy = y2 - y1;
//    double e = 0.;
//    double e10 = (double)dy / (double)dx;
//    double e01 = -1.;
//
//    for(int x = x1; x < x2; x++){
//        pos_px(x,y, true);
//        e += e10;
//        if (e >= 0.5) {
//            y++;
//            e += e01;
//        }
//    }
//
//}

void TUI_PENDULE::ligne(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy;

    if (dx != 0)
    {
        if (dx > 0)
        {
            dy = y2 - y1;
            if (dy != 0)
            {
                if (dy > 0)
                {
                    // vecteur oblique dans le 1er quadran
                    if (dx >= dy)
                    {
                        // vecteur diagonal ou oblique proche de l’horizontale, dans le 1er octant
                        int e = dx;
                        dx = e * 2;
                        dy = dy * 2;
                        // e est positif
                        for (;;)
                        {
                            pos_px(x1, y1, true);
                            ++x1;
                            if (x1 == x2)
                            {
                                break;
                            }
                            e -= dy;
                            if (e < 0)
                            {
                                ++y1; // déplacement diagonal
                                e += dx;
                            }
                        }
                    }
                    else
                    {
                        // vecteur oblique proche de la verticale, dans le 2d octant
                        int e = dy;
                        dy = e * 2;
                        dx = dx * 2;
                        // e est positif
                        for (;;)
                        { // déplacements verticaux
                            pos_px(x1, y1, true);
                            ++y1;
                            if (y1 == y2)
                            {
                                break;
                            }
                            e -= dx;
                            if (e < 0)
                            {
                                ++x1; // déplacement diagonal
                                e += dy;
                            }
                        }
                    }
                }
                else
                { // dy < 0 (et dx > 0)
                    // vecteur oblique dans le 4e cadran
                    if (dx >= -dy)
                    {
                        // vecteur diagonal ou oblique proche de l’horizontale, dans le 8e octant
                        int e = dx;
                        dx = e * 2;
                        dy = dy * 2;
                        // e est positif
                        for (;;)
                        { // déplacements horizontaux
                            pos_px(x1, y1, true);
                            ++x1;
                            if (x1 == x2)
                            {
                                break;
                            }
                            e = e + dy;

                            if (e < 0)
                            {
                                --y1; // déplacement diagonal
                                e = e + dx;
                            }
                        }
                    }
                    else
                    {
                        // vecteur oblique proche de la verticale, dans le 7e octant
                        int e = dy;
                        dy = e * 2;
                        dx = dx * 2; // e est négatif
                        for (;;)
                        { // déplacements verticaux
                            pos_px(x1, y1, true);
                            --y1;
                            if (y1 == y2)
                            {
                                break;
                            }
                            e += dx;
                            if (e > 0)
                            {
                                ++x1; // déplacement diagonal
                                e += dy;
                            }
                        }
                    }
                }
            }
            else
            { // dy = 0 (et dx > 0)
                // vecteur horizontal vers la droite
                do
                {
                    pos_px(x1, y1, true);
                    ++x1;
                } while (x1 != x2);
            }
        }
        else
        { // dx < 0
            dy = y2 - y1;
            if (dy != 0)
            {
                if (dy > 0)
                {
                    // vecteur oblique dans le 2d quadran
                    if (-dx >= dy)
                    {
                        // vecteur diagonal ou oblique proche de l’horizontale, dans le 4e octant
                        int e = dx;
                        dx = e * 2;
                        dy = dy * 2;
                        // e est négatif
                        for (;;)
                        { // déplacements horizontaux
                            pos_px(x1, y1, true);
                            --x1;
                            if (x1 == x2)
                            {
                                break;
                            }
                            e += dy;
                            if (e >= 0)
                            {
                                ++y1; // déplacement diagonal
                                e += dx;
                            }
                        }
                    }
                    else
                    {
                        // vecteur oblique proche de la verticale, dans le 3e octant
                        int e = dy;
                        dy = e * 2;
                        dx = dx * 2;
                        // e est positif
                        for (;;)
                        { // déplacements verticaux
                            pos_px(x1, y1, true);
                            ++y1;
                            if (y1 == y2)
                            {
                                break;
                            }
                            e += dx;
                            if (e <= 0)
                            {
                                --x1; // déplacement diagonal
                                e += dy;
                            }
                        }
                    }
                }
                else
                { // dy < 0 (et dx < 0)
                    // vecteur oblique dans le 3e cadran
                    if (dx <= dy)
                    {
                        // vecteur diagonal ou oblique proche de l’horizontale, dans le 5e octant
                        int e = dx;
                        dx = e * 2;
                        dy = dy * 2;
                        // e est négatif
                        for (;;)
                        { // déplacements horizontaux
                            pos_px(x1, y1, true);
                            --x1;
                            if (x1 == x2)
                            {
                                break;
                            }
                            e -= dy;
                            if (e >= 0)
                            {
                                --y1; // déplacement diagonal
                                e += dx;
                            }
                        }
                    }
                    else
                    { // vecteur oblique proche de la verticale, dans le 6e octant
                        int e = dy;
                        dy = e * 2;
                        dx = dx * 2;
                        // e est négatif
                        for (;;)
                        { // déplacements verticaux
                            pos_px(x1, y1, true);
                            --y1;
                            if (y1 == y2)
                            {
                                break;
                            }
                            e -= dx;
                            if (e >= 0)
                            {
                                --x1; // déplacement diagonal
                                e += dy;
                            }
                        }
                    }
                }
            }
            else
            { // dy = 0 (et dx < 0)
                // vecteur horizontal vers la gauche
                do
                {
                    pos_px(x1, y1, true);
                    --x1;
                } while (x1 != x2);
            }
        }
    }
    else
    {
        // dx = 0
        dy = y2 - y1;
        if (dy != 0)
        {
            if (dy > 0)
            {
                // vecteur vertical croissant
                do
                {
                    pos_px(x1, y1, true);
                    ++y1;
                } while (y1 != y2);
            }
            else
            { // dy < 0 (et dx = 0)
                // vecteur vertical décroissant
                do
                {
                    pos_px(x1, y1, true);
                    --y1;
                } while (y1 != y2);
            }
        }
    }
    // le pixel final (x2, y2) n’est pas tracé.
}

int TUI_PENDULE::convertx(double x, Maestro m) {
    double tmplen = 2. * m.l_totale() + m.l_max() + 0.5;
    double coef = sub_width / (tmplen);
    return int(round(coef * x + sub_width / 2));
}

int TUI_PENDULE::converty(double y, Maestro m) {
    double tmplen = 2. * m.l_totale() + m.l_max() + 0.5;
    double coef = - sub_heith /(tmplen); 
    return int(round(coef * y + sub_heith / 3));
}

void TUI_PENDULE::draw_pendule(Maestro m) {
    Pendule** list = m.get_pendule();

    for (int i = 0; i < m.nb_p(); i++) {
        Pendule* pend = list[i];
        int x = convertx(pend->x(), m);
        int y = converty(pend->y(), m);
        
        Pendule *parent = pend->attacher();
        circle(x,y);
        if (parent != NULL)
        {
            int xp = convertx(parent->x(), m);
            int yp = converty(parent->y(), m);

            ligne(x, y, xp, yp);
        } else {
            ligne(x, y, convertx(0, m), converty(0, m));
        }
    }
}

void TUI_PENDULE::sub_screen_clean() {
    for (int i = 0; i < sub_width * sub_heith; i++)
    {
        pos_px(i, false);
    }
}


void TUI_PENDULE::transfere_sub_to_screen() {
    for (int i = 0; i < width * heith; i++) {
        // les x,y de l’ecran
        int x = i % width;
        int y = i / width;

        bool hg = at( 2 * x,  4 * y ); // pixel haut gauche
        bool hd = at( 2 * x+1,  4 * y ); // pixel haut droit
        bool chg = at( 2 * x,  4 * y +1); // pixel centre gauche
        bool chd = at( 2 * x+1,  4 * y +1); // pixel centre droit
        bool cbg = at( 2 * x,  4 * y +2); // pixel bas gauche
        bool cbd = at( 2 * x+1,  4 * y +2); // pixel bas droit
        bool bg = at( 2 * x,  4 * y +3); // pixel bas gauche
        bool bd = at( 2 * x+1,  4 * y +3); // pixel bas droit

        bool cases[8] = {bd, bg, cbd, cbg, chd, chg, hd, hg}; // représentation binaire en bit de point faible

        int indice = 0;
        for (int j = 0; j < 8; j++)
        {
            indice *= 2;
            if (cases[j]) {
                indice += 1;
            }
        }      

        pos_char(x, y, caractere[indice]);

    }
}