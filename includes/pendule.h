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
    double oldtheta();
    double omega();
    double m();
    Pendule* attacher();
    
    //Les set
    void id(int id);
    void r(double r);
    void theta(double theta);
    void oldtheta(double theta);
    void omega(double omega);
    void m(double m);

    void attacher(Pendule* attache);
    void print();

private:

    int identifieur;
    double th;
    double oldth;
    double longeur;
    double vit_angl;
    double masse;
    Pendule* attache;
};


#endif