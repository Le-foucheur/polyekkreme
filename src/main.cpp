#include <stdio.h>

#include "pendule.h"
#include "maestro.h"

int main() {

    Maestro maitre = Maestro(2, 0.1, 10);
    
    Pendule test1 = Pendule(0, 0.5, 1, 1, 0);

    Pendule test2 = Pendule(1, 0.5, 1, 2, 0, &test1);

    maitre.add_pendule(&test1);
    maitre.add_pendule(&test2);

    maitre.print(true);

    maitre.calcule_temp_plus_1();

    maitre.print(true);

    return 0;
}