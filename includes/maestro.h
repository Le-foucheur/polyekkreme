#ifndef MAESTRO
#define MAESTRO

#include "pendule.h"

class Maestro {
    public:
        Maestro();
        Maestro(int nbmax, long double dt, long double tmax, double gravity, double frotement);
        ~Maestro();

        //Get
        Pendule** get_pendule();
        long double dt();
        long double tmax();
        long double t();
        long double l_totale();
        long double l_max();
        double g();
        double gam();
        int nb_p();
        int nb_pmax();

        //Set
        void add_pendule(Pendule* pendule);
        void dt(long double dt);
        void tmax(long double tmax);

        void print(bool pendule);
        void print();
        void print_pendules();
        void calcule_temp_plus_1();
        void calcule_tmax();

        void init_save();
        void save();

    private:
        int nb_pendule = 0;
        int nb_max_pendule = 0;
        double gamma = 0;
        double grav = 9.81;
        long double temps = 0;
        long double pas = 0.01;
        long double tempsmax = -1;
        long double lmax = 0;
        long double ltot = 0;
        Pendule** list_pendule = NULL;
};

template <typename T>
T max(T a, T b) {
    if (a >= b) {
        return a;
    } else {
        return b;
    }
}

#endif