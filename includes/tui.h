#ifndef TUIH
#define TUIH

#include <stdlib.h>
#include <string>

#include "maestro.h"

class TUI {
    public:
    TUI();
    TUI(int w, int h);
    TUI(int w, int h, bool mur);
    TUI(int w, int h, int ofset);
    ~TUI();

    //GET

    int w();
    int h();
    char* ecran();

    //SET

    void pos_char(int x, int y, char c);
    void pos_char(int i, char c);
    
    void pos_str(int x, int y, std::string s);
    void pos_str(int i, std::string s);

    void add_info(bool pendule, Maestro m);
    void screen_clean();

    int x(int i);
    int y(int i);

    void print_screen();

    private:
        bool wall;
        int width;
        int heith;
        int offset = 0;
        char* screen = NULL;
};

#endif