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

    TUI tuigauche = TUI((w.ws_col - 1) / 3, w.ws_row, true);
    TUI_PENDULE tuidroit = TUI_PENDULE( 2 * (w.ws_col - 1) / 3, w.ws_row, tuigauche.w() + 1);

    Maestro maitre = Maestro(10, 0.001, 10);
    
    Pendule test1 = Pendule(0, 0.5, 1, 1.799, 0);

    Pendule test2 = Pendule(1, 0.5, 0.5, 1.019, 0, &test1);

    maitre.add_pendule(&test1);
    maitre.add_pendule(&test2);


    maitre.init_save();
    
    printf("\x1b[?47h\x1b[?1049h\x1b[?25l");
    for (double i = maitre.t(); i < maitre.tmax(); i += maitre.dt()) {
        clock_t now = clock();
        
        // print les info à gauche
        tuigauche.add_info(true, maitre);
        tuigauche.print_screen();
        
        //print les deux pendules
        tuidroit.draw_pendule(maitre);
        tuidroit.transfere_sub_to_screen();
        tuidroit.print_screen();
        maitre.save();
        maitre.calcule_temp_plus_1();
        
        //clean les pendules pour retiré les artefacte
        tuidroit.sub_screen_clean();
        tuidroit.screen_clean();
        tuigauche.screen_clean();
        tuidroit.transfere_sub_to_screen();

        //on retire le temps consomé par le procésus pour rendre le tout fluide
        clock_t diff = clock() - now;
        int dodo = (int)((double)diff / (1000.)); 
        if (dodo <= 70)
        {
            usleep(70);
        }
    }
    printf("\x1b[?1049l\x1b[?25h\x1b[?47l");

    return 0;
}

