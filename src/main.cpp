#include <stdio.h>
#include <cmath>
#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <ctime>
#include <chrono>

#include "pendule.h"
#include "maestro.h"
#include "tui.h"

void my_handler(int s)
{
    printf("\x1b[?1049l\x1b[?25h\x1b[?47l");
    exit(s);
}

int main() {

    signal(SIGINT, my_handler);

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    TUI tuigauche = TUI(w.ws_col / 3, w.ws_row, true);
    TUI_PENDULE tuidroit = TUI_PENDULE( 2 * w.ws_col / 3, w.ws_row - 4, tuigauche.w() + 1);

    Maestro maitre = Maestro(2, 0.001, 10);
    
    Pendule test1 = Pendule(0, 0.5, 1, M_PI_4, 0);

    Pendule test2 = Pendule(1, 0.5, 1, 0, 0, &test1);

    maitre.add_pendule(&test1);
    maitre.add_pendule(&test2);
    
    printf("\x1b[?47h\x1b[?1049h\x1b[?25l");
    for (double i = maitre.t(); i < maitre.tmax(); i += maitre.dt()) {
        clock_t now = clock();

        tuigauche.add_info(true, maitre);
        tuigauche.print_screen();

        tuidroit.transfere_sub_to_screen();
        tuidroit.print_screen();
        tuidroit.petit_circle(100,100);
        maitre.calcule_temp_plus_1();

        clock_t diff = clock() - now;
        int dodo = (int)diff / (CLOCKS_PER_SEC / 1000); 
        if (dodo <= 70)
        {
            usleep(dodo);
        }
    }
    printf("\x1b[?1049l\x1b[?25h\x1b[?47l");

    return 0;
}

