#ifndef MATIERE_H_INCLUDED
#define MATIERE_H_INCLUDED

#include "SDL2/SDL.h"
#include "Vecteur.h"

class Particule;

class Matiere
{
public:
    Matiere(SDL_Color couleur, double masse, double l0, double raideur, double amortissement);

    double getMasse(const Particule& p) const;
    // Renvoie la force de liaison appliquée à p1
    virtual Vecteur forceLiaison(const Particule& p1, const Particule& p2) const;

//private:
    SDL_Color m_couleur;
    double m_masse;
    double m_l0;
    double m_raideur;
    double m_amor;
};

#endif // MATIERE_H_INCLUDED
