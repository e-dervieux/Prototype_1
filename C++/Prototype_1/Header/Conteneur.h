#ifndef PROTOTYPE_1_CONTENEUR_H
#define PROTOTYPE_1_CONTENEUR_H

#include "Particule.h"

// Conteneur de particules
class Conteneur : public Element
{
public:
    Conteneur()
     : m_nbP(0), m_rTot(0), m_gTot(0), m_bTot(0), m_aTot(0)
    {}

    virtual void actualiserBarycentre() = 0;
    bool estVide() { return m_nbP==0; };
    inline int getNbP() const { return m_nbP; }
    virtual int getNbLDroite() const = 0;
    virtual int getNbLBas() const = 0;
    virtual int getNbLTot() const = 0;

    virtual SDL_Color getCouleur() const
    {
        if (m_nbP != 0)
            return {(Uint8)(m_rTot/m_nbP), (Uint8)(m_gTot/m_nbP), (Uint8)(m_bTot/m_nbP), (Uint8)(m_aTot/m_nbP)};
        else
            return {255,255,255,0};
    }

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
        double newMasse = m_masse + p->getMasse();

        m_pos = 1.0/newMasse * (m_masse*m_pos + (p->getMasse())*(p->getPos()));
        m_v = 1.0/newMasse * (m_masse*m_v + (p->getMasse())*(p->getV()));

        SDL_Color c = p->getCouleur();
        m_rTot += c.r;
        m_gTot += c.g;
        m_bTot += c.b;
        m_aTot += c.a;

        m_masse = newMasse;
        m_nbP = newP;
    }

    virtual void supprimerParticule(Particule* p)
    {
        if (p == NULL)
            return;

        int newP = m_nbP-1;
        double newMasse = m_masse - p->getMasse();

        if (newMasse > 0.0)
        {
            m_pos = 1.0/newMasse * (m_masse*m_pos - (p->getMasse())*(p->getPos()));
            m_v = 1.0/newMasse * (m_masse*m_v - (p->getMasse())*(p->getV()));
        }

        SDL_Color c = p->getCouleur();
        m_rTot -= c.r;
        m_gTot -= c.g;
        m_bTot -= c.b;
        m_aTot -= c.a;

        m_masse = newMasse;
        m_nbP = newP;
    }

protected:
    int m_nbP; // Nombre de particules total

    // Pour les calculs de couleur
    int m_rTot, m_gTot, m_bTot, m_aTot;
};

#endif //PROTOTYPE_1_CONTENEUR_H
