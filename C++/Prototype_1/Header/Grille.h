#ifndef GRILLE_H_INCLUDED
#define GRILLE_H_INCLUDED

// #include "Definitions.h"
#include "MatriceParticules.h"

class Grille // Cette classe est-elle utile, vis-a-vis
{
public:
    Grille(int w, int h, int smW, int smH, Particule* particules, int nbParticules);
    ~Grille();

    void actualiser(double dt); // Applique tous les calculs physiques pour obtenir la nouvelle grille apres dt
    // Devrait etre la seule fonction a utiliser depuis SceneSDL.

private:
    int m_w, m_h; // Dimensions globales de la matrice creuse
    int m_smW, m_smH; // A supprimer ?

    MatriceParticules m_mat; // Matrice de particules
};

#endif // GRILLE_H_INCLUDED
