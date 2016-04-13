#ifndef PROTOTYPE_1_DEMOCOLLISION1_H
#define PROTOTYPE_1_DEMOCOLLISION1_H

#include <sstream>
#include "SceneSDL.h"

#define NB_PART 4

class SceneDemoCollision1 : public SceneSDL
{
public:
    SceneDemoCollision1(MatriceParticules& mat, Particule* part, Matiere* matieres)
     : SceneSDL(mat), m_part(part), m_matieres(matieres)
    {
        init(1);
    }

    void charger(int config)
    {
        switch(config)
        {
            case 2:
                m_part[0].setPosInt(Vecteur(5.5,5.5));
                m_part[1].setPosInt(Vecteur(25.5,5.5));
                m_part[2].setPosInt(Vecteur(5.5,25.5));
                m_part[3].setPosInt(Vecteur(25.5,25.5));

                m_part[0].setV(Vecteur(5.0,1.0));
                m_part[1].setV(Vecteur(-5.0,1.0));
                m_part[2].setV(Vecteur(5.0,-1.0));
                m_part[3].setV(Vecteur(-5.0,-1.0));
                break;

            default:
                m_part[0].setPosInt(Vecteur(5.5,5.5));
                m_part[1].setPosInt(Vecteur(25.5,5.5));
                m_part[2].setPosInt(Vecteur(5.5,25.5));
                m_part[3].setPosInt(Vecteur(25.5,25.5));

                m_part[0].setV(Vecteur(5.0,0.0));
                m_part[1].setV(Vecteur(-5.0,0.0));
                m_part[2].setV(Vecteur(5.0,0.0));
                m_part[3].setV(Vecteur(-5.0,0.0));
        }
        m_titre = "Test de collisions entre particules (non liees)";

        m_mat.reinit();
    }

private:
    Particule* m_part;
    Matiere* m_matieres;
};

void demoCollision1()
{
    def::redefGrille(32,32,10,1,false,true,4,4);
    def::redefTemp(true, 0.05, 0);

    // Création de la matière
    Matiere* matieres = new Matiere[NB_PART];
    matieres[0] = Matiere({255,0,0,255}, 1.0, 2.0, 1.0, 0.0);
    matieres[1] = Matiere({0,0,255,255}, 1.0, 2.0, 1.0, 0.0);
    matieres[2] = Matiere({0,255,0,255}, 2.0);
    matieres[3] = Matiere({0,255,255,255}, 1.0);

    // On crée au préalable les particules
    Particule* particules = new Particule[NB_PART];
    for(int i = 0 ; i < NB_PART ; i++)
        particules[i] = Particule(-1,-1,&matieres[i]);

    // Création de la matrice
    MatriceParticules m(32,32,4,4, particules, NB_PART);

    // On fait tourner la scène
    SceneDemoCollision1 scene(m, particules, matieres);
    try { scene.bouclePrincipale(); }
    catch(Erreur& e) { std::cout << "Erreur !!" << std::endl << e.what() << std::endl; }

    delete[] particules;
    delete[] matieres;
}

#endif //PROTOTYPE_1_DEMOCOLLISION1_H
