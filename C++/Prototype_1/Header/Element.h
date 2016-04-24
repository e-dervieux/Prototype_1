#ifndef PROTOTYPE_1_ELEMENT_H
#define PROTOTYPE_1_ELEMENT_H

#include "Vecteur.h"

// Element physique : masse, couleur, position, etc...
// Cela concerne également les barycentres, modèles équivalents des conteneurs
class Element
{
public:
    Element(Vecteur&& pos = Vecteur(), Vecteur&& v = Vecteur())
     : m_pos(pos), m_v(v), m_masse(0.0)
    {}

    virtual void actualiser(double dt) = 0; // Calcule la frame suivante
    virtual void afficher(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule) = 0; // Calcule les couleurs des pixels, et les affiche sur le rendu SDL
    virtual void afficherLiaisons(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule) {} // Utilisé par SceneSDL

    inline Vecteur getPos() const { return m_pos; }
    inline void setPos(Vecteur pos) { m_pos = pos; }
    inline Vecteur getV() const { return m_v; }
    inline void setV(Vecteur v) { m_v = v; }
    virtual SDL_Color getCouleur() const = 0;
    virtual double getMasse() const { return m_masse; }
    virtual void appliquerDV(Vecteur dv) { m_v += dv; }

    virtual void reinit()
    {
        m_masse = 0.0;
    }

protected:
    Vecteur m_pos;
    Vecteur m_v;
    double m_masse;
};

#endif //PROTOTYPE_1_ELEMENT_H
