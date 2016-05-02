#include "../Header/Conteneur.h"
#include "../Header/Particule.h"

mcprive::Conteneur::Conteneur()
 : m_nbP(0), m_rTot(0), m_gTot(0), m_bTot(0), m_aTot(0), m_liaisons()
{}

SDL_Color mcprive::Conteneur::getCouleur() const
{
    if (m_nbP != 0)
        return {(Uint8) (m_rTot / m_nbP), (Uint8) (m_gTot / m_nbP), (Uint8) (m_bTot / m_nbP),
                (Uint8) (m_aTot / m_nbP)};
    else
        return {255, 255, 255, 0};
}

void mcprive::Conteneur::reinit()
{
    m_nbP = 0;
    Element::reinit();
}

void mcprive::Conteneur::ajouterParticule(Particule *p)
{
    if (p == NULL)
        return;

    int newP = m_nbP + 1;
    double newMasse = m_masse + p->getMasse();

    m_pos = 1.0 / newMasse * (m_masse * m_pos + (p->getMasse()) * (p->getPos()));
    m_v = 1.0 / newMasse * (m_masse * m_v + (p->getMasse()) * (p->getV()));

    SDL_Color c = p->getCouleur();
    m_rTot += c.r;
    m_gTot += c.g;
    m_bTot += c.b;
    m_aTot += c.a;

    m_masse = newMasse;
    m_nbP = newP;
}

void mcprive::Conteneur::supprimerParticule(Particule *p)
{
    if (p == NULL)
        return;

    int newP = m_nbP - 1;
    double newMasse = m_masse - p->getMasse();

    if (newMasse > 0.0)
    {
        m_pos = 1.0 / newMasse * (m_masse * m_pos - (p->getMasse()) * (p->getPos()));
        m_v = 1.0 / newMasse * (m_masse * m_v - (p->getMasse()) * (p->getV()));
    }

    SDL_Color c = p->getCouleur();
    m_rTot -= c.r;
    m_gTot -= c.g;
    m_bTot -= c.b;
    m_aTot -= c.a;

    m_masse = newMasse;
    m_nbP = newP;
}