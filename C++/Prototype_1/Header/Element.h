//
// Created by Marc on 15/04/2016.
//

#ifndef PROTOTYPE_1_ELEMENT_H
#define PROTOTYPE_1_ELEMENT_H

class Element
{
public:
    Element()
     : m_masse(0.0), m_couleur({255,255,255,0})
    {}

    inline Vecteur getPos() const { return m_pos; }
    inline void setPos(Vecteur pos);
    inline Vecteur getV() const { return m_v; }
    inline void setV(Vecteur v) { m_v = v; }
    virtual SDL_Color getCouleur() const { return m_couleur; }
    virtual double getMasse() const { return m_masse; }

protected:
    Vecteur m_pos, m_v;
    double m_masse;

    SDL_Color m_couleur;
};

#endif //PROTOTYPE_1_ELEMENT_H
