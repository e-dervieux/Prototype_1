#include "../Header/Matiere.h"
#include "../Header/Particule.h"

Matiere::Matiere(SDL_Color couleur, double masse, double l0, double raideur, double amortissement)
 : m_couleur(couleur), m_masse(masse), m_l0(l0), m_raideur(raideur), m_amor(amortissement)
{

}

double Matiere::getMasse(const Particule& p) const
{
    return m_masse;
}

Vecteur Matiere::forceLiaison(const Particule* p1, const Particule* p2) const
{
    Vecteur ab(p1->getPos(),p2->getPos());
    Vecteur vr(p1->getV(), p2->getV());

    // A paufiner
    return  m_raideur*(ab.norme()-m_l0)*(ab.unitaire()) - m_amor*vr;
}
