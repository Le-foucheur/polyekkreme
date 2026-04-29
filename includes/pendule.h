#ifndef PENDULE
#define PENDULE

class Pendule {
    
public:
    Pendule();
    Pendule(int id, long double m, long double r, long double theta0, long double omega, Pendule* parent);
    Pendule(int id, long double m, long double r, long double theta0, long double omega);
    ~Pendule();

    //Les gets

    int id();
    long double x();
    long double y();
    long double r();
    long double theta();
    long double oldomega();
    long double omega();
    long double m();
    Pendule* attacher();
    
    //Les set
    void id(int id);
    void r(long double r);
    void theta(long double theta);
    void oldomega(long double theta);
    void omega(long double omega);
    void m(long double m);

    void attacher(Pendule* attache);
    void print();

private:

    int identifieur = 0.;
    long double th = 0.;
    long double longeur = 0.;
    long double vit_angl = 0.;
    long double oldvit = 0.;
    long double masse = 0.;
    Pendule* attache = NULL;
};


#endif