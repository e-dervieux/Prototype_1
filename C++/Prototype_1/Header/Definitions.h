#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

#include "MatriceCreuse.h"
#include "Particule.h"

namespace def
{
    class EstNulPart : public EstNul<Particule>
    {
    public:
        bool operator()(Particule& p) { return p.m_matiere == NULL; }
    };
    class DefautPart : public Defaut<Particule>
    {
    public:
        Particule operator()(int x, int y) { return Particule(x,y,NULL); }
    };
    extern EstNulPart testPart;
    extern DefautPart defautPart;

    enum TOUCHE{K_DROITE, K_HAUT, K_GAUCHE, K_BAS, K_Z, K_Q, K_S, K_D, K_SHIFT, NB_TOUCHES};

    // Elements liés à la grille
    extern int width;
    extern int height;
    extern int taillePixel; // Taille graphique d'un pixel (en pixels graphiques) (=échelle)
    extern int partPP; // Nombre de particules sur une longueur de pixel
    extern bool grilleAffichee;
    extern int pasGrille;
    extern int divisionGrille;

    // Calculs physiques
    extern double dtMax;
    extern int nbLiaisons; // Nombre de liaisons que peut avoir une particule

    void redefinir(int w = 200, int h = 150, int tp = 5, bool ga = true, int pg = 5, int dg = 5);
}

#endif // DEFINITIONS_H_INCLUDED
