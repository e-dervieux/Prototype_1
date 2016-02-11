#ifndef MATRICEPARTICULES_H_INCLUDED
#define MATRICEPARTICULES_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "MatriceCreuse.h"
#include "Particule.h"
#include "Definitions.h"

class MatriceParticules
{
public:
    MatriceParticules(int mpX, int mpY, int smX, int smY, Particule* particules, int nbParticules);
    ~MatriceParticules();

    bool estValide(Particule& p);

    void forcesLiaison(); // Calcule et applique les forces de liaison entre les particules
    void calculerDeplacement(double dt); //  Calcule la prochaine position des particules
    void deplacer(); // Effectue le d�placement des particules dans la matrice

void afficher(SDL_Renderer* rendu, int partPP, int taillePixel); // Calcule les couleurs des pixels, et les affiche sur le rendu SDL

    Particule** getSM(int i, int j) { return m_tabSM[i*m_mpY+j]; }
//private:
    int m_mpX, m_mpY;
    int m_smX, m_smY;
    Particule* m_part; // Tableau des particules à gérer
    int m_nbPart; // Nombre de particules dans le tableau

    Particule*** m_tabSM;
    int* m_tabCnt;

    void set(int x, int y, Particule* p);
    void suppr(int x, int y);
    bool estVide(int x, int y);
};

#endif // MATRICEPARTICULES_H_INCLUDED
