#include <iostream> // DEBUG
#include "../Header/Matiere.h"
#include "../Header/Particule.h"
#include "../Header/Definitions.h"

Matiere::Matiere(SDL_Color couleur, double masse, double l0, double raideur, double amortissement, double dLiaisonInv, double lLiaisonMax,int exposantLiaison)
 : m_couleur(couleur), m_masse(masse), m_l0(l0), m_raideur(raideur), m_amor(amortissement), m_dLiaisonInv(dLiaisonInv), m_lLiaisonMax(lLiaisonMax),m_exposantLiaison(exposantLiaison)
{
    double d=dLiaisonInv;
    double k=raideur;
    double f=def::fLiaisonMax;
    double p=exposantLiaison;

    m_a=-pow(d,p+1)*k*p*(d*d*k*k-2*d*k*k*l0+k*k*l0*l0-2*d*f*k+2*f*k*l0+f*f)/((d*k*p-k*l0*p-d*k-f*p)*(d*k*p-k*l0*p-d*k-f*p));
    m_b=pow(d,p+1)/(d*k*p-k*l0*p-d*k-f*p);
    m_c=k*(d*d*k*p-2*d*k*l0*p+k*l0*l0*p-d*f*p+f*l0*p-d*f)/(d*k*p-k*l0*p-d*k-f*p);
}

// TODO briser liaison si force trop intense

Vecteur Matiere::forceLiaison(Particule* p1, Particule* p2) const
{
    Vecteur ab(p1->getPos(),p2->getPos());
    Vecteur vr(p1->getV(), p2->getV());

    double l = ab.norme();

    if (l >= m_lLiaisonMax)
        throw Brisure(p1,p2);
    else if (l >= m_dLiaisonInv) // Force proportionnelle + amortissement
        return  m_raideur*(l-m_l0)*(ab.unitaire()) + m_amor*vr;
    else // Force en 1/r^exposant
        return (m_a/(m_b+pow(l,m_exposantLiaison))+m_c)*(ab.unitaire()) + m_amor*vr;
}
