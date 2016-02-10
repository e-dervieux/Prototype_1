#include "../Header/MatriceParticules.h"

MatriceParticules::MatriceParticules(int mpX, int mpY, int smX, int smY)
 : MatriceCreuse(smX, smY, mpX,mpY, def::testPart, def::defautPart, def::supprPart)
{

}

void MatriceParticules::ajouterPart(std::vector<Particule>& particules)
{
    for(int i = 0 ; i < particules.size() ; i++)
    {
        Particule& p = particules[i];
        set(p.getXInt(),p.getYInt(),&particules[i]);
    }
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
                Particule*& p = mat[j];

                if (!m_estNul(p))
                {
                    p->appliquerForcesLiaison();
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
                Particule*& p = mat[j];
                if (!m_estNul(p))
                    p->calculerDeplacement(dt);
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
                Particule*& p = mat[j];
                if (!m_estNul(p))
                {
                    int xOldPart = p->getXInt();
                    int yOldPart = p->getYInt();

                    Vecteur pos = p->getPos(); // Position ou mettre le pixel
                    int xNouvPart = (int)pos.getX();
                    int yNouvPart = (int)pos.getY();

                    // Tenter de mettre la particule aux coordonnees (x,y)
                    bool aEteModifie = false;

                    // Seulement si la particule bouge :
                    if (xOldPart != xNouvPart || yOldPart != yNouvPart)
                    {
                        //Tant que la place n'est pas libre
                        // Cette boucle sera la partie à améliorer pour gérer convenablement les collisions
                        while (!this->estNul(xNouvPart, yNouvPart)) {
                            xNouvPart++;
                            yNouvPart++;
                            aEteModifie = true;
                        }

                        // Si on sort de la grille, on supprime la particule
                        if (xNouvPart < 0 || xNouvPart >= m_dimMPX * m_dimSMX || yNouvPart < 0 || yNouvPart >= m_dimMPY * m_dimSMY)
                        {
                            p->supprimerLiaisons();
                            delete p; // Supprime-t-on la particule de la mémoire ici ? (cf stockage des particules)
                            this->suppr(xOldPart, yOldPart);
                        }
                        else {
                            this->set(xNouvPart, yNouvPart, p);
                            p->setPosInt(xNouvPart, yNouvPart);

                            //Si jamais on a modifié les coordonnées dans la matrice par rapport
                            //Aux coordonnées "vraies" calculées, alors on accorde les coordonnées
                            //Double avec les entières.
                            if (aEteModifie) {
                                p->setPos(Vecteur((double) xNouvPart + 0.5, (double) yNouvPart + 0.5));
                            }

                            //On supprime finalement p
                            this->suppr(xOldPart, yOldPart);

                            if (mat == NULL)
                                break;
                        }
                    }
                }
            }
        }
    }
}

void MatriceParticules::afficher(SDL_Renderer* rendu)
{
    // Pour l'instant, on suppose que les sous-matrices sont carrées !!!

    /* 3 cas de figure :
        - taille d'une SM == taille d'un pixel (optimisation du 2nd cas)
        - taille d'une SM > taille d'un pixel
        - taille d'une SM < taille d'un pixel*/
    if (m_dimSMX == def::partPP)
    {
        int xg = 0, yg = 0; // x et y du pixel à afficher
        for(int i = 0 ; i < m_dimMPX*m_dimMPY ; i++)
        {
            SousMatrice& mat = m_tabSM[i];
            if (mat != NULL)
            {
                int rtot=0, gtot=0, btot=0;
                // On pourrait encore optimiser cette boucle en comparant le nombre de particules traitées et m_tabCnt[i]
                for(int j = 0 ; j < m_dimSMX*m_dimSMY ; j++)
                {
                    Particule*& p = mat[j];
                    if (!m_estNul(p))
                    {
                        SDL_Color c = p->getCouleur();
                        rtot += c.r;
                        gtot += c.g;
                        btot += c.b;
                    }
                }

                int nb = m_tabCnt[i];
                SDL_SetRenderDrawColor(rendu, (Uint8)(rtot/nb),(Uint8)(gtot/nb),(Uint8)(btot/nb),255);
                SDL_Rect rect = {def::taillePixel*xg, def::taillePixel*yg,def::taillePixel,def::taillePixel};
                SDL_RenderFillRect(rendu, &rect);
            }

            yg++;
            if (yg == m_dimMPX)
            {
                yg = 0;
                xg++;
            }
        }
    }
    else if (m_dimSMX > def::partPP)
    {
        // Parcours de sous-matrices
        for(int i = 0 ; i < m_dimMPX*m_dimMPY ; i++)
        {
            SousMatrice& mat = m_tabSM[i];
            if (mat != NULL)
            {
                // Parcours de pixels dans les sous-matrices
                for(int xp = 0 ; xp < m_dimSMX/def::partPP ; xp++)
                {
                    for(int yp = 0 ; yp < m_dimSMY/def::partPP ; yp++)
                    {
                        int rtot = 0, gtot = 0, btot = 0;
                        int nb = 0; // Nombre de particules dans le pixel

                        // Parcours des particules dans le pixel
                        for(int x = 0 ; x < def::partPP ; x++)
                        {
                            for(int y = 0 ; y < def::partPP ; y++)
                            {
                                Particule*& p = mat[(xp*def::partPP+x)*m_dimSMY+(yp*def::partPP+y)];
                                if (!m_estNul(p))
                                {
                                    SDL_Color c = p->getCouleur();
                                    rtot += c.r;
                                    gtot += c.g;
                                    btot += c.b;
                                    nb++;
                                }
                            }
                        }

                        // Affichage si le pixel n'est pas vide
                        if (nb != 0)
                        {
                            int xg = (i/m_dimMPY)*(m_dimSMX/def::partPP) + xp;
                            int yg = (i%m_dimMPY)*(m_dimSMY/def::partPP) + yp;
                            SDL_SetRenderDrawColor(rendu, (Uint8)(rtot/nb),(Uint8)(gtot/nb),(Uint8)(btot/nb),255);
                            SDL_Rect rect = {def::taillePixel*xg, def::taillePixel*yg,def::taillePixel,def::taillePixel};
                            SDL_RenderFillRect(rendu, &rect);
                        }
                    }
                }
            }
        }
    }
    else // m_dimSMX < def::partPP
    {
        // Utile ?
    }
}
