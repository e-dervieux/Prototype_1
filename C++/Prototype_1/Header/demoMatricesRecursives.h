#ifndef PROTOTYPE_1_DEMOMATRICESRECURSIVES_H
#define PROTOTYPE_1_DEMOMATRICESRECURSIVES_H

#include "MatriceParticules.h"
#include "demoCohesion.h"

class SceneDemoMR : public SceneDemoCohesion
{
public:
    SceneDemoMR(Element& mat, Jambon & j1, Jambon & j2, Matiere& m)
     : SceneDemoCohesion(mat, j1, j2, m)
    {}

    virtual void charger(int config)
    {
        SceneDemoCohesion::charger(config);
        switch(config)
        {
            default:
                m_j2.appliquerDV(Vecteur(30.0,0.0));
                break;
        }
    }
};

void demoMatricesRecursives()
{
    double L = 5.0;
    double L0, K, CC; // Initialisés par la scène

    def::redefGrille(200,120,5.0,0,0,4,false,false,8,8);
    def::coucheCollision = 1;
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
    MatriceParticules<8,4,2> mat(200, 120, def::coucheCollision, particules, nbPart);
    std::cout << mat.getProfondeur();

    // Lancement de la scène SDL
    SceneDemoMR scene(mat, j1, j2, m);
    try { scene.bouclePrincipale(); }
    catch(Erreur& e) { std::cout << "Erreur !!" << std::endl << e.what() << std::endl; }

    delete[] particules;
}

#endif //PROTOTYPE_1_DEMOMATRICESRECURSIVES_H
