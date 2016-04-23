#ifndef PROTOTYPE_1_DEMOCOLLISIONMATRICES_H
#define PROTOTYPE_1_DEMOCOLLISIONMATRICES_H

#include "MatriceParticules.h"
#include "SceneSDL.h"

#ifdef NB_PART
    #undef NB_PART
#endif
#define NB_PART 20


class SceneDemoCM : public SceneSDL
{
public:
    SceneDemoCM(Element& e, Particule* part)
            : SceneSDL(e), m_part(part)
    {}

    virtual void actionClavier(bool& continuer, bool& recommencer)
    {
        SceneSDL::actionClavier(continuer, recommencer);

        if (m_clavier[def::K_SHIFT])
            def::delaiEntreFrames = 500;
        else
            def::delaiEntreFrames = 100;
    }

    virtual void charger(int config)
    {
        switch(config)
        {
            default:
                m_part[0].setPosInt(Vecteur(12.5,12.5));
                m_part[0].setV(Vecteur(3.0,0.0));
                m_part[1].setPosInt(Vecteur(20.5,12.5));
                m_part[1].setV(Vecteur(0.0,0.0));

                m_part[2].setPosInt(Vecteur(12.5,20.5));
                m_part[2].setV(Vecteur(3.0,0.0));
                m_part[3].setPosInt(Vecteur(20.5,20.5));
                m_part[3].setV(Vecteur(0.0,0.0));
                m_part[2].lier(&m_part[3]);

                m_titre = "Collision simple entre 2 matrices / 2 particules";
        }

        m_element.reinit();
    }

private:
    Particule* m_part;
};

void demoCollisionMatrices()
{
    // Définitions
    def::redefGrille(32,32, 10.0, 0, 1, 2, true, true, 8, 16);
    def::redefTemp(true, 0.1, 100);
    def::liaisonsAffichees = true;

    // Création des particules
    Matiere m({255, 0, 0, 255}, 1.0, 3.0, 0.0, 0.0);
    Matiere m2({0, 0, 255, 255}, 1.0, 3.0, 0.0, 0.0);
    Particule* part = new Particule[NB_PART];
    for(int i = 0 ; i < NB_PART ; i++)
        part[i] = Particule(-1,-1, &m);
    part[3] = Particule(-1,-1, &m2);

    // Matrice de particules
    MatriceParticules<16,8> mat(32,32, 1, part, NB_PART);

    // Création de la scène
    SceneDemoCM scene(mat, part);

    // Simulation de la scène
    try { scene.bouclePrincipale(); }
    catch(Erreur& err) { std::cout << "Erreur !!" << std::endl << err.what() << std::endl; }

    delete[] part;
}

#endif //PROTOTYPE_1_DEMOCOLLISIONMATRICES_H
