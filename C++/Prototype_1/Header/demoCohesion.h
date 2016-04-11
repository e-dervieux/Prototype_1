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
        Vecteur v1(m_l * sqrt(3.0)/2.0,0.5 * m_l);
        Vecteur v2(m_l * sqrt(3.0),0.0 * m_l);
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

    static int nbPart() { return C*C; }

private:
    Vecteur m_o;
    Particule* m_part;
    double m_l;
};

class SceneDemoCohesion : public SceneSDL
{
public:
    SceneDemoCohesion(MatriceParticules& mat, Jambon & j1, Jambon & j2, Matiere& m)
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

    void charger(int config)
    {
        m_j1.init();
        m_j2.init();
        m_mat.reinit();

        SDL_Color rouge = {255,0,0,255};
        switch(config)
        {
            case 2:
                m_k = 800.0;
                m_cc = 85.0;
                break;

            case 3:
                m_k = 1000.0;
                m_cc = 100.0;
                break;

            default:
                m_k = 500.0;
                m_cc = 60.0;
        }
        m_m = Matiere(rouge,1.0,3.0,m_k,m_cc);
        std::stringstream tmp;
        tmp << "Test de cohésion : k = " << m_k << ", c = " << m_cc;
        m_titre = tmp.str();

        if (m_config != config)
        {
            def::pasGrille = 16;
            def::partPP = 1;
        }
        def::divisionGrille = 5;
    }

private:
    Jambon & m_j1;
    Jambon & m_j2;

    Matiere& m_m;
    double m_k, m_cc;
};

void demoCohesion()
{
    double L = 5.0;
    double L0, K, CC; // Initialisés par la scène

    def::redefGrille(200,120,5,1,false);
    def::redefTemp(true, 0.003, 0);

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
    MatriceParticules mat(200, 120, 16, 16, particules, nbPart);

    // Lancement de la scène SDL
    SceneDemoCohesion scene(mat, j1, j2, m);
    try { scene.bouclePrincipale(); }
    catch(Erreur& e) { std::cout << "Erreur !!" << std::endl << e.what() << std::endl; }

    delete[] particules;
}

#endif //PROTOTYPE_1_DEMOCOHESION_H
