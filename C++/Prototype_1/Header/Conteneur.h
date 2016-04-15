#ifndef PROTOTYPE_1_CONTENEUR_H
#define PROTOTYPE_1_CONTENEUR_H

#include "Particule.h"

// Conteneur de particules
class Conteneur : public Element
{
public:
    Conteneur()
            : m_nbE(0)
    {}

    virtual void actualiserBarycentre() = 0;
    bool estVide() { return m_nbE==0; };
    inline int getNbE() const { return m_nbE; }
    inline int getPTot() const { return m_pTot; }

    // Vide le conteneur et le re-remplit s'il est attaché à d'autres données (cf MatriceParticules)
    virtual void reinit()
    {
        m_nbE = 0;
        m_pTot = 0;
        Element::reinit();
    }

protected:
    int m_nbE; // Nombre de sous-éléments non vides (utile ?)
    int m_pTot; // Nombre de particules total
};

#endif //PROTOTYPE_1_CONTENEUR_H
