#ifndef MAESTRO
#define MAESTRO

#include "pendule.h"

class Maestro {
    public:
        Maestro();
        Maestro(int nbmax, double dt, double tmax);
        ~Maestro();

        //Get
        Pendule** get_pendule();
        double dt();
        double tmax();
        double t();
        int nb_p();
        int nb_pmax();

        //Set
        void add_pendule(Pendule* pendule);
        void dt(double dt);
        void tmax(double tmax);

        void print(bool pendule);
        void print();
        void print_pendules();
        void calcule_temp_plus_1();
        void calcule_tmax();

        void init_save();
        void save();

    private:
        int nb_pendule;
        int nb_max_pendule;
        double temps;
        double grav = 9.81;
        double pas;
        double tempsmax;
        Pendule** list_pendule;
};

#endif