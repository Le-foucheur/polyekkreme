#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include "maestro.h"
#include "pendule.h"

Maestro::Maestro() {
    this->list_pendule = (Pendule**)malloc(sizeof(Pendule*));
}

Maestro::~Maestro() {
    //free(list_pendule);
}

Maestro::Maestro(int nbmax, double dt, double tmax, double gravity) {
    this->nb_max_pendule = nbmax;
    this->nb_pendule = 0;
    this->temps = 0;
    this->pas = dt;
    this->tempsmax = tmax;
    this->grav = gravity;
    this->list_pendule = (Pendule**)malloc(sizeof(Pendule*) * nbmax);
    
    double tmpltot = 0;
    double tmplmax = 0;
    for (int i = 0; i < nb_pendule; i++) {
        double tmp = list_pendule[i]->r();
        tmpltot += tmp;
        tmplmax = max(tmp, tmplmax);
    }
    this->lmax = tmplmax;
    this->ltot = tmpltot;
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

double Maestro::l_max() {
    return lmax;
}

double Maestro::l_totale() {
    return ltot;
}

double Maestro::t() {
    return temps;
}
int Maestro::nb_p() {
    return nb_pendule;
}
int Maestro::nb_pmax() {
    return nb_max_pendule;
}

/*      Set     */

void Maestro::add_pendule(Pendule* pendule) {
    if (nb_pendule >= nb_max_pendule) {
        fprintf(stderr, "impossible d’ajouter des pendules: %d / %d\n", nb_pendule, nb_max_pendule);
        exit(1);
    }
    this->list_pendule[nb_pendule] = pendule;
    this->nb_pendule += 1;
    this->ltot = ltot + pendule->r();
    this->lmax = max (lmax, pendule->r());
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

double n_pendule(int i, double* x, double* v, double* l, double g, Maestro m) {
    double tmp1 = 0;
    double tmp2 = 0;
    for (int j = 0; j < i; j++){
        double Dtheta_avant = x[i] - x[j];
        double acc_avant = (v[j] - m.get_pendule()[j]->oldomega()) / m.dt();

        tmp1 += acc_avant * cos(Dtheta_avant) + v[j] * v[j] * sin(Dtheta_avant);
    }

    for (int j = i + 1; j < m.nb_p(); j++){
        double Dtheta_apres = x[j] - x[i];
        double acc_apres = (v[j] - m.get_pendule()[j]->oldomega()) / m.dt();

        
        tmp2 +=  (m.nb_p() + 1 - j) * ( acc_apres * cos(Dtheta_apres) - v[j] * v[j] * sin(Dtheta_apres) );
    }
    
    return - g / l[i] * sin(x[i]) - tmp1 - tmp2 / (m.nb_p() + 1 - i);
}

// Fonction qui calcule toutes les accélérations d'un seul coup via le formalisme matriciel Gémini
void calculer_accelerations_npendule(int n, double* x, double* v, double* l, double* m, double g, double* acc_out) {
    // 1. Calcul des masses cumulées (mu)
    double* mu = (double*)malloc(n * sizeof(double));
    mu[n - 1] = m[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        mu[i] = m[i] + mu[i + 1];
    }
    
    // 2. Allocation Matrice A et Vecteur B
    double** A = (double**)malloc(n * sizeof(double*));
    for (int i = 0; i < n; i++) A[i] = (double*)malloc(n * sizeof(double));
    double* B = (double*)malloc(n * sizeof(double));
    
    // 3. Remplissage du système (A * acc = B)
    for (int i = 0; i < n; i++) {
        B[i] = -mu[i] * g * l[i] * sin(x[i]); // Gravité
        
        for (int j = 0; j < n; j++) {
            int max_ij = (i > j) ? i : j;
            
            // Matrice d'inertie (A)
            A[i][j] = mu[max_ij] * l[i] * l[j] * cos(x[i] - x[j]);
            
            // Forces centrifuges et Coriolis (Vecteur B)
            B[i] -= mu[max_ij] * l[i] * l[j] * v[j] * v[j] * sin(x[i] - x[j]);
        }
    }
    
    // 4. Résolution de A * acc_out = B (Pivot de Gauss partiel)
    for (int i = 0; i < n; i++) {
        int max_row = i;
        for (int k = i + 1; k < n; k++) {
            if (std::abs(A[k][i]) > std::abs(A[max_row][i])) max_row = k;
        }
        
        // Swap des lignes
        double* tmp_row = A[i]; A[i] = A[max_row]; A[max_row] = tmp_row;
        double tmp_b = B[i]; B[i] = B[max_row]; B[max_row] = tmp_b;
        
        // Élimination
        for (int k = i + 1; k < n; k++) {
            double factor = A[k][i] / A[i][i];
            B[k] -= factor * B[i];
            for (int j = i; j < n; j++) A[k][j] -= factor * A[i][j];
        }
    }
    
    // Remontée
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0;
        for (int j = i + 1; j < n; j++) sum += A[i][j] * acc_out[j];
        acc_out[i] = (B[i] - sum) / A[i][i];
    }
    
    // 5. Nettoyage de la mémoire
    for (int i = 0; i < n; i++) free(A[i]);
    free(A);
    free(B);
    free(mu);
}

//calcule une ittération
void Maestro::calcule_temp_plus_1() {
    if (nb_pendule == 2) {
        //les grandeurs utiles
        double x = list_pendule[0]->theta();
        double y = list_pendule[1]->theta();
        double u = list_pendule[0]->omega();
        double v = list_pendule[1]->omega();
        double m1 = list_pendule[0]->m();
        double m2 = list_pendule[1]->m();
        double l1 = list_pendule[0]->r();
        double l2 = list_pendule[1]->r();

        double kx1 = u * pas;
        double ky1 = v * pas;
        double ku1 = pas * double_pendule_f( x, y, u, v, l1, l2, m1, m2, grav);
        double kv1 = pas * double_pendule_g( x, y, u, v, l1, l2, m1, m2, grav);

        double kx2 = (u + 1./2. * ku1) * pas;
        double ky2 = (v + 1./2. * kv1) * pas;
        double ku2 = pas * double_pendule_f(x + kx1 /2., y + ky1/2., u + ku1/2., v + kv1 / 2., l1, l2, m1, m2, grav);
        double kv2 = pas * double_pendule_g(x + kx1 /2., y + ky1/2., u + ku1/2., v + kv1 / 2., l1, l2, m1, m2, grav);

        double kx3 = (u + 1./2. * ku2) * pas;
        double ky3 = (v + 1./2. * kv2) * pas;
        double ku3 = pas * double_pendule_f(x + kx2 /2., y + ky2/2., u + ku2/2., v + kv2 / 2., l1, l2, m1, m2, grav);
        double kv3 = pas * double_pendule_g(x + kx2 /2., y + ky2/2., u + ku2/2., v + kv2 / 2., l1, l2, m1, m2, grav);

        double kx4 = (u + ku3) * pas;
        double ky4 = (v + kv3) * pas;
        double ku4 = pas * double_pendule_f(x + kx3 , y + ky3, u + ku3, v + kv3, l1, l2, m1, m2, grav);
        double kv4 = pas * double_pendule_g(x + kx3 , y + ky3, u + ku3, v + kv3, l1, l2, m1, m2, grav);

        this->list_pendule[0]->theta(x + 1./6. * (kx1 + 2 * kx2 + 2 * kx3 + kx4));
        this->list_pendule[1]->theta(y + 1./6. * (ky1 + 2 * ky2 + 2 * ky3 + ky4));
        this->list_pendule[0]->omega(u + 1./6. * (ku1 + 2 * ku2 + 2 * ku3 + ku4));
        this->list_pendule[1]->omega(v + 1./6. * (kv1 + 2 * kv2 + 2 * kv3 + kv4));

//    } else if (nb_pendule > 2 || nb_pendule == 1) {
//
//        double* x = (double *)malloc(sizeof(double) * nb_pendule);
//        double* v = (double *)malloc(sizeof(double) * nb_pendule);
//        double* l = (double *)malloc(sizeof(double) * nb_pendule);
//        double* m = (double *)malloc(sizeof(double) * nb_pendule);
//        
//        double* kx1 = (double *)malloc(sizeof(double) * nb_pendule);
//        double* kv1 = (double *)malloc(sizeof(double) * nb_pendule);
//        
//        double* kx2 = (double *)malloc(sizeof(double) * nb_pendule);
//        double* kv2 = (double *)malloc(sizeof(double) * nb_pendule);
//        
//        double* kx3 = (double *)malloc(sizeof(double) * nb_pendule);
//        double* kv3 = (double *)malloc(sizeof(double) * nb_pendule);
//        
//        double* kx4 = (double *)malloc(sizeof(double) * nb_pendule);
//        double* kv4 = (double *)malloc(sizeof(double) * nb_pendule);
//        
//        //init vecteur
//        for (int i = 0; i < nb_pendule; i++) {
//            x[i] = list_pendule[i]->theta();
//            v[i] = list_pendule[i]->omega();
//            l[i] = list_pendule[i]->r();
//            m[i] = list_pendule[i]->m();
//        }
//        
//        //calc k1
//        for (int i = 0; i < nb_pendule; i++) {
//            kx1[i] = v[i] * pas;
//            kv1[i] = pas * n_pendule(i, x, v, l, grav, *this);
//        }
//        
//        //calc k2
//        double* tmpx = (double*)malloc(sizeof(double) * nb_pendule);
//        double* tmpv = (double*)malloc(sizeof(double) * nb_pendule);
//        for (int i = 0; i < nb_pendule; i++){
//            tmpx[i] = x[i] + kx1[i] / 2.;
//            tmpv[i] = v[i] + kv1[i] / 2.;
//        }
//        for (int i = 0; i < nb_pendule; i++){
//            kx2[i] = pas * tmpv[i];
//            kv2[i] = pas * n_pendule(i, tmpx, tmpv, l, grav, *this);
//        }
//
//        //calc k3
//        double* tmp1x = (double*)malloc(sizeof(double) * nb_pendule);
//        double* tmp1v = (double*)malloc(sizeof(double) * nb_pendule);
//        for (int i = 0; i < nb_pendule; i++){
//            tmp1x[i] = x[i] + kx2[i] / 2.;
//            tmp1v[i] = v[i] + kv2[i] / 2.;
//        }
//        for (int i = 0; i < nb_pendule; i++){
//            kx3[i] = pas * tmp1v[i];
//            kv3[i] = pas * n_pendule(i, tmp1x, tmp1v, l, grav, *this);
//        }
//
//        //calc k4
//        double* tmp2x = (double*)malloc(sizeof(double) * nb_pendule);
//        double* tmp2v = (double*)malloc(sizeof(double) * nb_pendule);
//        for (int i = 0; i < nb_pendule; i++){
//            tmp2x[i] = x[i] + kx3[i];
//            tmp2v[i] = v[i] + kv3[i];
//        }
//        for (int i = 0; i < nb_pendule; i++){
//            kx4[i] = pas * tmp2v[i];
//            kv4[i] = pas * n_pendule(i, tmp2x, tmp2v, l, grav, *this);
//        }
//
//        //update des coordonées et des vitesses
//        for (int i = 0; i < nb_pendule; i++){
//            this->list_pendule[i]->theta(x[i] + 1. / 6. * (kx1[i] + 2. * kx2[i] + 2. * kx3[i] + kx4[i]));
//            this->list_pendule[i]->oldomega(v[i]);
//            this->list_pendule[i]->omega(v[i] + 1. / 6. * (kv1[i] + 2. * kv2[i] + 2. * kv3[i] + kv4[i]));
//        }
//        free(x);
//        free(v);
//        free(l);
//        free(m);
//        
//        free(kx1);
//        free(kv1);
//        
//        free(kx2);
//        free(kv2);
//        
//        free(kx3);
//        free(kv3);
//        
//        free(kx4);
//        free(kv4);
//        
//        free(tmpv);
//        free(tmpx);
//        
//        free(tmp1v);
//        free(tmp1x);
//        
//        free(tmp2v);
//        free(tmp2x);

//    } else if (nb_pendule > 2 || nb_pendule == 1) {
//
//        double tmp2 = 0;
//        if(nb_pendule > 1 ){
//            tmp2 = list_pendule[1]->omega() * cos(list_pendule[1]->theta() - list_pendule[0]->theta());
//        }
//        double tmp = list_pendule[0]->theta() + pas * (list_pendule[0]->omega() + tmp2 - grav / list_pendule[0]->r() * sin(list_pendule[0]->theta()) );
//
//        this->list_pendule[0]->omega((tmp - list_pendule[0]->theta()) / pas);
//        this->list_pendule[0]->theta(tmp);
//
//        for (int i = 1; i < nb_pendule - 1; i++) {
//
//            double tmp1 = list_pendule[i]->theta() + pas * (list_pendule[i]->omega() + list_pendule[i - 1]->omega() * cos(list_pendule[i]->theta() - list_pendule[i - 1]->theta()) + list_pendule[i + 1]->omega() * cos(list_pendule[i + 1]->theta() - list_pendule[i]->theta()) - grav / list_pendule[i]->r() * sin(list_pendule[i]->theta()) );
//
//            this->list_pendule[i]->omega((tmp1 - list_pendule[i]->theta()) / pas);
//            this->list_pendule[i]->theta(tmp1);
//        }
//        if(nb_pendule != 1){
//            double tmp1 = list_pendule[nb_pendule - 1]->theta() + pas * (list_pendule[nb_pendule - 1]->omega() + list_pendule[nb_pendule - 2]->omega() * cos(list_pendule[nb_pendule - 1]->theta() - list_pendule[nb_pendule - 2]->theta()) - grav / list_pendule[nb_pendule - 1]->r() * sin(list_pendule[nb_pendule - 1]->theta()) );
//    
//            this->list_pendule[nb_pendule - 1]->omega((tmp1 - list_pendule[nb_pendule - 1]->theta()) / pas);
//            this->list_pendule[nb_pendule - 1]->theta(tmp1);
//        }

        //Gémini
    } else if (nb_pendule > 2 || nb_pendule == 1) {

        double* x = (double *)malloc(sizeof(double) * nb_pendule);
        double* v = (double *)malloc(sizeof(double) * nb_pendule);
        double* l = (double *)malloc(sizeof(double) * nb_pendule);
        double* m = (double *)malloc(sizeof(double) * nb_pendule);
        
        double* kx1 = (double *)malloc(sizeof(double) * nb_pendule);
        double* kv1 = (double *)malloc(sizeof(double) * nb_pendule);
        
        double* kx2 = (double *)malloc(sizeof(double) * nb_pendule);
        double* kv2 = (double *)malloc(sizeof(double) * nb_pendule);
        
        double* kx3 = (double *)malloc(sizeof(double) * nb_pendule);
        double* kv3 = (double *)malloc(sizeof(double) * nb_pendule);
        
        double* kx4 = (double *)malloc(sizeof(double) * nb_pendule);
        double* kv4 = (double *)malloc(sizeof(double) * nb_pendule);

        //init vecteur
        for (int i = 0; i < nb_pendule; i++) {
            x[i] = list_pendule[i]->theta();
            v[i] = list_pendule[i]->omega();
            l[i] = list_pendule[i]->r();
            m[i] = list_pendule[i]->m();
        }
        
        // NOUVEAU : tableau temporaire pour récupérer les accélérations matricielles
        double* acc_tmp = (double*)malloc(sizeof(double) * nb_pendule);
        
        //calc k1
        calculer_accelerations_npendule(nb_pendule, x, v, l, m, grav, acc_tmp);
        for (int i = 0; i < nb_pendule; i++) {
            kx1[i] = pas * v[i];
            kv1[i] = pas * acc_tmp[i];
        }
        
        //calc k2
        double* tmpx = (double*)malloc(sizeof(double) * nb_pendule);
        double* tmpv = (double*)malloc(sizeof(double) * nb_pendule);
        for (int i = 0; i < nb_pendule; i++){
            tmpx[i] = x[i] + kx1[i] / 2.;
            tmpv[i] = v[i] + kv1[i] / 2.;
        }
        calculer_accelerations_npendule(nb_pendule, tmpx, tmpv, l, m, grav, acc_tmp);
        for (int i = 0; i < nb_pendule; i++){
            kx2[i] = pas * tmpv[i];
            kv2[i] = pas * acc_tmp[i];
        }

        //calc k3
        double* tmp1x = (double*)malloc(sizeof(double) * nb_pendule);
        double* tmp1v = (double*)malloc(sizeof(double) * nb_pendule);
        for (int i = 0; i < nb_pendule; i++){
            tmp1x[i] = x[i] + kx2[i] / 2.;
            tmp1v[i] = v[i] + kv2[i] / 2.;
        }
        calculer_accelerations_npendule(nb_pendule, tmp1x, tmp1v, l, m, grav, acc_tmp);
        for (int i = 0; i < nb_pendule; i++){
            kx3[i] = pas * tmp1v[i];
            kv3[i] = pas * acc_tmp[i];
        }

        //calc k4
        double* tmp2x = (double*)malloc(sizeof(double) * nb_pendule);
        double* tmp2v = (double*)malloc(sizeof(double) * nb_pendule);
        for (int i = 0; i < nb_pendule; i++){
            tmp2x[i] = x[i] + kx3[i];
            tmp2v[i] = v[i] + kv3[i];
        }
        calculer_accelerations_npendule(nb_pendule, tmp2x, tmp2v, l, m, grav, acc_tmp);
        for (int i = 0; i < nb_pendule; i++){
            kx4[i] = pas * tmp2v[i];
            kv4[i] = pas * acc_tmp[i];
        }

        //update des coordonées et des vitesses
        for (int i = 0; i < nb_pendule; i++){
            this->list_pendule[i]->theta(x[i] + 1. / 6. * (kx1[i] + 2. * kx2[i] + 2. * kx3[i] + kx4[i]));
            this->list_pendule[i]->oldomega(v[i]);
            this->list_pendule[i]->omega(v[i] + 1. / 6. * (kv1[i] + 2. * kv2[i] + 2. * kv3[i] + kv4[i]));
        }
        
        // ... (garde tes free() existants et n'oublie pas d'ajouter celui-ci) :
        free(acc_tmp);
        free(x);
        free(v);
        free(l);
        free(m);
        
        free(kx1);
        free(kv1);
        
        free(kx2);
        free(kv2);
        
        free(kx3);
        free(kv3);
        
        free(kx4);
        free(kv4);
        
        free(tmpv);
        free(tmpx);
        
        free(tmp1v);
        free(tmp1x);
        
        free(tmp2v);
        free(tmp2x);


    } else {
        printf("pas de methode pour %d pendule\n", nb_pendule);
        this->temps += tempsmax;
        return;
    }

    this->temps += pas;
}

void Maestro::calcule_tmax() {
    if (temps >= tempsmax)
    {
        return;
    }
    FILE *savefile = fopen("./target/pendule_coordonee.csv", "w");
    if (savefile == NULL)
    {
        fprintf(stderr, "impossible de créé et ou d'ouvrir le fichier ./target/plot/pendule_coordonee.csv");
        exit(1);
    }
    FILE *portraifile = fopen("./target/pendule_portrait.csv", "w");
    if (portraifile == NULL)
    {
        fprintf(stderr, "impossible de créé et ou d'ouvrir le fichier ./target/plot/pendule_portrait.csv");
        exit(1);
    }
    FILE *energiefile = fopen("./target/pendule_energie.csv", "w");
    if (energiefile == NULL)
    {
        fprintf(stderr, "impossible de créé et ou d'ouvrir le fichier ./target/plot/pendule_energie.csv");
        exit(1);
    }
    fprintf(savefile, "t;");
    for (int i = 0; i <= nb_pendule / 2; i++) {
        fprintf(savefile, "x%d;y%d;", i, i);
        fprintf(portraifile, "theta%d;omega%d;", i, i);
    }
    fprintf(energiefile, "t;Ep;Ec;Em;");
    fprintf(savefile, "\n");
    fprintf(portraifile, "\n");
    fprintf(energiefile, "\n");

    for (; temps <= tempsmax;) {
        fprintf(savefile, "%0.3g;", temps);
        if (nb_pendule == 2 ){
            fprintf(energiefile, "%0.3g;", temps);

            double m1 = list_pendule[0]->m();
            double l1 = list_pendule[0]->r();
            double o1 = list_pendule[0]->omega();
            double m2 = list_pendule[1]->m();
            double l2 = list_pendule[1]->r();
            double o2 = list_pendule[1]->omega();
            double D = list_pendule[0]->theta() - list_pendule[1]->theta();

            double Ec = 1. / 2. * m1 * l1 * l1 * o1 * o1 + 1./2. * m2 * (l1 * l1 * o1 * o1 + l2 * l2 * o2 * o2 + 2 * l1 * l2 * o1 * o2 * cos(D));
            double Ep = grav * (m1 * list_pendule[0]->y() + m2 * list_pendule[1]->y());
            double Em = Ec + Ep;
            fprintf(energiefile, "%0.3g;%0.3g;%0.3g;", Ep, Ec, Em);
        }
        for (int i = 0; i < nb_pendule; i++){
            fprintf(savefile, "%0.3g;%0.3g;", list_pendule[i]->x(), list_pendule[i]->y());
            fprintf(portraifile, "%0.3g;%0.3g;", list_pendule[i]->theta(), list_pendule[i]->omega());
            
        }
        fprintf(savefile, "\n");
        fprintf(portraifile, "\n");
        fprintf(energiefile, "\n");
        calcule_temp_plus_1();
    }
    fclose(savefile);
    fclose(portraifile);
    fclose(energiefile);
} 

void Maestro::init_save() {
    FILE *savefile = fopen("./target/data/pendule_coordonee.csv", "w");
    if (savefile == NULL)
    {
        fprintf(stderr, "impossible de créé et ou d'ouvrir le fichier ./target/plot/pendule_coordonee.csv");
        exit(1);
    }
    FILE *portraifile = fopen("./target/data/pendule_portrait.csv", "w");
    if (portraifile == NULL)
    {
        fprintf(stderr, "impossible de créé et ou d'ouvrir le fichier ./target/plot/pendule_portrait.csv");
        exit(1);
    }
    FILE *energiefile = fopen("./target/data/pendule_energie.csv", "w");
    if (energiefile == NULL)
    {
        fprintf(stderr, "impossible de créé et ou d'ouvrir le fichier ./target/plot/pendule_energie.csv");
        exit(1);
    }
    fprintf(savefile, "t;");
    for (int i = 0; i <= nb_pendule; i++)
    {
        fprintf(savefile, "x%d;y%d;", i, i);
        fprintf(portraifile, "theta%d;omega%d;", i, i);
    }
    fprintf(energiefile, "t;Ep;Ec;Em;");
    fprintf(savefile, "\n");
    fprintf(portraifile, "\n");
    fprintf(energiefile, "\n");

    fclose(savefile);
    fclose(portraifile);
    fclose(energiefile);
}

void Maestro::save() {
    FILE *savefile = fopen("./target/data/pendule_coordonee.csv", "a");
    if (savefile == NULL)
    {
        fprintf(stderr, "impossible de créé et ou d'ouvrir le fichier ./target/plot/pendule_coordonee.csv");
        exit(1);
    }
    FILE *portraifile = fopen("./target/data/pendule_portrait.csv", "a");
    if (portraifile == NULL)
    {
        fprintf(stderr, "impossible de créé et ou d'ouvrir le fichier ./target/plot/pendule_portrait.csv");
        exit(1);
    }
    FILE *energiefile = fopen("./target/data/pendule_energie.csv", "a");
    if (energiefile == NULL)
    {
        fprintf(stderr, "impossible de créé et ou d'ouvrir le fichier ./target/plot/pendule_energie.csv");
        exit(1);
    }

    fprintf(savefile, "%0.3g;", temps);

    fprintf(energiefile, "%0.3g;", temps);

    double Ec = 0, Ep = 0;
    double x_curr = 0, y_curr = 0;
    double vx_curr = 0, vy_curr = 0;

    for (int i = 0; i < nb_pendule; i++) {
        double th = list_pendule[i]->theta();
        double om = list_pendule[i]->omega();
        double l  = list_pendule[i]->r();
        double mass = list_pendule[i]->m();

        x_curr += l * sin(th);
        y_curr -= l * cos(th);
        vx_curr += l * om * cos(th);
        vy_curr += l * om * sin(th);

        Ep += mass * grav * y_curr;
        Ec += 0.5 * mass * (vx_curr * vx_curr + vy_curr * vy_curr);
    }

        double Em = Ec + Ep;
        fprintf(energiefile, "%0.3g;%0.3g;%0.3g;", Ep, Ec, Em);
    
    for (int i = 0; i < nb_pendule; i++){
        fprintf(savefile, "%0.3g;%0.3g;", list_pendule[i]->x(), list_pendule[i]->y());
        fprintf(portraifile, "%0.3g;%0.3g;", list_pendule[i]->theta(), list_pendule[i]->omega());
        
    }
    fprintf(savefile, "\n");
    fprintf(portraifile, "\n");
    fprintf(energiefile, "\n");

    fclose(savefile);
    fclose(portraifile);
    fclose(energiefile);
} 