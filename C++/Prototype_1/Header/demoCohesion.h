//
// Created by Marc on 11/02/2016.
//

#ifndef PROTOTYPE_1_DEMOCOHESION_H
#define PROTOTYPE_1_DEMOCOHESION_H

#include <sstream>
#include "demoLiaison.h"
#include "SceneSDL.h"

#define C 10 // Nombre de particules sur un côté

class Jambon
{
public:
    virtual void init() = 0;
    virtual void appliquerDV(Vecteur&& v) = 0;
};

// Test de génération d'objets
class JambonHexa : public Jambon
{
public:
    JambonHexa(Vecteur(origine), Particule* particules, double L)
     : m_o(origine), m_part(particules), m_l(L)
    {}

    void init()
    {
        Vecteur v1((double)m_l * sqrt(3.0)/2.0,0.5 * m_l);
        Vecteur v2((double)m_l * sqrt(3.0),0.0 * m_l);
        Vecteur v3(0.0, m_l);
        for(int i = 0 ; i < C ; i++)
        {
            for(int j = 0 ; j < C ; j++)
                m_part[i*C+j].setPosInt(m_o + i*v2 + j*v3);
        }
        for(int i = 0 ; i < C-1 ; i++)
        {
            for(int j = 0 ; j < C-1 ; j++)
                m_part[C*C + i*(C-1) + j].setPosInt(m_o + v1 + i*v2 + j*v3);
        }

        for(int i = 0 ; i < C-1 ; i++)
        {
            for(int j = 0 ; j < C-1 ; j++)
            {
                m_part[i*C + j].lier(&m_part[C*C + i*(C-1) + j]);
                m_part[(i+1)*C + (j+1)].lier(&m_part[C*C + i*(C-1) + j]);
                m_part[(i+1)*C + j].lier(&m_part[C*C + i*(C-1) + j]);
                m_part[i*C + (j+1)].lier(&m_part[C*C + i*(C-1) + j]);
            }
        }
        for(int i = 0 ; i < C ; i++)
        {
            for(int j = 0 ; j < C-1 ; j++)
                m_part[i*C + j].lier(&m_part[i*C + (j+1)]);
        }

        for(int i = 0 ; i < nbPart() ; i++)
            m_part[i].setV(Vecteur());

        // Suppression des coins
        m_part[0].supprimerLiaisons();
        m_part[0].setPosInt(Vecteur(-1.0,-1.0));
        m_part[C-1].supprimerLiaisons();
        m_part[C-1].setPosInt(Vecteur(-1.0,-1.0));
        m_part[C*(C-1)].supprimerLiaisons();
        m_part[C*(C-1)].setPosInt(Vecteur(-1.0,-1.0));
        m_part[C*C-1].supprimerLiaisons();
        m_part[C*C-1].setPosInt(Vecteur(-1.0,-1.0));
    }

    virtual void appliquerDV(Vecteur&& v)
    {
        for(int i = 0 ; i < nbPart() ; i++)
            m_part[i].appliquerDV(v);
    }

    static int nbPart() { return C*C + (C-1)*(C-1); }

private:
    Vecteur m_o;
    Particule* m_part;
    double m_l;
};

class JambonCarre : public Jambon
{
public:
    JambonCarre(Vecteur(origine), Particule* particules, double L)
     : m_o(origine), m_part(particules), m_l(L)
    {}

    void init()
    {
        Vecteur v1(m_l, 0.0);
        Vecteur v2(0.0, m_l);
        for(int i = 0 ; i < C ; i++)
        {
            for(int j = 0 ; j < C ; j++)
                m_part[i*C+j].setPosInt(m_o + i*v1 + j*v2);
        }

        for(int i = 0 ; i < C ; i++)
        {
            for(int j = 0 ; j < C ; j++)
            {
                if (i < C-1)
                    m_part[i*C + j].lier(&m_part[(i+1)*C + j]);

                if (j < C-1)
                    m_part[i*C + j].lier(&m_part[i*C + (j+1)]);
            }
        }

        for(int i = 0 ; i < nbPart() ; i++)
            m_part[i].setV(Vecteur());
    }

    virtual void appliquerDV(Vecteur&& v)
    {
        for(int i = 0 ; i < nbPart() ; i++)
            m_part[i].appliquerDV(v);
    }

    static int nbPart() { return C*C; }

private:
    Vecteur m_o;
    Particule* m_part;
    double m_l;
};

class SceneDemoCohesion : public SceneSDL
{
public:
    SceneDemoCohesion(Element& mat, Jambon & j1, Jambon & j2, Matiere& m)
     : SceneSDL(mat), m_j1(j1), m_j2(j2), m_m(m)
    {
        init(1);
    }

    virtual void actionClavier(bool& continuer, bool& recommencer)
    {
        SceneSDL::actionClavier(continuer, recommencer);

        if (m_clavier[def::K_SHIFT])
            def::delaiEntreFrames = 150;
        else
            def::delaiEntreFrames = 0;
    }

    virtual void charger(int config)
    {
        m_j1.init();
        m_j2.init();
        m_element.reinit();

        double k, cc, l0;

        SDL_Color rouge = {255,0,0,255};
        switch(config)
        {
            case 2:
                l0 = 2.5;
                k = 13.0;
                cc = 6.0;
                break;

            case 3:
                l0 = 3.0;
                k = 25.0;
                cc = 6.0;
                break;

            default:
                l0 = 3.0;
                k = 5.0;
                cc = 6.0;
        }
        m_m = Matiere(rouge,1.0,l0,k,cc);
        std::stringstream tmp;
        tmp << "Test de cohésion : l0 = " << l0 << ", k = " << k << ", c = " << cc;
        m_titre = tmp.str();
    }

protected:
    Jambon & m_j1;
    Jambon & m_j2;

    Matiere& m_m;
};

void demoCohesion()
{
    double L = 5.0;
    double L0, K, CC; // Initialisés par la scène

    def::redefGrille(200,120,5.0,0,0,3,false,false,8,8);
    def::redefTemp(true, 0.03, 0);

    // Création de la matière
    SDL_Color c = {255, 0, 0, 255};
    Matiere m(c, 1.0, L0, K, CC);

    // Création des particules
    int nbPart = JambonHexa::nbPart() + JambonCarre::nbPart();
    Particule* particules = new Particule[nbPart];
    Particule refP(-1, -1, &m);
    for(int i = 0 ; i < nbPart ; i++)
        particules[i] = refP;

    JambonHexa j1(Vecteur(80.5, 25.5), particules, L);
    j1.init();
    JambonCarre j2(Vecteur(10.5, 25.5), particules+JambonHexa::nbPart(), L);
    j2.init();

    // Création de la grille
    MatriceParticulesOld mat(200, 120, 16, 16, particules, nbPart);

    // Lancement de la scène SDL
    SceneDemoCohesion scene(mat, j1, j2, m);
    try { scene.bouclePrincipale(); }
    catch(Erreur& e) { std::cout << "Erreur !!" << std::endl << e.what() << std::endl; }

    delete[] particules;
}

#endif //PROTOTYPE_1_DEMOCOHESION_H
