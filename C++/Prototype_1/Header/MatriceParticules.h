#ifndef PROTOTYPE_1_MATRICEP_H
#define PROTOTYPE_1_MATRICEP_H

#include "MatriceCreuse.h"

// Classe définie à partir de CouchesParticules(MatriceCreuse)
// Les méthodes de gestion des particules sont hybrides : soit à partir du tableau de particules,
// soit en parcours en profondeur
template<size_t ...dims>
class MatriceParticules : public MatriceCreuse<dims...>
{
    using Parent = MatriceCreuse<dims...>;

public:
    MatriceParticules(size_t w, size_t h, int coucheCol, Particule* particules, int nbParticules)
     : Parent(w,h), m_part(particules), m_nbPart(nbParticules),
       m_coucheCol(coucheCol), m_dimCol(Parent::getDim(coucheCol))
    {
        ajouterParticules();
        this->actualiserBarycentre();
    }

    // Ajoute les particules de m_particules dans la matrice
    void ajouterParticules()
    {
        for(int i = 0 ; i < m_nbPart ; i++)
        {
            Particule* p = &m_part[i];
            if (estValide(*p))
            {
                this->set(p->getXInt(), p->getYInt(), p);
                lier(*p,1);
            }
            else
                p->supprimerLiaisons();
        }
    }

    // Réinitialise la matrice de particules à partir du tableau
    // afin qu'elle soit gérée de manière correcte.
    virtual void reinit()
    {
        // Suppression du contenu de la matrice Creuse
        Parent::reinit();

        // Rajout des particules dans la matrice
        ajouterParticules();
        this->actualiserBarycentre();

        // Suppression des forces rémanentes
        for(int i = 0 ; i < m_nbPart ; i++)
            m_part[i].annulerForces();
    }

    // Indique si une particule est bien dans la matrice
    bool estValide(Particule& p)
    {
        int x = p.getXInt();
        int y = p.getYInt();
        return (x >= 0 && x < this->m_w && y >= 0 && y < this->m_h);
    }

    // Ajoute/supprime les liaisons que contient la particule p
    void lier(Particule& p, int nb = 1)
    {
        Particule** l = p.getLiaisons(); // l est normalement toujours correct (non NULL, etc...)

        for(int i = 0 ; i < def::nbLiaisons ; i++)
        {
            Particule* p2 = l[i];
            if (p2 != NULL)
                lier(p.getXInt(), p.getYInt(), p2->getXInt(), p2->getYInt(), nb);
        }
    }

    // Calcule et applique les forces de liaison entre les particules
    void forcesLiaison()
    {
        for(int i = 0 ; i < m_nbPart ; i++)
        {
            Particule& p = m_part[i];
            if (estValide(p))
                p.appliquerForcesLiaison();
        }
    }

    // Effectue le déplacement des particules dans la matrice
    void deplacer(double dt)
    {
        for(int i = 0 ; i < m_nbPart ; i++)
        {
            Particule& p = m_part[i];
            if (estValide(p))
            {
                int xOldPart = p.getXInt();
                int yOldPart = p.getYInt();

                // Déplacement de la particule
                this->suppr(xOldPart,yOldPart);
                p.actualiser(dt);

                Vecteur pos = p.getPos(); // Nouvelle position où mettre le pixel
                int xNouvPart = (int)pos.getX();
                int yNouvPart = (int)pos.getY();

                // On tente de mettre la particule aux coordonnees (x,y)

                // Seulement si la particule bouge :
                if (xOldPart != xNouvPart || yOldPart != yNouvPart)
                {
                    // Si la particule sort de la grille
                    if (xNouvPart < 0 || xNouvPart >= this->m_w || yNouvPart < 0 || yNouvPart >= this->m_h)
                    {
                        // On supprime les liaisons
                        lier(p, -2);
                        p.supprimerLiaisons();
                    }
                    // Sinon, on cherche une collision au niveau des SM
                    else if (!collisionSM(xOldPart,yOldPart,xNouvPart,yNouvPart))
                    {
                        // Si collision, la particule ne bouge pas
                        this->set(xOldPart, yOldPart, &p);
                        // Normalement, getSM() renvoie une matrice existante (sinon, il n'y aurait pas de collision)
                        Conteneur* sm = this->getSM(xNouvPart,yNouvPart,m_coucheCol);
                        int sx = xNouvPart/m_dimCol;
                        int sy = yNouvPart/m_dimCol;
                        p.collision(*sm, sx*m_dimCol, sy*m_dimCol, m_dimCol);
                    }
                    else
                    {
                        // Collision au niveau des particules
                        Particule* p2 = this->get(xNouvPart,yNouvPart);
                        if (p2 != NULL)
                            p.collision(*p2,xNouvPart,yNouvPart,1);
                        else
                        {
                            // S'il n'y a pas eu de collision, on bouge la particule dans la grille (pourrait être fait dans gererCollision() ?)
                            lier(p, -2);
                            p.setInt(xNouvPart, yNouvPart);
                            lier(p, 2);
                            this->set(xNouvPart, yNouvPart, &p);
                        }
                    }
                }
                else
                    this->set(xOldPart, yOldPart, &p);
            }
        }
    }

    // Pour l'instant, les matrices de collision sont toujours adjacentes
    bool collisionSM(int x1, int y1, int x2, int y2)
    {
        if (this->m_tab == NULL)
            return false;

        // Coordonnées des SM
        int sx1 = x1 / m_dimCol;
        int sy1 = y1 / m_dimCol;
        int sx2 = x2 / m_dimCol;
        int sy2 = y2 / m_dimCol;

        if (sx1 == sx2)
        {
            if (sy1 == sy2) // Même sous-matrice
                return false;
            else if (sy1 < sy2) // En bas
                return (this->liaisonSMBas(x1, y1, m_coucheCol) <= 0 );
            else // En haut
                return (this->liaisonSMBas(x2, y2, m_coucheCol) <= 0 );
        }
        else if (sx1 < sx2)
        {
            if (sy1 == sy2) // A droite
                return this->liaisonSMDroite(x1, y1, m_coucheCol) <= 0;
            else if (sy1 < sy2) // En bas à droite
                return (this->liaisonSMDroite(x1, y1, m_coucheCol) <= 0 || this->liaisonSMBas(x2, y1, m_coucheCol) <= 0)
                       && (this->liaisonSMBas(x1, y1, m_coucheCol) <= 0 || this->liaisonSMDroite(x1, y2, m_coucheCol) <= 0);
            else // En haut à droite
                return (this->liaisonSMDroite(x1, y1, m_coucheCol) <= 0 || this->liaisonSMBas(x2, y2, m_coucheCol) <= 0)
                       && (this->liaisonSMBas(x1, y2, m_coucheCol) <= 0 || this->liaisonSMDroite(x1, y2, m_coucheCol) <= 0);
        }
        else // sx1 > sx2
        {
            if (sy1 == sy2) // A gauche
                return this->liaisonSMDroite(x2, y2, m_coucheCol) <= 0;
            else if (sy1 < sy2) // En bas à gauche
                return (this->liaisonSMDroite(x2, y1, m_coucheCol) <= 0 || this->liaisonSMBas(x2, y1, m_coucheCol) <= 0)
                       && (this->liaisonSMBas(x1, y1, m_coucheCol) <= 0 || this->liaisonSMDroite(x1, y2, m_coucheCol) <= 0);
            else // En haut à gauche
                return (this->liaisonSMDroite(x2, y1, m_coucheCol) <= 0 || this->liaisonSMBas(x2, y2, m_coucheCol) <= 0)
                       && (this->liaisonSMBas(x1, y2, m_coucheCol) <= 0 || this->liaisonSMDroite(x2, y2, m_coucheCol) <= 0);
        }
    }

    // Pour l'instant, les matrices de collision sont toujours adjacentes
    void lier(int x1, int y1, int x2, int y2, int nb)
    {
        if (this->m_tab == NULL)
            return;

        // Coordonnées des SM
        int sx1 = x1/m_dimCol;
        int sy1 = y1/m_dimCol;
        int sx2 = x2/m_dimCol;
        int sy2 = y2/m_dimCol;

        if (sx1 == sx2)
        {
            if (sy1 == sy2) // Même sous-matrice
                return;
            else if (sy1 < sy2) // En bas
                this->lierSMBas(x1,y1,nb,m_coucheCol);
            else // En haut
                this->lierSMBas(x2,y2,nb,m_coucheCol);
        }
        else if (sx1 < sx2)
        {
            if (sy1 == sy2) // A droite
                this->lierSMDroite(x1,y1,nb,m_coucheCol);
            else if (sy1 < sy2) // En bas à droite
            {
                this->lierSMDroite(x1,y1,nb,m_coucheCol); this->lierSMBas(x2,y1,nb,m_coucheCol);
                this->lierSMBas(x1,y1,nb,m_coucheCol); this->lierSMDroite(x1,y2,nb,m_coucheCol);
            }
            else // En haut à droite
            {
                this->lierSMDroite(x1,y1,nb,m_coucheCol); this->lierSMBas(x2,y2,nb,m_coucheCol);
                this->lierSMBas(x1,y2,nb,m_coucheCol); this->lierSMDroite(x1,y2,nb,m_coucheCol);
            }
        }
        else // sx1 > sx2
        {
            if (sy1 == sy2) // A gauche
                this->lierSMDroite(x2,y2,nb,m_coucheCol);
            else if (sy1 < sy2) // En bas à gauche
            {
                this->lierSMDroite(x2,y1,nb,m_coucheCol); this->lierSMBas(x2,y1,nb,m_coucheCol);
                this->lierSMBas(x1,y1,nb,m_coucheCol); this->lierSMDroite(x1,y2,nb,m_coucheCol);
            }
            else // En haut à gauche
            {
                this->lierSMDroite(x2,y1,nb,m_coucheCol); this->lierSMBas(x2,y2,nb,m_coucheCol);
                this->lierSMBas(x1,y2,nb,m_coucheCol); this->lierSMDroite(x2,y2,nb,m_coucheCol);
            }
        }
    }

    // Calcule la frame suivante, à partir des méthodes ci-dessus
    void actualiser(double dt)
    {
        // Calculer la force à appliquer et l'appliquer à chaque particule
        forcesLiaison();

        // Modifier les coordonnées de ces particules
        deplacer(dt);

        // Actualisation des barycentres (plus besoin !)
        //this->actualiserBarycentre();

        // Actualisation des allocations mémoire
        this->actualiserAlloc();
    }

    void afficherLiaisons(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule)
    {
        SDL_SetRenderDrawColor(rendu, 0,255,0,60);

        for(int i = 0 ; i < m_nbPart ; i++)
            m_part[i].afficherLiaisons(rendu, coucheAffichage, tailleParticule);
    }

private:
    Particule* m_part; // Tableau des particules à gérer
    int m_nbPart; // Nombre de particules dans le tableau
    int m_coucheCol;
    int m_dimCol;
};

#endif //PROTOTYPE_1_MATRICEP_H
