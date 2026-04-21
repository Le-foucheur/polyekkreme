#ifndef TUIH
#define TUIH

#include <stdlib.h>
#include <string>
#include <vector>

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
    std::string* ecran();

    //SET

    void pos_char(int x, int y, std::string c);
    void pos_char(int i, std::string c);
    
    void pos_str(int x, int y, std::string s);
    void pos_str(int i, std::string s);

    void add_info(bool pendule, Maestro m, std::vector<int>* show_id);
    void screen_clean();

    int x(int i);
    int y(int i);

    void print_screen();

    protected:
        bool wall;
        int width;
        int heith;
        int offset = 0;
        std::string* screen = NULL;
};

class TUI_PENDULE : public TUI {
    public:
    TUI_PENDULE();
    TUI_PENDULE(int w, int h, int ofset, bool res);
    ~TUI_PENDULE();

    // GET

    bool at(int x, int y);

    //SET

    void pos_px(int x, int y, bool px);
    void pos_px(int i, bool px);

    int convertx(double x, Maestro m);
    int converty(double y, Maestro m);

    void circle(int x, int y);
    void petit_circle(int x, int y);
    void ligne(int x1, int y1, int x2, int y2);
    void draw_pendule(Maestro m);

    void transfere_sub_to_screen();
    void sub_screen_clean();

    private : 
        int sub_width;
        int sub_heith;
        bool resolution = false;
        bool* sous_screen = NULL;
};

#endif