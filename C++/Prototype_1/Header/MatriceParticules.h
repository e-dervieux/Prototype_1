#ifndef MATRICEPARTICULES_H_INCLUDED
#define MATRICEPARTICULES_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "MatriceCreuse.h"
#include "Particule.h"
#include "Definitions.h"

class MatriceParticules : public MatriceCreuse<Particule*>
{
public:
    MatriceParticules(int mpX, int mpY, int smX, int smY);

    void ajouterPart(std::vector<Particule>& particules);

    void forcesLiaison(); // Calcule et applique les forces de liaison entre les particules
    void calculerDeplacement(double dt); //  Calcule la prochaine position des particules
    void deplacer(); // Effectue le d�placement des particules dans la matrice

    void afficher(SDL_Renderer* rendu); // Calcule les couleurs des pixels, et les affiche sur le rendu SDL
};

#endif // MATRICEPARTICULES_H_INCLUDED
