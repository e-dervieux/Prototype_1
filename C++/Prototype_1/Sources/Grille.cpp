#include "../Header/Grille.h"

Grille::Grille(int w, int h, int smW, int smH)
 : m_w(w), m_h(h), m_smW(smW), m_smH(smH),
   m_mat((int)ceil((double)w/(double)smW), (int)ceil((double)h/(double)smH), smW, smH)
{

}

Grille::~Grille()
{

}

void Grille::actualiser(double dt)
{
    //Calculer la force à appliquer et l'appliquer à chaque particule
    m_mat.forcesLiaison();

    //Modifier les coordonnées de ces particules
    m_mat.calculerDeplacement(dt);

    //Deplacer effectivement ces coordonnées dans la grille
    m_mat.deplacer();

}