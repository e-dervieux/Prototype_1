//
// Created by Marc on 08/02/2016.
//

#ifndef PROTOTYPE_1_DEMOMATRICEPARTICULES_H
#define PROTOTYPE_1_DEMOMATRICEPARTICULES_H

#include <vector>

#include "MatriceParticules.h"

void demoMatriceParticules()
{
    // Création de la matière
    SDL_Color rouge;
    rouge.r=255;rouge.g=0;rouge.b=0;rouge.a=255;
    Matiere matiere(rouge);

    // On crée au préalable les particules
    Particule* particules = new Particule[2];
    particules[0] = Particule(2,2,&matiere); // P0
    particules[1] = Particule(3,3,&matiere); // P1

    // Création de la matrice
    MatriceParticules<4> m(16,16, 0, particules, 2);

    // Affichage
    std::cout << "Ajout de 2 particules en (2,2) et (3,3) :" << std::endl;
    m.afficherP();
    std::cout << "Sous-matrice (0,0) :" << std::endl;
    m.getSM(0,0).afficherP();

    std::cout << "   ----------" << std::endl << std::endl;
    std::cout << "On bouge la particule de (3.5,3.5) a (3.7,5.1)" << std::endl;
    particules[1].setPos(Vecteur(3.7,5.1));
    m.deplacer(0.0);

    m.afficherP();
    std::cout << "Sous-matrice (0,0) :" << std::endl;
    m.getSM(0,0).afficherP();
    std::cout << "Sous-matrice (0,0) :" << std::endl;
    m.getSM(0,1).afficherP();
    std::cout << std::endl << std::endl;
}

#endif //PROTOTYPE_1_DEMOMATRICEPARTICULES_H
