//
// Created by Marc on 10/02/2016.
//

#ifndef PROTOTYPE_1_DEMOLIAISON_H
#define PROTOTYPE_1_DEMOLIAISON_H

#include "demoGraphique1.h"

void demoLiaison()
{
    def::redefinir(32,32,10,true,4,4);
    def::partPP = 1;

    // Initiation de la fenetre
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* fenetre = SDL_CreateWindow("Test de liaison",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,321,321,0);
    SDL_Renderer* rendu = SDL_CreateRenderer(fenetre, -1, 0);

    // Création de la matière
    SDL_Color rouge;
    rouge.r=255;rouge.g=0;rouge.b=0;rouge.a=255;
    Matiere matiere(rouge, 1.0, 15.0, 1.0, 0.1);

    // On crée au préalable les particules
    Particule* particules = new Particule[2];
    particules[0] = Particule(5,5,&matiere); // P0
    particules[1] = Particule(20,25,&matiere); // P1

    particules[0].lier(&particules[1]);

    // Création de la matrice
    MatriceParticules m(8,8,4,4, particules, 2);

    for(int i = 0 ; i < 500 ; i++)
    {
        m.forcesLiaison();
        m.calculerDeplacement(0.05);
        m.deplacer();

        // Affichage
        SDL_SetRenderDrawColor(rendu,255,255,255,255);
        SDL_RenderClear(rendu);
        m.afficher(rendu);
        afficherGrille(rendu);
        SDL_RenderPresent(rendu);
        SDL_Delay(50);
    }

    // Fin
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();

    delete[] particules;
}

#endif //PROTOTYPE_1_DEMOLIAISON_H
