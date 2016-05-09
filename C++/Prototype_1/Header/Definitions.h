#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

#include "Particule.h"

// Offset utilisé lors des collisions, pour que la particule ne soit jamais sur l'extreme bord d'une case
#define OFFSET_COLLISION 0.0001

namespace def
{
    enum TOUCHE{K_ESPACE, K_DROITE, K_HAUT, K_GAUCHE, K_BAS, K_Z, K_Q, K_S, K_D, K_L, K_G, K_SHIFT, K_KP_PLUS, K_KP_MOINS, NB_TOUCHES};

    // Elements liés à la grille
    extern int width; // Taille de la scène principale
    extern int height;
    extern double tailleParticule; // Taille graphique d'un pixel (en vrais pixels)
    extern int coucheAffichage; // Niveau de couche de particules à afficher
    extern int coucheCollision; // Niveau de couche pour la gestion de collisions avec les sous-matrices
    extern int coucheMax; // Niveau de couche maximal atteignable
    extern bool grilleAffichee; // Pour afficher eune grille par-dessus la rendu (utile pour le debug)
    extern bool divisionsAffichees; // Pour afficher des divisions plus grosses sur cette grille
    extern int pasGrille; // Nombre de pixels par côté de carreau de la grille
    extern int divisionGrille; // Nombre de pixels entre 2 divisions épaisses

    // Autres paramètres d'affichage
    extern bool liaisonsAffichees;

    // Paramètres temporels
    extern bool pasFixe; // Affichage à pas temporel fixe, ou à pas dynamique
    extern double dtMax; // Délai maximum entre frames (en s), ou bien pas temporel si on tourne à pas fixe
    extern Uint32 delaiEntreFrames; // En ms

    // Calculs physiques
    extern int nbIterationsEuler; // Nombre d'itérations du calcul d'Euler entre 2 frames

    // Pour le debug
    extern SDL_Renderer* rendu;

    // Redéfinir un ensemble de paramètres
    void redefGrille(int width, int height, double tailleParticule, int coucheAffichee = 0, int coucheCollision = 0, int coucheMax = 1,
                     bool grilleAffichee = false, bool divisionsAffichees = false, int pasGrille = 4, int divisionGrille = 16);
    void redefTemp(bool pasFixe, double dtMax = 0.1, Uint32 delaiEntreFrames = 0);

    void echellePlus();
    void echelleMoins();
}

#endif // DEFINITIONS_H_INCLUDED
