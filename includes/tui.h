#ifndef TUIH
#define TUIH

#include <stdlib.h>
#include <string>

#include "maestro.h"

class TUI {
    public:
    TUI();
    ~TUI();

    //GET

    unsigned short w();
    unsigned short h();
    char* ecran();

    //SET

    void pos_char(int x, int y, char c);
    void pos_char(int i, char c);
    
    void pos_str(int x, int y, std::string s);
    void pos_str(int i, std::string s);

    void add_info(bool pendule, Maestro m);

    int x(int i);
    int y(int i);

    void print_screen();

    private:
        unsigned short width;
        unsigned short heith;
        char* screen = NULL;
};

#endif