#ifndef PROTOTYPE_1_DEMOCOLLISION2_H
#define PROTOTYPE_1_DEMOCOLLISION2_H

#include "demoCohesion.h"

using MatriceDemoCollision = MatriceParticules<16,8,4>;

class Mur
{
public:
    Mur(int w, int h, double x = 0.0, double y = 0.0, double l = 1.0, Particule* part = NULL)
     : m_x(x), m_y(y), m_l(l), m_w(w), m_h(h), m_part(part),
       m_m({50,50,50,255}, 500000000000000000000.0)
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
            m_part[i] = Particule(Vecteur(x,y), &m_m, 3);
        }
        for(int i = 0 ; i < m_h ; i++)
        {
            double x = m_x + m_l*m_w;
            double y = m_y + i*m_l;
            m_part[m_w+i] = Particule(Vecteur(x,y), &m_m, 3);
        }
        for(int i = 0 ; i < m_w ; i++)
        {
            double x = m_x + (i+1)*m_l;
            double y = m_y + m_l*m_h;
            m_part[m_w+m_h+i] = Particule(Vecteur(x,y), &m_m, 3);
        }
        for(int i = 0 ; i < m_h ; i++)
        {
            double x = m_x;
            double y = m_y + m_l*(i+1);
            m_part[2*m_w+m_h+i] = Particule(Vecteur(x,y), &m_m, 3);
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
        m_j2 = new JambonHexa(&m_m2, part+JambonHexa::nbPart(), 2.0);
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
        double k = 175.0;
        double cc = 8.0;
        double v = 10.0;

        // Réinitialisation des particules
        for(int i = 0 ; i < 2*JambonHexa::nbPart() + Mur(80,40).nbPart() ; i++)
            m_part[i].setPosInt(Vecteur(-1.0,-1.0));

        // Initialisation
        switch(config)
        {
            case 5:
                m_m1 = Matiere({255,0,0,255}, 1.0, 2.0, k, cc);
                m_m2 = Matiere({0,0,255,255}, 1.0, 2.0, k, cc);

                m_j1->init(Vecteur(30.0, 30.0));
                m_j1->appliquerDV(Vecteur(v, 0.0));

                m_j2->init(Vecteur(100.0,30.0));
                m_j2->appliquerDV(Vecteur(-v, 0.0));

                m_titre = "2 formes entrant en collision... ;_;";
                break;

            case 4:
                m_m2 = Matiere({255,0,0,255}, 1.0, 2.0, k, cc);

                m_j2->init(Vecteur(35.0,40.0));
                m_j2->appliquerDV(Vecteur(-15.0,0.0));

                m_titre = "Maille hexagonale contre un mur...";
                break;

            case 3:
                m_m1 = Matiere({255,0,0,255}, 1.0, 2.0, k, cc);

                m_j1->init(Vecteur(35.0,40.0));
                m_j1->appliquerDV(Vecteur(-v,0.0));

                m_titre = "Maille carree contre un mur";
                break;

            case 2:
                m_m1 = Matiere({255,0,0,255}, 1.0, 2.0, k, cc);
                m_m2 = Matiere({0,0,255,255}, 1.0, 2.0, k, cc);

                m_part[0] = Particule(50,50, &m_m1);
                m_part[0].appliquerDV(Vecteur(35.0,15.0));

                m_part[1] = Particule(105,65, &m_m2);
                m_part[1].appliquerDV(Vecteur(-20.0,0.0));

                m_titre = "Rebond entre particules";
                break;

            default:
                m_m1 = Matiere({255,0,0,255}, 1.0, 2.0, k, cc);

                m_part[0] = Particule(50,50, &m_m1);
                m_part[0].appliquerDV(Vecteur(25.0,15.0));

                m_titre = "Rebond contre des bords plats";
                break;
        }

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

void demoCollision2() {
    def::redefGrille(180,100,5.0,0,2,3,true,true,8,16);
    def::redefTemp(true, 0.03, 0);
    def::nbIterationsEuler = 1;

    // Création des particules
    int nbPart = 2*JambonHexa::nbPart() + Mur(80,40).nbPart();
    Particule* particules = new Particule[nbPart];

    Mur mur(80, 40, 12.5, 12.5, 2.0, particules+2*JambonHexa::nbPart());

    // Création de la grille
    MatriceDemoCollision mat(180, 100, 1, particules, nbPart);

    // Lancement de la scène SDL
    try
    {
        SceneDemoCol2 scene(mat, mur, particules);
        scene.bouclePrincipale();
    }
    catch(Erreur& e) { std::cout << "Erreur !!" << std::endl << e.what() << std::endl; }

    delete[] particules;
}

#endif //PROTOTYPE_1_DEMOCOLLISION2_H
