//
// Created by Marc on 09/02/2016.
//

#ifndef PROTOTYPE_1_DEMOGRAPHIQUE1_H
#define PROTOTYPE_1_DEMOGRAPHIQUE1_H

#include <iostream>
#include <SDL2/SDL.h>
#include "MatriceParticules.h"

void afficherGrille(SDL_Renderer* rendu) {
    // Lignes verticales
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 200);
    for (int i = 0; i <= def::width; i += def::pasGrille)
        SDL_RenderDrawLine(rendu, i * def::taillePixel, 0, i * def::taillePixel, def::height * def::taillePixel);
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 128);
    for (int i = 0; i <= def::width; i += def::divisionGrille * def::pasGrille) {
        SDL_RenderDrawLine(rendu, i * def::taillePixel + 1, 0, i * def::taillePixel + 1,
                           def::height * def::taillePixel);
        SDL_RenderDrawLine(rendu, i * def::taillePixel - 1, 0, i * def::taillePixel - 1,
                           def::height * def::taillePixel);
    }

    // Lignes horizontales
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 200);
    for (int j = 0; j <= def::height; j += def::pasGrille)
        SDL_RenderDrawLine(rendu, 0, j * def::taillePixel, def::width * def::taillePixel, j * def::taillePixel);
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 128);
    for (int j = 0; j <= def::height; j += def::divisionGrille * def::pasGrille) {
        SDL_RenderDrawLine(rendu, 0, j * def::taillePixel + 1, def::width * def::taillePixel, j * def::taillePixel + 1);
        SDL_RenderDrawLine(rendu, 0, j * def::taillePixel - 1, def::width * def::taillePixel, j * def::taillePixel - 1);
    }
}

void demoGraphique1()
{
    def::redefinir(16,16,10,true,4,4);
    def::partPP = 1;

    // Initiation de la fenetre
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* fenetre = SDL_CreateWindow("Premier test graphique",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,161,161,0);
    SDL_Renderer* rendu = SDL_CreateRenderer(fenetre, -1, 0);

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

    // Application d'une force
    particules[0].appliquerForce(Vecteur(1.0,0.5));

    for(int i = 0 ; i < 15 ; i++)
    {
        m.calculerDeplacement(1.0);
        m.deplacer();

        // Affichage
        SDL_SetRenderDrawColor(rendu,255,255,255,255);
        SDL_RenderClear(rendu);
        m.afficher(rendu);
        afficherGrille(rendu);
        SDL_RenderPresent(rendu);
        SDL_Delay(500);
    }

    // Fin
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();

    delete[] particules;
}

#endif //PROTOTYPE_1_DEMOGRAPHIQUE1_H
