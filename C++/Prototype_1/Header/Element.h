#ifndef PROTOTYPE_1_ELEMENT_H
#define PROTOTYPE_1_ELEMENT_H

#include "Vecteur.h"

class Element
{
public:
    Element(Vecteur&& pos = Vecteur(), Vecteur&& v = Vecteur())
     : m_pos(pos), m_v(v), m_masse(0.0)
    {}

    inline Vecteur getPos() const { return m_pos; }
    inline void setPos(Vecteur pos) { m_pos = pos; }
    inline Vecteur getV() const { return m_v; }
    inline void setV(Vecteur v) { m_v = v; }
    virtual SDL_Color getCouleur() const { return m_couleur; }
    virtual double getMasse() const { return m_masse; }

protected:
    Vecteur m_pos;
    Vecteur m_v;
    double m_masse;

    SDL_Color m_couleur;
};

#endif //PROTOTYPE_1_ELEMENT_H
