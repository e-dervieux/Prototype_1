#include <iostream> // DEBUG
#include "../Header/Matiere.h"
#include "../Header/Particule.h"

Matiere::Matiere(SDL_Color couleur, double masse, double l0, double raideur, double amortissement)
 : m_couleur(couleur), m_masse(masse), m_l0(l0), m_raideur(raideur), m_amor(amortissement)
{}

double Matiere::getMasse(const Particule& p) const
{
    return m_masse;
}

Vecteur Matiere::forceLiaison(const Particule* p1, const Particule* p2) const
{
    Vecteur ab(p1->getPos(),p2->getPos());
    Vecteur vr(p1->getV(), p2->getV());

    double l = ab.norme();

    double d1 = 2.5, d0 = 1.5;

    if (l >= d1)
        return  m_raideur*(l-m_l0)*(ab.unitaire()) + m_amor*vr;
    else if (l > d0)
    {
        double tmp = (d1-d0)/(l-d0);
        return -m_raideur*(d1-m_l0)*tmp*tmp*(ab.unitaire()) + m_amor*vr;
    }
    else
    {
        std::cout << "A" << std::endl;
        return 1.0*(ab.unitaire());
    }
}
