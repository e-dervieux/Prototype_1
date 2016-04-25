//
// Created by Marc on 24/04/2016.
//

#ifndef PROTOTYPE_1_DEMOCOLLISION2_H
#define PROTOTYPE_1_DEMOCOLLISION2_H

#include "demoCohesion.h"

using MatriceDemoCollision = MatriceParticules<16,8>;

class SceneDemoCol2 : public SceneDemoCohesion
{
public:
    SceneDemoCol2(MatriceDemoCollision& mat, Jambon& j1, Jambon& j2, Matiere& m)
     : SceneDemoCohesion(mat,j1,j2,m), m_mat(mat), m_appui(false)
    {}

    virtual void actionClavier(bool& continuer, bool& recommencer)
    {
        SceneDemoCohesion::actionClavier(continuer, recommencer);

        if (m_clavier[def::K_BAS] && !m_appui)
        {
            m_appui = true;
            m_mat.afficherLiaisonsSM(8,1);
        }
        else if (!m_clavier[def::K_BAS])
            m_appui = false;
    }

private:
    MatriceDemoCollision& m_mat;
    bool m_appui;
};

void demoCollision2()
{
    double L = 5.0;

    def::redefGrille(180,100,5.0,0,1,2,true,true,8,16);
    def::redefTemp(true, 0.03, 0);

    // Création de la matière
    SDL_Color c = {255, 0, 0, 255};
    Matiere m(c, 1.0);

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
    MatriceDemoCollision mat(180, 100, 1, particules, nbPart);

    // Lancement de la scène SDL
    SceneDemoCol2 scene(mat, j1, j2, m);
    try { scene.bouclePrincipale(); }
    catch(Erreur& e) { std::cout << "Erreur !!" << std::endl << e.what() << std::endl; }

    delete[] particules;
}
#endif //PROTOTYPE_1_DEMOCOLLISION2_H
