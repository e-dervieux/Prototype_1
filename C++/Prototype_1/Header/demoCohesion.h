//
// Created by Marc on 11/02/2016.
//

#ifndef PROTOTYPE_1_DEMOCOHESION_H
#define PROTOTYPE_1_DEMOCOHESION_H

#include "demoLiaison.h"
#include "SceneSDL.h"

#define C 20
#define L 3.5

// Test de génération d'objets
class Jambon
{
public:
    Jambon(Vecteur(origine), Particule* particules)
     : m_o(origine), m_part(particules)
    {

    }

    void init()
    {
        Vecteur v1(L * sqrt(3.0)/2.0,0.5 * L);
        Vecteur v2(L * sqrt(3.0),0.0 * L);
        Vecteur v3(0.0, L);
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
    }

    static int nbPart() { return C*C + (C-1)*(C-1); }

private:
    Vecteur m_o;
    Particule* m_part;
};

class ActionCohesion1 : public ActionClavier
{
public:
    ActionCohesion1(Jambon& j)
     : m_j(j)
    {}
    virtual void operator()(std::vector<bool>& clavier, bool& continuer)
    {
        if (clavier[def::K_ESPACE])
            m_j.init();
    }

private:
    Jambon& m_j;
};

void demoCohesion()
{
    def::redefinir(200,120,5,true,true,16,4);

    // Création de la matière
    SDL_Color c = {255, 0, 0, 255};
    Matiere m(c, 1.0, 3.0, 15.0, 3.0);

    // Création des particules
    int nbPart = Jambon::nbPart();
    Particule* particules = new Particule[nbPart];
    Particule refP(-1, -1, &m);
    for(int i = 0 ; i < nbPart ; i++)
        particules[i] = refP;

    Jambon j1(Vecteur(50.0, 50.0),particules);
    j1.init();

    // Création de la grille
    Grille g(192, 112, 16, 16, particules, nbPart);

    // Lancement de la scène SDL
    ActionCohesion1 a(j1);
    SceneSDL scene(g, a);
    try
    {
        scene.bouclePrincipale();
    }
    catch(Erreur& e)
    {
        std::cout << "Erreur !!" << std::endl << e.what() << std::endl;
    }

    delete[] particules;
}

#endif //PROTOTYPE_1_DEMOCOHESION_H
