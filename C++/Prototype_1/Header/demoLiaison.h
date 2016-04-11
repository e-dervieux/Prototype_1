#ifndef PROTOTYPE_1_DEMOLIAISON_H
#define PROTOTYPE_1_DEMOLIAISON_H

#include "demoGraphique1.h"

void demoLiaison()
{
    def::redefGrille(32,32,10,1,true,true,4,4);
    def::liaisonsAffichees = true;

    // Initiation de la fenetre
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* fenetre1 = SDL_CreateWindow("Test de liaison (1 part/pxl)",100,100,321,321,0);
    SDL_Renderer* rendu1 = SDL_CreateRenderer(fenetre1, -1, 0);

    SDL_Window* fenetre4 = SDL_CreateWindow("Test de liaison (4 part/pxl)", 500, 100,321,321,0);
    SDL_Renderer* rendu4 = SDL_CreateRenderer(fenetre4, -1, 0);

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
    MatriceParticules m(32,32,4,4, particules, 2);

    for(int i = 0 ; i < 500 ; i++)
    {
        m.forcesLiaison();
        m.calculerDeplacement(0.05);
        m.deplacer(0.05);

        // Affichage
        SDL_SetRenderDrawColor(rendu1,255,255,255,255);
        SDL_RenderClear(rendu1);
        m.afficher(rendu1, 1, 10);
        afficherGrille(rendu1, 32, 32, 10, 4, 4);
        m.afficherLiaisons(rendu1, 1, 10);
        SDL_RenderPresent(rendu1);

        SDL_SetRenderDrawColor(rendu4,255,255,255,255);
        SDL_RenderClear(rendu4);
        m.afficher(rendu4, 4, 40);
        afficherGrille(rendu4, 32, 32, 40, 1, 4);
        m.afficherLiaisons(rendu4, 4, 40);
        SDL_RenderPresent(rendu4);
        SDL_Delay(50);
    }

    // Fin
    SDL_DestroyRenderer(rendu1);
    SDL_DestroyRenderer(rendu4);
    SDL_DestroyWindow(fenetre1);
    SDL_DestroyWindow(fenetre4);
    SDL_Quit();

    delete[] particules;
}

#endif //PROTOTYPE_1_DEMOLIAISON_H
