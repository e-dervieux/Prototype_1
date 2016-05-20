#ifndef PROTOTYPE_1_MATRICEP_H
#define PROTOTYPE_1_MATRICEP_H

#include "MatriceCreuse.h"

// Classe définie à partir de CouchesParticules(MatriceCreuse)
// Les méthodes de gestion des particules sont hybrides : soit à partir du tableau de particules,
// soit en parcours en profondeur
template<size_t ...dims>
class MatriceParticules : public mcprive::MatriceCreuse<dims...>
{
    using Parent = mcprive::MatriceCreuse<dims...>;

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
        LiaisonPart* l = p.getLiaisons(); // l est normalement toujours correct (non NULL, etc...)

        for(int i = 0 ; i < p.getNbL() ; i++)
        {
            Particule* p2 = l[i].part;
            if (p2 != NULL && !l[i].bris && estValide(*p2))
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
            {
                std::list<Brisure> l = p.appliquerForcesLiaison();

                // Traitement des brisures
                for(std::list<Brisure>::iterator it = l.begin() ; it != l.end() ; it++)
                    lier(it->p1->getXInt(), it->p1->getYInt(), it->p2->getXInt(), it->p2->getYInt(), -2);
            }
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
                    else
                    {
                        // TODO revoir les tests à effectuer
                        mcprive::Conteneur* sm = this->getSM(xNouvPart,yNouvPart,m_coucheCol);
                        if (m_coucheCol > 0 && sm != NULL && !sm->estVide() && collisionSM(xOldPart,yOldPart,xNouvPart,yNouvPart))
                        {
                            const mcprive::LiaisonsMC& l = sm->getL();

                            int sx = xNouvPart/m_dimCol;
                            int sy = yNouvPart/m_dimCol;
                            p.collision(*sm, sx*m_dimCol, sy*m_dimCol, m_dimCol,
                                        l.get(mcprive::dir::haut) > 0,
                                        l.get(mcprive::dir::gauche) > 0,
                                        l.get(mcprive::dir::bas) > 0,
                                        l.get(mcprive::dir::droite) > 0);

                            // Si collision, la particule ne bouge pas
                            this->set(xOldPart, yOldPart, &p);
                        }
                        else if (sm != NULL)
                        {
                            // Collision au niveau des particules
                            Particule* p2 = this->get(xNouvPart,yNouvPart);
                            if (p2 != NULL)
                            {
                                p.collision(*p2);
                                this->set(xOldPart, yOldPart, &p);
                            }
                            else
                            {
                                // S'il n'y a pas eu de collision, on bouge la particule dans la grille (pourrait être fait dans gererCollision() ?)

                                // On ne redéfinit les liaisons entre sous-matrices que si les positions diffèrent de sous-matrice
                                // à la couche de collision
                                bool redefLiaisons =
                                        xOldPart/m_dimCol != xNouvPart/m_dimCol ||
                                        yOldPart/m_dimCol != yNouvPart/m_dimCol;

                                if (redefLiaisons)
                                    lier(p, -2);
                                p.setInt(xNouvPart, yNouvPart);
                                if (redefLiaisons)
                                    lier(p, 2);
                                this->set(xNouvPart, yNouvPart, &p);
                            }
                        }
                        else // sm == NULL
                        {
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

    // Pour l'instant, les matrices de collision sont toujours adjacentes (ou en diagonale)
    bool collisionSM(int x1, int y1, int x2, int y2)
    {
        if (this->m_tab == NULL)
            return false;

        // Coordonnées des SM
        int sx1 = x1 / m_dimCol;
        int sy1 = y1 / m_dimCol;
        int sx2 = x2 / m_dimCol;
        int sy2 = y2 / m_dimCol;

        int xMin = (x1 <= x2) ? x1 : x2;
        int xMax = (x1 <= x2) ? x2 : x1;
        int yMin = (y1 <= y2) ? y1 : y2;
        int yMax = (y1 <= y2) ? y2 : y1;

        if (sx1 == sx2)
        {
            if (sy1 == sy2) // Même sous-matrice
                return false;
            else // En bas
            {
                mcprive::Conteneur* sm = this->getSM(xMin, yMin,m_coucheCol);
                return (sm == NULL) ? false : (sm->getL().get(mcprive::dir::bas) <= 0);
            }
        }
        else
        {
            if (sy1 == sy2) // A droite
            {
                mcprive::Conteneur* sm = this->getSM(xMin, yMin,m_coucheCol);
                return (sm == NULL) ? false : (sm->getL().get(mcprive::dir::droite) <= 0);
            }
            else // Diagonale
            {
                mcprive::Conteneur* sm1 = this->getSM(x1,y1, m_coucheCol); // Normalement sm1 != NULL
                mcprive::Conteneur* sm2 = this->getSM(x2,y2, m_coucheCol); // Là on ne sait pas
                bool sm2Vide = (sm2==NULL) ? true : sm2->estVide();
                const mcprive::LiaisonsMC& liaisons = sm1->getL();

                if (sx1 < sx2 && sy1 < sy2) // Diagonale bas-droite
                    return collisionSMDiag(liaisons, sm2, sm2Vide, x1, y1, x2, y2,
                                    mcprive::dir::bas, mcprive::dir::droite, mcprive::dir::haut, mcprive::dir::gauche);
                else if (sx1 < sx2) // Diagonale haut-droite
                    return collisionSMDiag(liaisons, sm2, sm2Vide, x1, y1, x2, y2,
                                    mcprive::dir::haut, mcprive::dir::droite, mcprive::dir::bas, mcprive::dir::gauche);
                else if (sy1 < sy2) // Bas-gauche
                    return collisionSMDiag(liaisons, sm2, sm2Vide, x1, y1, x2, y2,
                                    mcprive::dir::bas, mcprive::dir::gauche, mcprive::dir::haut, mcprive::dir::droite);
                else // Haut-gauche
                    return collisionSMDiag(liaisons, sm2, sm2Vide, x1, y1, x2, y2,
                                    mcprive::dir::haut, mcprive::dir::gauche, mcprive::dir::bas, mcprive::dir::droite);
            }
        }
    }

    // Pour l'instant, les matrices de collision sont toujours adjacentes !!
    void lier(int x1, int y1, int x2, int y2, int nb)
    {
        if (this->m_tab == NULL)
            return;

        // Coordonnées des SM
        int sx1 = x1/m_dimCol;
        int sy1 = y1/m_dimCol;
        int sx2 = x2/m_dimCol;
        int sy2 = y2/m_dimCol;

        int xMin = (x1 <= x2) ? x1 : x2;
        int xMax = (x1 <= x2) ? x2 : x1;
        int yMin = (y1 <= y2) ? y1 : y2;
        int yMax = (y1 <= y2) ? y2 : y1;

        if (sx1 == sx2)
        {
            if (sy1 == sy2) // Même sous-matrice
                return;
            else // En bas
            {
                this->lierSM(xMin,yMin,mcprive::dir::bas,nb,m_coucheCol);
                this->lierSM(xMax,yMax,mcprive::dir::haut,nb,m_coucheCol);
            }
        }
        else
        {
            if (sy1 == sy2) // A droite
            {
                this->lierSM(xMin, yMin, mcprive::dir::droite, nb, m_coucheCol);
                this->lierSM(xMax, yMax, mcprive::dir::gauche, nb, m_coucheCol);
            }
            else if ( (x1 < x2 && y1 < y2) || (x1 > x2 && y1 > y2) ) // Bas-droite
            {
                this->lierSM(xMin,yMin, mcprive::dir::bas|mcprive::dir::droite, nb, m_coucheCol);
                this->lierSM(xMax,yMax, mcprive::dir::haut|mcprive::dir::gauche, nb, m_coucheCol);
            }
            else // Bas-gauche
            {
                this->lierSM(xMin,yMax, mcprive::dir::haut|mcprive::dir::droite, nb, m_coucheCol);
                this->lierSM(xMax,yMin, mcprive::dir::bas|mcprive::dir::gauche, nb, m_coucheCol);
            }
        }
    }

    void calculerDeplacement(double dt)
    {
        for(int i = 0 ; i < m_nbPart ; i++)
            m_part[i].calculerDeplacement(dt);
    }

    void croisementLiaisons()
    {
        for(int i = 0 ; i < m_nbPart ; i++)
            m_part[i].croisementLiaisons();
    }

    // Calcule la frame suivante, à partir des méthodes ci-dessus
    void actualiser(double dt)
    {
        for(int i = 0 ; i < def::nbIterationsEuler ; i++)
        {
            // Calculer la force à appliquer et l'appliquer à chaque particule
            forcesLiaison();

            // Calculer les positions à partir de ces nouvelles positions
            calculerDeplacement(dt/def::nbIterationsEuler);

            // Eviter que les liaisons entre particules ne se croisent
            croisementLiaisons();
        }

        // Modifier les coordonnées de ces particules
        deplacer(dt);

        // Actualisation des barycentres (plus besoin !)
        //this->actualiserBarycentre();

        // Actualisation des allocations mémoire
        this->actualiserAlloc();
    }

    void afficherLiaisons(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule)
    {
        if (def::liaisonsSMAffichees)
            this->afficherLiaisonsSM(rendu, m_coucheCol, tailleParticule,0,0);

        if (def::liaisonsAffichees)
        {
            for(int i = 0 ; i < m_nbPart ; i++)
                m_part[i].afficherLiaisons(rendu, coucheAffichage, tailleParticule);
        }
    }

private:
    bool collisionSMDiag(mcprive::LiaisonsMC const& liaisons, mcprive::Conteneur* sm2, bool sm2Vide,
                         int x1, int y1, int x2, int y2,
                         mcprive::Direction dir1, mcprive::Direction dir2, mcprive::Direction dir3, mcprive::Direction dir4)
    {
        if (liaisons.get(dir1 | dir2)) // Liaison déjà existante
            return false;
        else if (!sm2Vide)
        {
            // Passage par une matrice intermédiaire
            if (liaisons.get(dir1))
            {
                mcprive::Conteneur *sm3 = this->getSM(x1, y2, m_coucheCol); // Normalement non vide
                if (sm3 != NULL || sm3->getL().get(dir2))
                    return false;
            }
            if (liaisons.get(dir2))
            {
                mcprive::Conteneur *sm3 = this->getSM(x2, y1, m_coucheCol);
                if (sm3 != NULL || sm3->getL().get(dir1))
                    return false;
            }
            return true;
        }
        else if (liaisons.get(dir1) || liaisons.get(dir2)) // Idem, mais lorsque sm2 est vide
            return false;
        else
        {
            // Teste s'il y a une liaison sur la diagonale transversale
            mcprive::Conteneur *sm3 = this->getSM(x1, y2, m_coucheCol);
            bool sm3Vide = (sm3 == NULL) ? true : sm3->estVide();
            if (sm3Vide)
                return false;
            else
            {
                sm3 = this->getSM(x2, y1, m_coucheCol);
                sm3Vide = (sm3 == NULL) ? true : sm3->estVide();
                if (sm3Vide)
                    return false;
                else
                    return (sm3->getL().get(dir3 | dir4) > 0);
            }
        }
    }

    Particule* m_part; // Tableau des particules à gérer
    int m_nbPart; // Nombre de particules dans le tableau
    int m_coucheCol;
    int m_dimCol;
};

#endif //PROTOTYPE_1_MATRICEP_H
