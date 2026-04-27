#ifndef PENDULE
#define PENDULE

class Pendule {
    
public:
    Pendule();
    Pendule(int id, double m, double r, double theta0, double omega, Pendule* parent);
    Pendule(int id, double m, double r, double theta0, double omega);
    ~Pendule();

    //Les gets

    int id();
    double x();
    double y();
    double r();
    double theta();
    double oldomega();
    double omega();
    double m();
    Pendule* attacher();
    
    //Les set
    void id(int id);
    void r(double r);
    void theta(double theta);
    void oldomega(double theta);
    void omega(double omega);
    void m(double m);

    void attacher(Pendule* attache);
    void print();

private:

    int identifieur = 0.;
    double th = 0.;
    double longeur = 0.;
    double vit_angl = 0.;
    double oldvit = 0.;
    double masse = 0.;
    Pendule* attache = NULL;
};


#endif