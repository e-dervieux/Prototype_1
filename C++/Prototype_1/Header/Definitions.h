#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

#include "MatriceCreuseDemo.h"
#include "Particule.h"

#define OFFSET_COLLISION 0.0001

namespace def
{
    enum TOUCHE{K_ESPACE, K_DROITE, K_HAUT, K_GAUCHE, K_BAS, K_Z, K_Q, K_S, K_D, K_L, K_G, K_SHIFT, K_KP_PLUS, K_KP_MOINS, NB_TOUCHES};

    // Elements liés à la grille
    extern int width;
    extern int height;
    extern int taillePixel; // Taille graphique d'un pixel (en vrais pixels)
    extern int partPP; // Nombre de particules sur une longueur de pixel
    extern bool grilleAffichee;
    extern bool divisionsAffichees;
    extern int pasGrille;
    extern int divisionGrille;

    // Autres paramètres d'affichage
    extern bool liaisonsAffichees;

    // Paramètres temporels
    extern bool pasFixe; // Affichage à pas temporel fixe, ou à pas dynamique
    extern double dtMax; // Délai maximum entre frames (en s), ou bien pas temporel si on tourne à pas fixe
    extern Uint32 delaiEntreFrames; // En ms

    // Calculs physiques
    extern int nbLiaisons; // Nombre de liaisons que peut avoir une particule

    void redefGrille(int width, int height, int taillePixel, int partPP = 1,
                     bool grilleAffichee = false, bool divisionsAffichees = false, int pasGrille = 5, int divisionGrille = 5);
    void redefTemp(bool pasFixe, double dtMax = 0.1, Uint32 delaiEntreFrames = 0);

    void echellePlus();
    void echelleMoins();
}

#endif // DEFINITIONS_H_INCLUDED
