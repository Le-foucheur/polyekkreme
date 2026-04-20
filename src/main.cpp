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
    
    Maestro maitre = Maestro(100, 0.0001, 25);
    
    Pendule* ancre = new Pendule(0, 1, 1, M_PI_2, 0);
    
    maitre.add_pendule(ancre);
    
    
    for(int i = 1; i < 100; i++){
        Pendule* parent = maitre.get_pendule()[maitre.nb_p() - 1];
        
        Pendule* nouveau = new Pendule(i, 1, 1, 0, 0, parent);
        
        maitre.add_pendule(nouveau);
        
    }



    maitre.init_save();

    float dodo = 1.f;
    
    const double FPS = 300.0;
    const long micro_per_frame = (long)(1000000.0 / FPS); 

    int steps_per_frame = (int)((1.0 / FPS) / maitre.dt());
    if (steps_per_frame < 1) {
        steps_per_frame = 1;
    }

    printf("\x1b[?47h\x1b[?1049h\x1b[?25l");
    while (maitre.t() < maitre.tmax()) {
        clock_t t = clock();
        

        // print les info à gauche
        tuigauche.add_info(true, maitre);
        tuigauche.print_screen();
        
        //print les deux pendules
        tuidroit.draw_pendule(maitre);
        tuidroit.transfere_sub_to_screen();
        tuidroit.print_screen();

        maitre.save();

        for (int s = 0; s < steps_per_frame; s++){
            maitre.calcule_temp_plus_1();
        }
        

        //clean les pendules pour retiré les artefacte
        tuidroit.sub_screen_clean();
        tuidroit.screen_clean();
        tuigauche.screen_clean();
        tuidroit.transfere_sub_to_screen();


        //on retire le temps consomé par le procésus pour rendre le tout fluide
        t = clock() - t;
        dodo = (float)t / CLOCKS_PER_SEC ; 
        if (dodo <= micro_per_frame)
        {
            usleep((useconds_t)(micro_per_frame - dodo));
        }
    }
    printf("\x1b[?1049l\x1b[?25h\x1b[?47l");

    return 0;
}

