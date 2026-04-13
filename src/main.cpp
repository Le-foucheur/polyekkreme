#include <stdio.h>
#include <cmath>

#include "pendule.h"
#include "maestro.h"
#include "tui.h"

int main() {

    TUI tui;

    Maestro maitre = Maestro(2, 0.001, 10);
    
    Pendule test1 = Pendule(0, 0.5, 1, M_PI_4, 0);

    Pendule test2 = Pendule(1, 0.5, 1, 0, 0, &test1);

    maitre.add_pendule(&test1);
    maitre.add_pendule(&test2);

    maitre.print(true);

    maitre.calcule_tmax();

    maitre.print(true);

    tui.

    tui.print_screen();

    return 0;
}