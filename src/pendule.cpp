#include <cmath>
#include <stdio.h>
#include "pendule.h"

/*  Les constructeur et desctruceur    */

Pendule::Pendule(){
    this->attache = NULL;
}

Pendule::~Pendule(){

}

Pendule::Pendule(int id, long double m, long double r, long double theta0, long double omega0, Pendule *parent){
    this->identifieur = id;
    this->masse = m;
    this->longeur = r;
    this->th = theta0;
    this->oldvit = omega0;
    this->vit_angl = omega0;
    this->attache = parent;
}

Pendule::Pendule(int id, long double m, long double r, long double theta0, long double omega0){
    this->identifieur = id;
    this->masse = m;
    this->longeur = r;
    this->th = theta0;
    this->oldvit = omega0;
    this->vit_angl = omega0;
}

/*      Gets    */

int Pendule::id() {
    return identifieur;
}

long double Pendule::r() {
    return longeur;
}

long double Pendule::theta() {
    return th;
}

long double Pendule::oldomega() {
    return oldvit;
}

long double Pendule::omega() {
    return vit_angl;
} 

long double Pendule::x() {
    long double tmp = longeur * sinl(th);
    if (attache == NULL) {
        return tmp;
    }
    else {
        return attache->x() + tmp;
    }
}

long double Pendule::y() {
    long double tmp = - longeur * cosl(th);
    if (attache == NULL) {
        return tmp;
    }
    else {
        return attache->y() + tmp;
    }
}

long double Pendule::m() {
    return masse;
}

Pendule* Pendule::attacher() {
    return attache;
}

/*      Set     */

void Pendule::id(int id) {
    this->identifieur = id;
}

void Pendule::r(long double r) {
    this->longeur = r;
}

void Pendule::theta(long double theta) {
    this->th = std::remainder(theta, 2 * M_PI);
}

void Pendule::oldomega(long double omega) {
    this->oldvit = omega;
}

void Pendule::omega(long double omega) {
    this->vit_angl = omega;
}

void Pendule::m(long double m) {
    this->masse = m;
}

/*  Les fonc random     */

void Pendule::attacher(Pendule* parent) {
    this->attache = parent;
}

void Pendule::print(){
    printf("id: %d\n", identifieur);
    printf("\tmasse: %0.3Lg\n", masse);
    printf("\tcoord cart: %0.3Lg, %0.3Lg\n", x(), y());
    printf("\tcoord pol: %0.3Lg, %0.3Lg\n", longeur, th - M_PI_2);
    printf("\tcoord nat: %0.3Lg, %0.3Lg\n", longeur, th);
    printf("\tvitesse angulaire: %0.3Lg\n", vit_angl);
    if (attache != NULL){
        printf("\tid attache: %d\n", attache->id());
    } else {
        printf("\tid attache: pas d’attache\n");
    }
}