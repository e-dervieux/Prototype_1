#ifndef PROTOTYPE_1_DEMOCOLLISION2_H
#define PROTOTYPE_1_DEMOCOLLISION2_H

#include "demoCohesion.h"

using MatriceDemoCollision = MatriceParticules<16,8,4>;

class Mur
{
public:
    Mur(int w, int h, double x = 0.0, double y = 0.0, double l = 1.0, Particule* part = NULL)
     : m_x(x), m_y(y), m_l(l), m_w(w), m_h(h), m_part(part),
       m_m({50,50,50,255}, 5000000000000000.0)
    {}

    void init()
    {
        if (m_part == NULL)
            return;

        // Placement des particules
        for(int i = 0 ; i < m_w ; i++)
        {
            double x = m_x + i*m_l;
            double y = m_y;
            m_part[i] = Particule((int)x, (int)y, x, y, &m_m);
        }
        for(int i = 0 ; i < m_h ; i++)
        {
            double x = m_x + m_l*m_w;
            double y = m_y + i*m_l;
            m_part[m_w+i] = Particule((int)x, (int)y, x, y, &m_m);
        }
        for(int i = 0 ; i < m_w ; i++)
        {
            double x = m_x + (i+1)*m_l;
            double y = m_y + m_l*m_h;
            m_part[m_w+m_h+i] = Particule((int)x, (int)y, x, y, &m_m);
        }
        for(int i = 0 ; i < m_h ; i++)
        {
            double x = m_x;
            double y = m_y + m_l*(i+1);
            m_part[2*m_w+m_h+i] = Particule((int)x, (int)y, x, y, &m_m);
        }

        // Liaisons
        for(int i = 1 ; i < m_w ; i++)
            m_part[i-1].lier(&m_part[i]);
        for(int i = 0 ; i < m_h ; i++)
            m_part[m_w+i-1].lier(&m_part[m_w+i]);
        for(int i = 1 ; i < m_w ; i++)
            m_part[m_w+m_h+i-1].lier(&m_part[m_w+m_h+i]);
        m_part[2*m_w+m_h-1].lier(&m_part[m_w+m_h-1]); // Coin inférieur droit
        for(int i = 1 ; i < m_h ; i++)
            m_part[2*m_w+m_h+i-1].lier(&m_part[2*m_w+m_h+i]);
        m_part[2*(m_w+m_h)-1].lier(&m_part[m_w+m_h]); // Coin inférieur gauche
        m_part[2*m_w+m_h].lier(&m_part[0]);
    }

    inline int nbPart() {return 2*(m_w+m_h);}
private:
    double m_x, m_y;
    double m_l;
    int m_w, m_h;
    Particule* m_part;
    Matiere m_m;
};

class SceneDemoCol2 : public SceneSDL
{
public:
    SceneDemoCol2(MatriceDemoCollision& mat, Mur& mur, Particule* part)
     : SceneSDL(mat), m_mur(mur), m_part(part)
    {
        m_j1 = new JambonCarre(&m_m1, part, 2.0);
        m_j2 = new JambonCarre(&m_m2, part+JambonCarre::nbPart(), 2.0);
    }

    ~SceneDemoCol2()
    {
        delete m_j1;
        delete m_j2;
    }

    virtual void actionClavier(bool& continuer, bool& recommencer)
    {
        SceneSDL::actionClavier(continuer, recommencer);

        if (m_clavier[def::K_SHIFT])
            def::delaiEntreFrames = 100;
        else
            def::delaiEntreFrames = 0;
    }

    virtual void charger(int config)
    {
        double k = 100.0;
        double cc = 5.0;
        double v = 10.0;
        Matiere m1,m2;

        switch(config)
        {
            default:
                break;
        }

        m_m1 = Matiere({255,0,0,255}, 1.0, 2.0, k, cc);
        m_m2 = Matiere({0,0,255,255}, 1.0, 2.0, k, cc);

        for(int i = 0 ; i < 2*JambonHexa::nbPart() + Mur(80,40).nbPart() ; i++)
            m_part[i].setPosInt(Vecteur(-1.0,-1.0));

        //m_part[0] = Particule(50,50,&m_m1);
        //m_part[0].appliquerDV(Vecteur(25.0,0.0));
        m_j1->init(Vecteur(30.0, 30.0));
        m_j1->appliquerDV(Vecteur(v, 0.0));
        m_j2->init(Vecteur(80.0,30.0));
        m_j2->appliquerDV(Vecteur(-v, 0.0));
        m_mur.init();
        m_element.reinit();
    }

private:
    Jambon* m_j1;
    Jambon* m_j2;
    Mur& m_mur;

    Particule* m_part;
    Matiere m_m1, m_m2;
};

void demoCollision2()
{
    double L = 2.0;

    def::redefGrille(180,100,5.0,0,2,3,true,true,8,16);
    def::redefTemp(true, 0.03, 0);
    def::nbIterationsEuler = 4;

    // Création des particules
    int nbPart = 2*JambonHexa::nbPart() + Mur(80,40).nbPart();
    Particule* particules = new Particule[nbPart];

    Mur mur(80, 40, 12.0, 12.0, 2.0, particules+JambonHexa::nbPart()+JambonCarre::nbPart());

    // Création de la grille
    MatriceDemoCollision mat(180, 100, 2, particules, nbPart);

    // Lancement de la scène SDL
    SceneDemoCol2 scene(mat, mur, particules);
    try { scene.bouclePrincipale(); }
    catch(Erreur& e) { std::cout << "Erreur !!" << std::endl << e.what() << std::endl; }

    delete[] particules;
}
#endif //PROTOTYPE_1_DEMOCOLLISION2_H
