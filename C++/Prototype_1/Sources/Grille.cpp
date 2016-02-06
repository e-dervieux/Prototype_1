#include "../Header/Grille.h"

Grille::Grille(int w, int h, int smW, int smH)
 : m_w(w), m_h(h), m_smW(smW), m_smH(smH),
   m_mat((int)ceil((double)w/(double)smW), (int)ceil((double)h/(double)smH), smW, smH)
{

}

Grille::~Grille()
{

}
