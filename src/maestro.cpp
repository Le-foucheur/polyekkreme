#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include "maestro.h"
#include "pendule.h"

Maestro::Maestro() {
    this->list_pendule = (Pendule**)malloc(sizeof(Pendule*));
}

Maestro::~Maestro() {
    free(list_pendule);
}

Maestro::Maestro(int nbmax, double dt, double tmax) {
    this->nb_max_pendule = nbmax;
    this->nb_pendule = 0;
    this->temps = 0;
    this->grav =9.91;
    this->pas = dt;
    this->tempsmax = tmax;
    this->list_pendule = (Pendule**)malloc(sizeof(Pendule*) * nbmax);
} 

/*      Get     */

Pendule** Maestro::get_pendule() {
    return list_pendule;
}

double Maestro::dt() {
    return pas;
}

double Maestro::tmax() {
    return tempsmax;
}

/*      Set     */

void Maestro::add_pendule(Pendule* pendule) {
    if (nb_pendule >= nb_max_pendule) {
        fprintf(stderr, "impossible d’ajouter des pendules");
        exit(1);
    }
    this->list_pendule[nb_pendule] = pendule;
    this->nb_pendule += 1;
}

void Maestro::dt(double dt) {
    this->pas = dt;
}
void Maestro::tmax(double tmax) {
    this->tempsmax = tmax;
}

/*      Les print   */

void Maestro::print_pendules() {
    for (int i = 0; i < nb_pendule; i++){
        list_pendule[i]->print();
        printf("\n");
    }
}

void Maestro::print(bool pendule){
    printf("pas : %g\n", pas);
    printf("temps / temps maximum: %g / %g\n", temps, tempsmax);
    printf("nb pendule / nb max: %d / %d\n", nb_pendule, nb_max_pendule);
    if (pendule) {
        print_pendules();
    } 
}

void Maestro::print() {
    print(false);
}

/*      Resolution numerique    */

// les foncts des equa diff du double pendule
double double_pendule_f(double x, double y, double u, double v, double l1, double l2, double m1, double m2, double g) {
    double Dtetha = y - x;
    return (u * u * m2 * l1 * cos(Dtetha) * sin(Dtetha) + v * v * m2 * l2 * sin(Dtetha) - (m1 + m2) * g * sin(x) + m2 * g * cos(Dtetha) * sin(y)) / ((m1 + m2) * l1 - m2 * l1 * cos(Dtetha) * cos(Dtetha));
}

double double_pendule_g(double x, double y, double u, double v, double l1, double l2, double m1, double m2, double g) {
    double Dtetha = y - x;
    return (-v * v * m2 * l2 * cos(Dtetha) * sin(Dtetha) + (m1 + m2) * (g * sin(x) * cos(Dtetha) - l1 * u * u * sin(Dtetha) - g * sin(y))) / ((m1 + m2) * l2 - m2 * l2 * cos(Dtetha) * cos(Dtetha));
}

//calcule une ittération
void Maestro::calcule_temp_plus_1() {
    if (nb_pendule == 2) {
        //les grandeurs utiles
        double x = list_pendule[0]->theta();
        double y = list_pendule[1]->theta();
        double xp = list_pendule[0]->omega();
        double yp = list_pendule[1]->omega();
        double m1 = list_pendule[0]->m();
        double m2 = list_pendule[1]->m();
        double l1 = list_pendule[0]->r();
        double l2 = list_pendule[1]->r();

        double u = xp;
        double v = yp;

        double kx1 = xp * pas;
        double ky1 = yp * pas;
        double ku1 = pas * double_pendule_f( x, y, u, v, l1, l2, m1, m2, grav);
        double kv1 = pas * double_pendule_g( x, y, u, v, l1, l2, m1, m2, grav);

        double kx2 = (u + 1/2 * ku1) * pas;
        double ky2 = (v + 1/2 * kv1) * pas;
        double ku2 = pas * double_pendule_f(x + kx1 /2, y + ky1/2, u + ku1/2, v + kv1 / 2, l1, l2, m1, m2, grav);
        double kv2 = pas * double_pendule_g(x + kx1 /2, y + ky1/2, u + ku1/2, v + kv1 / 2, l1, l2, m1, m2, grav);

        double kx3 = (u + 1/2 * ku2) * pas;
        double ky3 = (v + 1/2 * kv2) * pas;
        double ku3 = pas * double_pendule_f(x + kx2 /2, y + ky2/2, u + ku2/2, v + kv2 / 2, l1, l2, m1, m2, grav);
        double kv3 = pas * double_pendule_g(x + kx2 /2, y + ky2/2, u + ku2/2, v + kv2 / 2, l1, l2, m1, m2, grav);

        double kx4 = (u + 1/2 * ku3) * pas;
        double ky4 = (v + 1/2 * kv3) * pas;
        double ku4 = pas * double_pendule_f(x + kx3 /2, y + ky3/2, u + ku3/2, v + kv3 / 2, l1, l2, m1, m2, grav);
        double kv4 = pas * double_pendule_g(x + kx3 /2, y + ky3/2, u + ku3/2, v + kv3 / 2, l1, l2, m1, m2, grav);

        this->list_pendule[0]->theta(x + 1/6 * (kx1 + 2 * kx2 + 2 * kx3 + kx4));
        this->list_pendule[1]->theta(y + 1/6 * (ky1 + 2 * ky2 + 2 * ky3 + ky4));
        this->list_pendule[0]->omega(u + 1/6 * (ku1 + 2 * ku2 + 2 * ku3 + ku4));
        this->list_pendule[1]->omega(v + 1/6 * (kv1 + 2 * kv2 + 2 * kv3 + kv4));

        printf("%0.3g\n", x + 1 / 6 * (kx1 + 2 * kx2 + 2 * kx3 + kx4));
        printf("%0.3g\n", y + 1 / 6 * (ky1 + 2 * ky2 + 2 * ky3 + ky4));
        printf("%0.3g\n", u + 1 / 6 * (ku1 + 2 * ku2 + 2 * ku3 + ku4));
        printf("%0.3g\n\n", v + 1 / 6 * (kv1 + 2 * kv2 + 2 * kv3 + kv4));
    }

    this->temps += pas;
}

void Maestro::calcule_tmax() {
    if (temps >= tempsmax)
    {
        return;
    }
    FILE *savefile = fopen("./target/ballcoordo.csv", "w");
    if (savefile == NULL)
    {
        fprintf(stderr, "impossible de créé et ou d'ouvrir le fichier ./target/plot/ballcoordo.csv");
        exit(1);
    }

    for (double t = temps; temps < tempsmax; temps += pas) {
        fprintf(savefile, "%0.3g", temps);
        for (int i = 0; i < nb_pendule; i++){
            fprintf(savefile, "%0.3g;%0.3g;", list_pendule[i]->x(), list_pendule[i]->y());
        }
        fprintf(savefile, "\n");
        calcule_temp_plus_1();
    }
} 