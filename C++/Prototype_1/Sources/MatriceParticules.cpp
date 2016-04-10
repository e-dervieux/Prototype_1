#include "../Header/MatriceParticules.h"

int nbC = 0;

MatriceParticules::MatriceParticules(int w, int h, int smX, int smY, Particule* particules, int nbParticules)
 : m_w(w), m_h(h), m_mpX((int)ceil((double)w/(double)smX)), m_mpY((int)ceil((double)h/(double)smY)), m_smX(smX), m_smY(smY), m_part(particules), m_nbPart(nbParticules)
{
    // Initialisation des tableaux
    m_tabSM = new Particule**[m_mpX*m_mpY];
    m_tabCnt = new int[m_mpX*m_mpY];
    for(int i = 0 ; i < m_mpX*m_mpY ; i++)
    {
        m_tabSM[i]=NULL;
        m_tabCnt[i]=0;
    }

    // Ajout des particules
    ajouterParticules();
}

MatriceParticules::~MatriceParticules()
{
    // Libération des sous-matrices
    for(int i = 0 ; i < m_mpX*m_mpY ; i++)
    {
        if(m_tabSM[i] != NULL)
            delete[] m_tabSM[i];
    }
    delete[] m_tabSM;
    delete[] m_tabCnt;
}

void MatriceParticules::ajouterParticules()
{
    for(int i = 0 ; i < m_nbPart ; i++)
    {
        Particule* p = &m_part[i];
        if (estValide(*p))
            set(p->getXInt(), p->getYInt(), p);
        else
            p->supprimerLiaisons();
    }
}

void MatriceParticules::reinit()
{
    // Suppression du contenu de la matrice Creuse
    for(int i = 0 ; i < m_mpX*m_mpY ; i++)
    {
        if (m_tabSM[i] != NULL)
        {
            delete[] m_tabSM[i];
            m_tabSM[i] = NULL;
            m_tabCnt[i] = 0;
        }
    }

    // Rajout des particules
    ajouterParticules();
}

bool MatriceParticules::estValide(Particule &p)
{
    int x = p.getXInt();
    int y = p.getYInt();
    return (x >= 0 && x < m_w && y >= 0 && y < m_h);
}

void MatriceParticules::forcesLiaison()
{
    for(int i = 0 ; i < m_nbPart ; i++)
    {
        Particule& p = m_part[i];
        if (estValide(p))
            p.appliquerForcesLiaison();
    }
}

void MatriceParticules::calculerDeplacement(double dt)
{
    for(int i = 0 ; i < m_nbPart ; i++)
    {
            Particule& p = m_part[i];
            if (estValide(p))
                p.calculerDeplacement(dt);
    }
}

void MatriceParticules::deplacer(double dt)
{
    for(int i = 0 ; i < m_nbPart ; i++)
    {
        Particule& p = m_part[i];
        if (estValide(p))
        {
            int xOldPart = p.getXInt();
            int yOldPart = p.getYInt();

            Vecteur pos = p.getPos(); // Position où mettre le pixel
            int xNouvPart = (int)pos.getX();
            int yNouvPart = (int)pos.getY();

            // On tente de mettre la particule aux coordonnees (x,y)

            // Seulement si la particule bouge :
            if (xOldPart != xNouvPart || yOldPart != yNouvPart)
            {
                // Si la particule sort déjà de la grille
                if (xNouvPart < 0 || xNouvPart >= m_mpX*m_smX || yNouvPart < 0 || yNouvPart >= m_mpY*m_smY)
                {
                    p.supprimerLiaisons();
                    suppr(xOldPart,yOldPart);
                }
                else
                {
                    // Cette boucle sera la partie à améliorer pour gérer convenablement les collisions
                    Particule* p2 = get(xNouvPart,yNouvPart);
                    if (p2 != NULL)
                        p.collision(*p2, dt);
                    else
                    {
                        // On bouge les coordonnées entières de la particule
                        p.setInt(xNouvPart, yNouvPart);

                        // Si on sort de la grille...
                        if (!estValide(p))
                            p.supprimerLiaisons();
                        else
                            set(xNouvPart, yNouvPart, &p);

                        suppr(xOldPart, yOldPart);
                    }
                }
            }
        }
    }
}

void MatriceParticules::actualiser(double dt)
{
    // Calculer la force à appliquer et l'appliquer à chaque particule
    forcesLiaison();

    // Modifier les coordonnées de ces particules
    calculerDeplacement(dt);

    // Deplacer effectivement ces coordonnées dans la grille
    deplacer(dt);
}

void MatriceParticules::afficher(SDL_Renderer* rendu, int partPP, int taillePixel)
{
    // Pour l'instant, on suppose que les sous-matrices sont carrées !!!

    /* 3 cas de figure :
        - taille d'une SM == taille d'un pixel (optimisation du 2nd cas)
        - taille d'une SM > taille d'un pixel
        - taille d'une SM < taille d'un pixel*/
    if (m_smX == partPP)
    {
        int xg = 0, yg = 0; // x et y du pixel à afficher
        for(int i = 0 ; i < m_mpX*m_mpY ; i++)
        {
            Particule**& mat = m_tabSM[i];
            if (mat != NULL)
            {
                int rtot=0, gtot=0, btot=0;
                // On pourrait encore optimiser cette boucle en comparant le nombre de particules traitées et m_tabCnt[i]
                for(int j = 0 ; j < m_smX*m_smY ; j++)
                {
                    Particule*& p = mat[j];
                    if (p != NULL)
                    {
                        SDL_Color c = p->getCouleur();
                        rtot += c.r;
                        gtot += c.g;
                        btot += c.b;
                    }
                }

                int nb = m_tabCnt[i];
                SDL_SetRenderDrawColor(rendu, (Uint8)(rtot/nb),(Uint8)(gtot/nb),(Uint8)(btot/nb), 255);
                SDL_Rect rect = {taillePixel*xg, taillePixel*yg,taillePixel,taillePixel};
                SDL_RenderFillRect(rendu, &rect);
            }

            yg++;
            if (yg == m_mpY)
            {
                yg = 0;
                xg++;
            }
        }
    }
    else if (m_smX > partPP)
    {
        // Parcours de sous-matrices
        for(int i = 0 ; i < m_mpX*m_mpY ; i++)
        {
            Particule**& mat = m_tabSM[i];
            if (mat != NULL)
            {
                // Parcours de pixels dans les sous-matrices
                for(int xp = 0 ; xp < m_smX / partPP; xp++)
                {
                    for(int yp = 0 ; yp < m_smY / partPP; yp++)
                    {
                        int rtot = 0, gtot = 0, btot = 0;
                        int nb = 0; // Nombre de particules dans le pixel

                        // Parcours des particules dans le pixel
                        for(int x = 0 ; x < partPP; x++)
                        {
                            for(int y = 0 ; y < partPP; y++)
                            {
                                Particule*& p = mat[(xp * partPP + x) * m_smY + (yp * partPP + y)];
                                if (p != NULL)
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
                            int xg = (i/m_mpY)*(m_smX / partPP) + xp;
                            int yg = (i%m_mpY)*(m_smY / partPP) + yp;
                            SDL_SetRenderDrawColor(rendu, (Uint8)(rtot/nb),(Uint8)(gtot/nb),(Uint8)(btot/nb),255);
                            SDL_Rect rect = {taillePixel*xg, taillePixel*yg,taillePixel,taillePixel};
                            SDL_RenderFillRect(rendu, &rect);
                        }
                    }
                }
            }
        }
    }
    else // m_smX < partPP
    {
        // Utile ?
    }
}

void MatriceParticules::afficherLiaisons(SDL_Renderer* rendu, int partPP, int taillePixel)
{
    SDL_SetRenderDrawColor(rendu, 0,255,0,60);

    for(int i = 0 ; i < m_nbPart ; i++)
        m_part[i].afficherLiaisons(rendu, partPP, taillePixel);
}

void MatriceParticules::set(int x, int y, Particule *p)
{
    if (x < 0 || x >= m_mpX*m_smX || y < 0 || y >= m_mpY*m_smY)
        return;

    int indSM = (x/m_smX)*m_mpY + (y/m_smY);
    Particule**& sm = m_tabSM[indSM];
    if (sm == NULL) // Si la sous-matrice associée n'existe pas encore, la créer
    {
        sm = new Particule*[m_smX*m_smY];
        for(int i = 0 ; i < m_smX*m_smY ; i++)
            sm[i] = NULL;
    }

    // Test si la coordonnée est libre, pour bien compter le nombre de particules dans la SM
    Particule*& tmp = sm[(x%m_smX)*m_smY+(y%m_smY)];
    if (tmp == NULL)
        m_tabCnt[indSM]++;

    tmp = p;
}

void MatriceParticules::suppr(int x, int y)
{
    int indSM = (x/m_smX)*m_mpY + (y/m_smY);
    Particule**& sm = m_tabSM[indSM];
    if (sm == NULL) // (x,y) est déjà vide...
        return;

    // Test si la coordonnée est occupée, pour bien compter le nombre de particules dans la SM
    Particule*& tmp = sm[(x%m_smX)*m_smY+(y%m_smY)];
    if (tmp == NULL)
        return;

    tmp = NULL;
    m_tabCnt[indSM]--;

    // Si la SM est devenue vide
    if (m_tabCnt[indSM] == 0)
    {
        delete[] sm;
        sm = NULL;
    }
}

bool MatriceParticules::estVide(int x, int y)
{
    if (x < 0 || x >= m_mpX*m_smX || y < 0 || y >= m_mpY*m_smY)
        return true;

    int indSM = (x/m_smX)*m_mpY + (y/m_smY);
    Particule** sm = m_tabSM[indSM];
    if (sm == NULL)
        return true;

    Particule* tmp = sm[(x%m_smX)*m_smY+(y%m_smY)];
    return (tmp == NULL);
}

Particule* MatriceParticules::get(int x, int y)
{
    int indSM = (x/m_smX)*m_mpY + (y/m_smY);
    Particule** sm = m_tabSM[indSM];
    if (sm == NULL)
            return NULL;
    return sm[(x%m_smX)*m_smY+(y%m_smY)];
}