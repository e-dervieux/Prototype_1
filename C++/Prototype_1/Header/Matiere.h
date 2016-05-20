#ifndef MATIERE_H_INCLUDED
#define MATIERE_H_INCLUDED

#include <SDL2/SDL.h>
#include "Vecteur.h"
#include "Brisure.h"

class Particule;

class Matiere
{
public:
    Matiere(SDL_Color couleur = {255,255,255,0}, double masse = 1.0, double l0 = 0.0, double raideur = 0.0, double amortissement = 0.0,
            double dLiaisonInv=0.5, double lLiaisonMax=10.0,int exposantLiaison=1);

    double getMasse() const { return m_masse; }
    SDL_Color getCouleur() const { return m_couleur; }

    // Applique la force de liaison appliquee a p1, et brise les liaisons si nécessaire
    virtual void forceLiaison(Particule* p, LiaisonPart& lp) const;

//private:
    SDL_Color m_couleur;
    double m_masse;
    double m_l0;
    double m_raideur;
    double m_amor;

    double m_dLiaisonInv; //La distance à laquelle on change de profil de force.
    double m_lLiaisonMax; //La longueur de brisure de la liaison;
    double m_lLiaisonSuppr; // Longueur à partir de laquelle une liaison brisée est supprimée
    int m_exposantLiaison; //Exposant du terme inverse de la liaison.

    double m_a;
    double m_b;
    double m_c;
};

#endif // MATIERE_H_INCLUDED
