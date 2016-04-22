#ifndef PROTOTYPE_1_CONTENEUR_H
#define PROTOTYPE_1_CONTENEUR_H

#include "Particule.h"

// Conteneur de particules
class Conteneur : public Element
{
public:
    Conteneur()
     : m_nbP(0)
    {}

    virtual void actualiserBarycentre() = 0;
    bool estVide() { return m_nbP==0; };
    inline int getNbP() const { return m_nbP; }
    virtual int getNbLDroite() const = 0;
    virtual int getNbLBas() const = 0;
    virtual int getNbLTot() const = 0;

    // Vide le conteneur et le re-remplit s'il est attaché à d'autres données (cf MatriceParticulesOld)
    virtual void reinit()
    {
        m_nbP = 0;
        Element::reinit();
    }

    // Rajoute une particule dans les calculs barycentriques
    virtual void ajouterParticule(Particule* p)
    {
        if (p == NULL)
            return;

        int newP = m_nbP+1;
        double newMasse = m_masse += p->getMasse();

        m_pos = 1.0/newMasse * (m_masse*m_pos + (p->getMasse())*(p->getPos()));
        m_v = 1.0/newMasse * (m_masse*m_v + (p->getMasse())*(p->getV()));

        SDL_Color c = p->getCouleur();
        m_couleur.r = (Uint8)( (m_nbP*m_couleur.r + c.r)/newP );
        m_couleur.g = (Uint8)( (m_nbP*m_couleur.g + c.g)/newP );
        m_couleur.b = (Uint8)( (m_nbP*m_couleur.b + c.b)/newP );
        m_couleur.a = (Uint8)( (m_nbP*m_couleur.a + c.a)/newP );

        m_masse = newMasse;
        m_nbP = newP;
    }

    virtual void supprimerParticule(Particule* p)
    {
        if (p == NULL)
            return;

        int newP = m_nbP-1;
        double newMasse = m_masse += p->getMasse();

        if (newMasse > 0.0)
        {
            m_pos = 1.0/newMasse * (m_masse*m_pos - (p->getMasse())*(p->getPos()));
            m_v = 1.0/newMasse * (m_masse*m_v - (p->getMasse())*(p->getV()));
        }

        if (newP > 0)
        {
            SDL_Color c = p->getCouleur();
            m_couleur.r = (Uint8)( (m_nbP*m_couleur.r - c.r)/newP );
            m_couleur.g = (Uint8)( (m_nbP*m_couleur.g - c.g)/newP );
            m_couleur.b = (Uint8)( (m_nbP*m_couleur.b - c.b)/newP );
            m_couleur.a = (Uint8)( (m_nbP*m_couleur.a - c.a)/newP );
        }

        m_masse = newMasse;
        m_nbP = newP;
    }

protected:
    int m_nbP; // Nombre de particules total
};

#endif //PROTOTYPE_1_CONTENEUR_H
