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
                    //TODO
                    for(int i=0;i<def::nbLiaisons;i++)
                    {
                        if(p.m_liaisons[i] != NULL)
                        {
                            p.appliquerForce(p.m_matiere->forceLiaison(p,*p.m_liaisons[i]));
                        }
                    }
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
                    Vecteur pos = p.getPos(); // Position ou mettre le pixel
                    int xNouvPart = (int)pos.getX();
                    int yNouvPart = (int)pos.getY();

                    // Tenter de mettre la particule aux coordonnees (x,y)
                    bool aEteModifie = false;

                    //Tant que la place n'est pas libre
                    while(!this->estNul(xNouvPart, yNouvPart))
                    {
                        xNouvPart++;
                        yNouvPart++;
                        aEteModifie = true;
                    }

                    //On crée ensuite une copie de p qu'on place en coordonnées (xNouvPart;yNouvPart)
                    //Si jamais erreur c'est que set ne copie pas !
                    this->set(xNouvPart, yNouvPart, p);

                    //Si jamais on a modifié les coordonnées dans la matrices par rapport
                    //Aux coordonnées "vraies" calculées, alors on accorde les coordonnées
                    //Double avec les entières.
                    if(aEteModifie)
                    {
                        this->get(xNouvPart, yNouvPart).m_pos=Vecteur((double)xNouvPart,(double)yNouvPart);
                    }

                    //On met à jour les liaisons des autres particules.
                    for(int i=0;i<def::nbLiaisons;i++)
                    {
                        if(p.m_liaisons[i]!=NULL)
                        {
                            for(int j=0;j<def::nbLiaisons;j++)
                            {
                                if (p.m_liaisons[i]->m_liaisons[j] == &p)
                                {
                                    p.m_liaisons[i]->m_liaisons[j]=&this->get(xNouvPart,yNouvPart);
                                    j=def::nbLiaisons;
                                }
                            }
                        }
                    }
                    //On supprime finalement p
                    this->suppr(p.m_x,p.m_y);
                }
            }
        }
    }
}

void MatriceParticules::afficher(SDL_Renderer* rendu)
{
    //TODO
}
