#ifndef PROTOTYPE_1_DEMOCOLLISION1_H
#define PROTOTYPE_1_DEMOCOLLISION1_H

#include <sstream>
#include "SceneSDL.h"
#include "MatriceParticules.h"

#define NB_PART 10

class SceneDemoCollision1 : public SceneSDL
{
public:
    SceneDemoCollision1(Element& mat, int config, Particule* part, Matiere* matieres)
     : SceneSDL(mat), m_part(part), m_matieres(matieres)
    {}

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
        for(int i = 0 ; i < NB_PART ; i++)
        {
            m_matieres[i] = Matiere();
            m_part[i] = Particule(-1,-1,&m_matieres[i]);
        }

        double k = 300.0;
        double cc = 3.0;

        switch(config)
        {
            case 3:
                for(int i = 0 ; i < 4 ; i++)
                {
                    m_matieres[i] = Matiere({255,0,0,255},100.0,3.0,k,cc);
                    m_part[i].setPosInt(Vecteur(10.5,5.5+3.0*(double)i));
                    m_matieres[i+4] = Matiere({255,0,0,255},100.0,3.0,k,cc);
                    m_part[i+4].setPosInt(Vecteur(13.5,5.5+3.0*(double)i));

                    m_part[i].lier(&m_part[i+4]);
                }
                for(int i = 0 ; i < 3 ; i++)
                {
                    m_part[i].lier(&m_part[i+1]);
                    m_part[i+4].lier(&m_part[i+5]);
                }

                m_matieres[8] = Matiere({0,0,255,255},2.0,3.0,k,cc);
                m_matieres[9] = Matiere({0,0,255,255},2.0,3.0,k,cc);
                m_part[8].setPosInt(Vecteur(2.5,14.2));
                m_part[8].setV(Vecteur(3.0,0.0));
                m_part[9].setPosInt(Vecteur(5.5,14.2));
                m_part[9].setV(Vecteur(3.0,0.0));

                m_part[8].lier(&m_part[9]);
                break;

            case 2:
                m_matieres[0] = Matiere({255,0,0,255}, 1.0);
                m_matieres[1] = Matiere({255,0,0,255}, 1.0);
                m_matieres[2] = Matiere({0,0,255,255}, 2.0);
                m_matieres[3] = Matiere({255,0,0,255}, 1.0);

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
                m_matieres[0] = Matiere({255,0,0,255}, 1.0, 2.0, 1.0, 0.0);
                m_matieres[1] = Matiere({0,0,255,255}, 1.0, 2.0, 1.0, 0.0);
                m_matieres[2] = Matiere({0,255,0,255}, 2.0);
                m_matieres[3] = Matiere({0,255,255,255}, 1.0);

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

        m_element.reinit();
    }

private:
    Particule* m_part;
    Matiere* m_matieres;
};

void demoCollision1(int config = 1)
{
    def::redefGrille(32,32,10,0,false,true,4,16);
    def::redefTemp(true, 0.05, 0);

    // Création de la matière
    Matiere* matieres = new Matiere[NB_PART];

    // On crée au préalable les particules
    Particule* particules = new Particule[NB_PART];

    // Création de la matrice
    MatriceParticules<16,4> m(32,32, 0, particules, NB_PART);

    // On fait tourner la scène
    SceneDemoCollision1 scene(m, config, particules, matieres);
    try { scene.bouclePrincipale(); }
    catch(Erreur& e) { std::cout << "Erreur !!" << std::endl << e.what() << std::endl; }

    delete[] particules;
    delete[] matieres;
}

#endif //PROTOTYPE_1_DEMOCOLLISION1_H
