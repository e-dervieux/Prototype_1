#include "../Header/MatriceParticules.h"

MatriceParticules::MatriceParticules(int mpX, int mpY, int smX, int smY)
 : MatriceCreuse(smX, smY, mpX,mpY, def::testPart, def::defautPart)
{

}

void MatriceParticules::forcesLiaison()
{
    for(int i = 0 ; i < m_dimMPX*m_dimMPY ; i++)
    {
        SousMatrice& mat = m_tabSM[i];
        if (mat != NULL)
        {
            for(int j = 0 ; j < m_dimSMX*m_dimSMY ; j++)
            {
                Particule& p = mat[j];

                if (!m_estNul(p))
                {
                    // A FAIRE !
                }
            }
        }
    }
}

void MatriceParticules::calculerDeplacement(double dt)
{
    for(int i = 0 ; i < m_dimMPX*m_dimMPY ; i++)
    {
        SousMatrice& mat = m_tabSM[i];
        if(mat != NULL)
        {
            for(int j = 0 ; j < m_dimSMX*m_dimSMY ; j++)
            {
                Particule& p = mat[j];
                p.calculerDeplacement(dt);
            }
        }
    }
}

void MatriceParticules::deplacer()
{
    for(int i = 0 ; i < m_dimMPX*m_dimMPY ; i++)
    {
        SousMatrice& mat = m_tabSM[i];
        if (mat != NULL)
        {
            for(int j = 0 ; j < m_dimSMX*m_dimSMY ; j++)
            {
                Particule& p = mat[j];
                if (!m_estNul(p))
                {
                    Vecteur pos = p.getPos(); // Position o� mettre le pixel
                    int x = (int)pos.getX();
                    int y = (int)pos.getY();

                    // Tenter de mettre la particule aux coordonn�es (x,y)
                    // A FAIRE !!
                }
            }
        }
    }
}

void MatriceParticules::afficher(SDL_Renderer* rendu)
{
    // A FAIRE !!
}
