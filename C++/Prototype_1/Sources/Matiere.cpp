#include <iostream> // DEBUG
#include "../Header/Matiere.h"
#include "../Header/Particule.h"
#include "../Header/Definitions.h"

Matiere::Matiere(SDL_Color couleur, double masse, double l0, double raideur, double amortissement, double dLiaisonInv, double lLiaisonMax,int exposantLiaison)
 : m_couleur(couleur), m_masse(masse), m_l0(l0), m_raideur(raideur), m_amor(amortissement),
   m_dLiaisonInv(dLiaisonInv), m_lLiaisonMax(lLiaisonMax), m_lLiaisonSuppr(2*lLiaisonMax), m_exposantLiaison(exposantLiaison)
{
    double d=dLiaisonInv;
    double k=raideur;
    double f=def::fLiaisonMax;
    double p=exposantLiaison;

    m_a=-pow(d,p+1)*k*p*(d*d*k*k-2*d*k*k*l0+k*k*l0*l0-2*d*f*k+2*f*k*l0+f*f)/((d*k*p-k*l0*p-d*k-f*p)*(d*k*p-k*l0*p-d*k-f*p));
    m_b=pow(d,p+1)/(d*k*p-k*l0*p-d*k-f*p);
    m_c=k*(d*d*k*p-2*d*k*l0*p+k*l0*l0*p-d*f*p+f*l0*p-d*f)/(d*k*p-k*l0*p-d*k-f*p);
}

void Matiere::forceLiaison(Particule* p, LiaisonPart& lp) const
{
    Vecteur ab(p->getPos(),lp.part->getPos());
    Vecteur vr(p->getV(), lp.part->getV());

    double l = ab.norme();

    if (l >= m_lLiaisonSuppr)
    {
        // Supprime la liaison
        lp.part->delier(p);
        lp = LiaisonPart();
        if (!lp.bris)
            throw Brisure(p,lp.part);
    }
    else if (l >= m_lLiaisonMax)
    {
        // TODO Retravailler l'expression de la force
        Vecteur f = m_raideur*m_l0*(ab.unitaire()); // Force répulsive, pas d'amortissement
        p->appliquerForce(f);
        if (!lp.bris)
            throw Brisure(p,lp.part);
    }
    else if (l >= m_dLiaisonInv) // Force proportionnelle + amortissement
    {
        Vecteur f = m_raideur*(l-m_l0)*(ab.unitaire()) + m_amor*vr;
        p->appliquerForce(f);
    }
    else // Force en 1/r^exposant
    {
        Vecteur f = (m_a/(m_b+pow(l,m_exposantLiaison))+m_c)*(ab.unitaire()) + m_amor*vr;
        p->appliquerForce(f);
    }
}
