#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

#include "MatriceCreuse.h"
#include "Particule.h"

namespace def
{
    enum TOUCHE{K_DROITE, K_HAUT, K_GAUCHE, K_BAS, K_Z, K_Q, K_S, K_D, K_SHIFT, NB_TOUCHES};

    // Elements li�s � la grille
    extern int width;
    extern int height;
    extern int taillePixel; // Taille graphique d'un pixel (en pixels graphiques) (=�chelle)
    extern int partPP; // Nombre de particules sur une longueur de pixel
    extern bool grilleAffichee;
    extern bool divisionsAffichees;
    extern int pasGrille;
    extern int divisionGrille;

    // Calculs physiques
    extern double dtMax;
    extern int nbLiaisons; // Nombre de liaisons que peut avoir une particule

    void redefinir(int w = 200, int h = 120, int tp = 5, bool ga = true, bool da = true, int pg = 5, int dg = 5);
}

#endif // DEFINITIONS_H_INCLUDED
