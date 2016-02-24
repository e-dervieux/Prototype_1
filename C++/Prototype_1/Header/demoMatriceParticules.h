//
// Created by Marc on 08/02/2016.
//

#ifndef PROTOTYPE_1_DEMOMATRICEPARTICULES_H
#define PROTOTYPE_1_DEMOMATRICEPARTICULES_H

#include <vector>

#include "demoMatriceCreuse.h"
#include "MatriceParticules.h"

void demoMatriceParticules()
{
    // Création de la matière
    SDL_Color rouge;
    rouge.r=255;rouge.g=0;rouge.b=0;rouge.a=255;
    Matiere matiere(rouge, 1.0, 2.0, 1.0, 0.0);

    // On crée au préalable les particules
    Particule* particules = new Particule[2];
    particules[0] = Particule(2,2,&matiere); // P0
    particules[1] = Particule(3,3,&matiere); // P1

    // Création de la matrice
    MatriceParticules m(4,4,4,4, particules, 2);

    // Affichage
    std::cout << "Ajout de 2 particules en (2,2) et (3,3) :" << std::endl;
    afficherMat(m.m_tabCnt,4,4);
    std::cout << "Sous-matrice (0,0) :" << std::endl;
    afficherMat(m.getSM(0,0),4,4);

    std::cout << "   ----------" << std::endl << std::endl;
    std::cout << "On bouge la particule de (3.5,3.5) a (3.7,5.1)" << std::endl;
    particules[1].setPos(Vecteur(3.7,5.1));
    m.deplacer(0.0);
    afficherMat(m.m_tabCnt,4,4);
    std::cout << "Sous-matrice (0,0) :" << std::endl;
    afficherMat(m.getSM(0,0),4,4);
    std::cout << "Sous-matrice (0,1) :" << std::endl;
    afficherMat(m.getSM(0,1),4,4);
}

#endif //PROTOTYPE_1_DEMOMATRICEPARTICULES_H
