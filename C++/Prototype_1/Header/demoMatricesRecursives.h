//
// Created by Marc on 16/04/2016.
//

#ifndef PROTOTYPE_1_DEMOMATRICESRECURSIVES_H
#define PROTOTYPE_1_DEMOMATRICESRECURSIVES_H

#include "MatriceParticules.h"
#include "demoCohesion.h"

void demoMatricesRecursives()
{
    double L = 5.0;
    double L0, K, CC; // Initialisés par la scène

    def::redefGrille(200,120,5.0,0,0,3,false,false,8,8);
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
    MatriceParticules<> mat(200, 120, particules, nbPart);

    // Lancement de la scène SDL
    SceneDemoCohesion scene(mat, j1, j2, m);
    try { scene.bouclePrincipale(); }
    catch(Erreur& e) { std::cout << "Erreur !!" << std::endl << e.what() << std::endl; }

    delete[] particules;
}

#endif //PROTOTYPE_1_DEMOMATRICESRECURSIVES_H
