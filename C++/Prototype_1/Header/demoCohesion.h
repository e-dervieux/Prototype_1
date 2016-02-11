//
// Created by Marc on 11/02/2016.
//

#ifndef PROTOTYPE_1_DEMOCOHESION_H
#define PROTOTYPE_1_DEMOCOHESION_H

#include "demoLiaison.h"
#include "SceneSDL.h"

void demoCohesion()
{
    def::redefinir(200,120,5,true,true,16,4);

    // Création de la matière
    SDL_Color c = {255, 0, 0, 255};
    Matiere m(c,1.0, 4.0, 0.5, 0.1);

    // Création des particules
    int nbPart = 3;
    Particule* particules = new Particule[nbPart];
    Particule refP(-1, -1, &m);
    for(int i = 0 ; i < nbPart ; i++)
        particules[i] = refP;

    particules[0].setPosInt(Vecteur(20.5, 20.5));
    particules[1].setPosInt(Vecteur(22.5, 23.5));
    particules[2].setPosInt(Vecteur(24.5, 20.5));
    particules[0].lier(&particules[1]);
    particules[1].lier(&particules[2]);
    particules[2].lier(&particules[0]);

    // Création de la grille
    Grille g(200, 120, 16, 16, particules, nbPart);

    // Lancement de la scène SDL
    SceneSDL scene(g);
    try
    {
        scene.bouclePrincipale();
    }
    catch(Erreur& e)
    {
        std::cout << "Erreur !!" << std::endl << e.what() << std::endl;
    }

    delete[] particules;
}

#endif //PROTOTYPE_1_DEMOCOHESION_H
