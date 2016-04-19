#ifndef MATRICEPARTICULES_H_INCLUDED
#define MATRICEPARTICULES_H_INCLUDED

#include <iostream> // DEBUG
#include <SDL2/SDL.h>

#include "Particule.h"
#include "Definitions.h"

class MatriceParticulesOld : public Element
{
    friend void demoMatriceParticules();

public:
    MatriceParticulesOld(int w, int h, int smX, int smY, Particule* particules, int nbParticules);
    ~MatriceParticulesOld();

    void ajouterParticules(); // Ajoute les particules de m_particules dans la matrice
    void reinit(); // Réinitialise la matrice de particules à partir du tableau
    // particules, afin qu'elle soit gérée de manière correcte.

    bool estValide(Particule& p); // Indique si une particule est bien dans la matrice

    void forcesLiaison(); // Calcule et applique les forces de liaison entre les particules
    void calculerDeplacement(double dt); //  Calcule la prochaine position des particules
    void deplacer(double dt); // Effectue le déplacement des particules dans la matrice
    void actualiser(double dt); // Calcule la frame suivante, à partir des méthodes ci-dessus

    void afficher(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule); // Calcule les couleurs des pixels, et les affiche sur le rendu SDL
    void afficherLiaisons(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule);

    Particule* get(int x, int y);
    Particule** getSM(int i, int j) { return m_tabSM[i*m_mpY+j]; }

private:
    int m_w, m_h;
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
