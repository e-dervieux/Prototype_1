#ifndef PROTOTYPE_1_DEMOCOLLISION1_H
#define PROTOTYPE_1_DEMOCOLLISION1_H

#include "../Header/demoCohesion.h"

void demoCollision1()
{
    def::redefinir(16,16,10,true,true,4,4);

    // Initiation de la fenetre
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* fenetre = SDL_CreateWindow("Premier test graphique",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,321,321,0);
    SDL_Renderer* rendu = SDL_CreateRenderer(fenetre, -1, 0);

    // Création de la matière
    Matiere* matieres = new Matiere[4];
    matieres[0] = Matiere({255,0,0,255}, 1.0, 2.0, 1.0, 0.0);
    matieres[1] = Matiere({0,0,255,255}, 1.0, 2.0, 1.0, 0.0);
    matieres[2] = Matiere({0,255,0,255}, 3.0);
    matieres[3] = Matiere({0,255,255,255}, 1.0);

    // On crée au préalable les particules
    Particule* particules = new Particule[4];
    particules[0] = Particule(5,5,&matieres[0]); // P0
    particules[1] = Particule(25,5,&matieres[1]); // P1
    particules[2] = Particule(5,25,&matieres[2]); // P2
    particules[3] = Particule(25,25,&matieres[3]); // P3

    // Création de la matrice
    MatriceParticules m(8,8,4,4, particules, 4);

    // Application des vitesses de base
    particules[0].setV(Vecteur(5.0,0.0));
    particules[1].setV(Vecteur(-5.0,0.0));
    particules[2].setV(Vecteur(5.0,0.0));
    particules[3].setV(Vecteur(-5.0,0.0));

    for(int i = 0 ; i < 80 ; i++)
    {
        m.calculerDeplacement(0.1);
        m.deplacer(0.1);

        // Affichage
        SDL_SetRenderDrawColor(rendu,255,255,255,255);
        SDL_RenderClear(rendu);
        m.afficher(rendu, 1, 10);

        //afficherGrille(rendu, 32, 32, 10, 4, 4);

        SDL_RenderPresent(rendu);
        SDL_Delay(100);
    }

    // Fin
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();

    delete[] particules;
    delete[] matieres;
}

#endif //PROTOTYPE_1_DEMOCOLLISION1_H
