#ifndef MATIERE_H_INCLUDED
#define MATIERE_H_INCLUDED

#include <SDL2/SDL.h>
#include "Vecteur.h"

class Particule;

class Matiere
{
public:
    Matiere(SDL_Color couleur = {255,255,255,0}, double masse = 1.0, double l0 = 0.0, double raideur = 0.0, double amortissement = 0.0,
            double dLiaisonInv=0.5, double lLiaisonMax=3.0,int exposantLiaison=1);

    double getMasse() const { return m_masse; }
    SDL_Color getCouleur() const { return m_couleur; }

    // Renvoie la force de liaison appliquee a p1
    virtual Vecteur forceLiaison(const Particule* p1, const Particule* p2) const;

//private:
    SDL_Color m_couleur;
    double m_masse;
    double m_l0;
    double m_raideur;
    double m_amor;

    double m_dLiaisonInv; //La distance à laquelle on change de profil de force.
    double m_lLiaisonMax; //La longueur de brisure de la liaison;
    int m_exposantLiaison; //Exposant du terme inverse de la liaison.

    double m_a;
    double m_b;
    double m_c;
};

#endif // MATIERE_H_INCLUDED
