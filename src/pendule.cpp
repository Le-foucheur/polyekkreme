#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include "pendule.h"

/*  Les constructeur et desctruceur    */

Pendule::Pendule(){
    this->attache = NULL;
}

Pendule::~Pendule(){

}

Pendule::Pendule(int id, double m, double r, double theta0, double omega0, Pendule *parent){
    this->identifieur = id;
    this->masse = m;
    this->longeur = r;
    this->th = theta0;
    this->vit_angl = omega0;
    this->attache = parent;
}

Pendule::Pendule(int id, double m, double r, double theta0, double omega0){
    this->identifieur = id;
    this->masse = m;
    this->longeur = r;
    this->th = theta0;
    this->vit_angl = omega0;
    this->attache = NULL;
}

/*      Gets    */

int Pendule::id() {
    return identifieur;
}

double Pendule::r() {
    return longeur;
}

double Pendule::theta() {
    return th;
}

double Pendule::omega() {
    return vit_angl;
} 

double Pendule::x() {
    double tmp = longeur * sin(th);
    if (attache == NULL) {
        return tmp;
    }
    else {
        return attache->x() + tmp;
    }
}

double Pendule::y() {
    double tmp = - longeur * cos(th);
    if (attache == NULL) {
        return tmp;
    }
    else {
        return attache->y() + tmp;
    }
}

double Pendule::m() {
    return masse;
}

/*      Set     */

void Pendule::id(int id) {
    this->identifieur = id;
}

void Pendule::r(double r) {
    this->longeur = r;
}

void Pendule::theta(double theta) {
    this->th = theta;
}

void Pendule::omega(double omega) {
    this->vit_angl = omega;
}

void Pendule::m(double m) {
    this->masse = m;
}

/*  Les fonc random     */

void Pendule::attacher(Pendule* parent) {
    this->attache = parent;
}

void Pendule::print(){
    printf("id: %d\n", identifieur);
    printf("\tmasse: %0.3g\n", masse);
    printf("\tcoord cart: %0.3g, %0.3g\n", x(), y());
    printf("\tcoord pol: %0.3g, %0.3g\n", longeur, th - M_PI_2);
    printf("\tcoord nat: %0.3g, %0.3g\n", longeur, th);
    printf("\tvitesse angulaire: %0.3g\n", vit_angl);
    if (attache != NULL){
        printf("\tid attache: %d\n", attache->id());
    } else {
        printf("\tid attache: pas d’attache\n");
    }
}