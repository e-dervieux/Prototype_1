#ifndef PROTOTYPE_1_MATRICECREUSE_H
#define PROTOTYPE_1_MATRICECREUSE_H

#include "Conteneur.h"

// TODO utiliser un namespace ! (voire un namespace dans un namespace)

// TODO Utiliser les variables de def plutôt que des arguments (cf coucheCollision, coucheAffichage, etc...)

// TODO Pouvoir utiliser le rendu de def pour afficher les collisions, etc...

// TODO Utiliser un test de préprocesseur pour les fonctions de déboggage (#ifdef DEBUG)

template<size_t ...dims>
class MatriceCreuse;

// Conteneur défini de manière récursive
template<>
template<size_t dimSM, size_t ...autres>
class MatriceCreuse<dimSM, autres...> : public Conteneur
{
    using SM = MatriceCreuse<autres...>;

public:
    MatriceCreuse<dimSM, autres...>(size_t w, size_t h)
     : Conteneur(), m_tab(NULL),
       m_w(w), m_h(h), m_smX((int)ceil((double)w/(double)dimSM)), m_smY((int)ceil((double)h/(double)dimSM)),
       m_nbLDroite(0), m_nbLBas(0)
    {}

    // Constructeur par défaut (utlisé uniquement par le conteneur !!!)
    MatriceCreuse<dimSM, autres...>()
     : Conteneur(), m_tab(NULL),
       m_nbLDroite(0), m_nbLBas(0)
    {}

    // Utilisé après pour définir les dimensions
    void setDim(size_t dim)
    {
        m_w = dim;
        m_h = dim;
        m_smX = dim/dimSM;
        m_smY = m_smX;
    }

    ~MatriceCreuse()
    {
        if (m_tab != NULL)
            delete [] m_tab;
    }

    static inline int getProfondeur() { return 1+SM::getProfondeur(); }

    // Redéfinit également le nombre de particules, la masse, etc...
    // /!\ N'est peut-être jamais utilisé !?
    virtual void actualiserBarycentre()
    {
        if (m_tab==NULL)
        {
            m_masse = 0.0;
            m_nbP = 0;
        }
        else
        {
            // Valeurs physiques
            m_masse = 0.0;
            m_pos = 0.0;
            m_v = 0.0;

            // Variables pour la couleur
            m_nbP = 0;
            double r = 0.0, g = 0.0, b = 0.0, a = 0.0;

            for(int i = 0 ; i < m_smX*m_smY ; i++)
            {
                m_masse += m_tab[i].getMasse();
                m_pos += m_tab[i].getPos();
                m_v += m_tab[i].getV();

                m_nbP += m_tab[i].getNbP();
                SDL_Color c = m_tab[i].getCouleur();
                r += (double)c.r;
                g += (double)c.g;
                b += (double)c.b;
                a += (double)c.a;
            }
            m_couleur = {
                    (Uint8)(r/(double)m_nbP),
                    (Uint8)(g/(double)m_nbP),
                    (Uint8)(b/(double)m_nbP),
                    (Uint8)(a/(double)m_nbP) };
        }
    }

    void actualiserAlloc()
    {
        if (m_tab == NULL)
            return;

        if (m_nbP == 0)
        {
            delete [] m_tab;
            m_tab = NULL;
            return;
        }

        for(int i = 0 ; i < m_smX*m_smY ; i++)
            m_tab[i].actualiserAlloc();
    }

    // Se charge de détecter et d'appliquer les collisions lorsque la particule p essaie de se déplacer aux coordonnées (x,y)
    // (x,y) sont des coordonnées absolues
    // Retourne true s'il y a eu collision
    bool gererCollision(Particule& p, int x, int y, int coucheCollision, bool colSMTrouvee = false, bool liaisonSM = false)
    {
        if (m_tab == NULL)
            return false;

        // Coordonnées ramenées dans la matrice
        int x2 = x%m_w;
        int y2 = y%m_h;

        // Coordonnées de la sous-matrice
        int sx = x2/dimSM;
        int sy = y2/dimSM;
        SM& sm = m_tab[sx*m_smY+sy];
        if (sm.estVide())
            return false;
        else
        {
            // Si la collision n'a pas été trouvée, c'est que la particule ET la SM sont dans cette matrice
            if (!colSMTrouvee)
            {
                int px = p.getXInt() / dimSM;
                int py = p.getYInt() / dimSM;
                collisionSM(px,py,sx,sy,colSMTrouvee,liaisonSM);
            }

            // Collision au niveau des sous-matrices
            if (coucheCollision+1 == getProfondeur())
            {
                // Si collision au niveau des SM
                if (!liaisonSM)
                {
                    int xSM = (x/dimSM)*dimSM;
                    int ySM = (y/dimSM)*dimSM;
                    p.collision(sm,xSM,ySM,dimSM);
                    return true;
                }
                else // Sinon, collision au niveau des particules
                    return sm.gererCollision(p, x, y, 0, true, true);
            }
            else // Si on n'est pas au bon niveau de couches, on continue de descendre dans la hiérarchie des sous-matrices
                return sm.gererCollision(p, x, y, coucheCollision, colSMTrouvee, liaisonSM);
        }
    }

    // Pour l'instant, les matrices sont toujours adjacentes
    // TODO Si les matrices ne sont pas adjacentes, faire un parcours de graphe... fuck it
    void collisionSM(int sx1, int sy1, int sx2, int sy2, bool& colSMTrouvee, bool& liaisonSM)
    {
        // SM1(sx1, sy1) et SM2(sx2, sy2) sont supposés dans cette matrice
        if (sx1 == sx2)
        {
            if (sy1 == sy2) // Même sous-matrice
                return;
            else if (sy1 < sy2) // SM2 en bas de SM1
            {
                colSMTrouvee = true;
                liaisonSM = m_tab[sx1*m_smY+sy1].getNbLBas() > 0;
            }
            else // SM2 en haut de SM1
            {
                colSMTrouvee = true;
                liaisonSM = m_tab[sx2*m_smY+sy2].getNbLBas() > 0;
            }
        }
        else if (sx1 < sx2)
        {
            if (sy1 == sy2) // A droite
            {
                colSMTrouvee = true;
                liaisonSM = m_tab[sx1*m_smY+sy1].getNbLDroite() > 0;
            }
            else if (sy1 < sy2) // En bas à droite
            {
                colSMTrouvee = true;
                liaisonSM = ( m_tab[sx1*m_smY+sy1].getNbLBas() > 0 && m_tab[sx1*m_smY+(sy1+1)].getNbLDroite() )
                         || ( m_tab[sx1*m_smY+sy1].getNbLDroite() > 0 && m_tab[(sx1+1)*m_smY+sy1].getNbLBas() );
            }
            else // En haut à droite
            {
                colSMTrouvee = true;
                liaisonSM = ( m_tab[sx1*m_smY+(sy1-1)].getNbLBas() > 0 && m_tab[sx1*m_smY+(sy1-1)].getNbLDroite() )
                         || ( m_tab[sx1*m_smY+sy1].getNbLDroite() > 0 && m_tab[(sx1+1)*m_smY+(sy1-1)].getNbLBas() );
            }
        }
        else
        {
            if (sy1 == sy2) // A gauche
            {
                colSMTrouvee = true;
                liaisonSM = m_tab[(sx1-1)*m_smY+sy1].getNbLDroite() > 0;
            }
            else if (sy1 < sy2) // En bas à gauche
            {
                colSMTrouvee = true;
                liaisonSM = ( m_tab[sx1*m_smY+sy1].getNbLBas() > 0 && m_tab[(sx1-1)*m_smY+(sy1+1)].getNbLDroite() )
                         || ( m_tab[(sx1-1)*m_smY+sy1].getNbLDroite() > 0 && m_tab[(sx1-1)*m_smY+sy1].getNbLBas() );
            }
            else // En haut à gauche
            {
                colSMTrouvee = true;
                liaisonSM = ( m_tab[sx2*m_smY+sy2].getNbLBas() > 0 && m_tab[sx2*m_smY+(sy2-1)].getNbLDroite() )
                         || ( m_tab[sx2*m_smY+sy2].getNbLDroite() > 0 && m_tab[(sx2-1)*m_smY+sy2].getNbLBas() );
            }
        }
    }

    // Changement de vitesse global
    virtual void appliquerDV(Vecteur dv)
    {
        Conteneur::appliquerDV(dv);

        if (m_tab != NULL)
        {
            for(int i = 0 ; i < m_smX*m_smY ; i++)
                m_tab[i].appliquerDV(dv);
        }
    }

    int getNbLDroite() const { return m_nbLDroite; }
    int getNbLBas() const { return m_nbLBas; }

    void lierDroite(int nb = 1) { m_nbLDroite += nb; }
    void lierBas(int nb = 1) { m_nbLBas += nb; }

    // Effectue récursivement les liaisons entre les coordonnées x et y
    // TODO Peut être optimisé avec des suppositions !
    void ajouterLiaison(int nb, int x1, int y1, int x2, int y2)
    {
        if (m_tab == NULL)
            return;

        // Indices des sous-matrices concernées
        int xSM1 = x1/dimSM;
        int ySM1 = y1/dimSM;
        int xSM2 = x2/dimSM;
        int ySM2 = y2/dimSM;

        // Indices x/y min/max
        int xMin, yMin, xMax, yMax;
        if (xSM1 <= xSM2) { xMin = xSM1; xMax = xSM2; }
        else { xMin = xSM2; xMax = xSM1; }

        if (ySM1 <= ySM2) { yMin = ySM1; yMax = ySM2; }
        else { yMin = ySM2; yMax = ySM1; }

        // Normalement, ces conditons suffisent
        xMin = (xMin <= 0) ? 0 : xMin;
        xMax = (xMax >= m_smX) ? m_smX : xMax;
        yMin = (yMin <= 0) ? 0 : yMin;
        yMax = (yMax >= m_smY) ? m_smY : yMax;

        // Ajout des liaisons récursivement
        for(int i = xMin ; i <= xMax ; i++)
        {
            for(int j = yMin ; j <= yMax ; j++)
            {
                if (i != xMax)
                    m_tab[i*m_smY+j].lierDroite(nb);
                if (j != yMax)
                    m_tab[i*m_smY+j].lierBas(nb);
                // Appel récursif
                if (i < m_smX && j < m_smY)
                    m_tab[i*m_smY+j].ajouterLiaison(nb,x1-dimSM*i, y1-dimSM*j,x2-dimSM*i,y2-dimSM*j);
            }
        }
    }

    // Retourne true si la matrice se remplit
    bool set(int x, int y, Particule* p)
    {
        if (x < 0 || x >= m_w || y < 0 || y >= m_h)
            return false;

        // Création du tableaux des sous-matrices, s'il n'existait pas encore
        if (m_tab == NULL)
        {
            m_tab = new SM[m_smX*m_smY];
            for(int i = 0 ; i < m_smX*m_smY ; i++)
                m_tab[i].setDim(dimSM);
        }

        // Coordonnées de la sous-matrice concernée
        int sx = x/dimSM;
        int sy = y/dimSM;
        int indSM = sx*m_smY + sy;
        SM& sm = m_tab[indSM];

        // Insertion du nouvel élément dans la sous-matrice
        if (sm.set(x%dimSM, y%dimSM, p))
        {
            ajouterParticule(p);
            return true;
        }
        else
            return false;
    }

    // Retourne la particule supprimée
    Particule* suppr(int x, int y)
    {
        if (m_tab == NULL || x < 0 || x >= m_w || y < 0 || y >= m_h)
            return NULL;

        // Coordonnées de la sous-matrice concernée
        int sx = x/dimSM;
        int sy = y/dimSM;
        int indSM = sx*m_smY + sy;

        SM& sm = m_tab[indSM];

        Particule* p = sm.suppr(x%dimSM,y%dimSM);

        // On vide le niveau inférieur
        if (p != NULL)
            // Si cela vide la sous-matrice, on la supprime
            supprimerParticule(p);

        return p;
    }

    // Supprime tous les sous-éléments, après les avoir vidés
    virtual void reinit()
    {
        if (m_tab != NULL)
        {
            for(int i = 0 ; i < m_smX*m_smY ; i++)
                m_tab[i].reinit();

            delete[] m_tab;
            m_tab = NULL;
            Conteneur::reinit();
        }
    }

    // Retourne la particule contenue aux coordonnées (x,y)
    Particule* get(int x, int y)
    {
        if (m_tab == NULL)
            return NULL;

        // Coordonnées de la sous-matrice concernée
        int sx = x/dimSM;
        int sy = y/dimSM;
        int indSM = sx*m_smY + sy;

        SM& sm = m_tab[indSM];
        return sm.get(x%dimSM,y%dimSM);
    }

    // Calcule les couleurs des pixels, et les affiche sur le rendu SDL
    virtual void afficher(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule, int x, int y)
    {
        if (coucheAffichage < getProfondeur())
        {
            if (m_tab != NULL)
            {
                for(int i = 0 ; i < m_smX ; i++)
                {
                    for(int j = 0 ; j < m_smY ; j++)
                        m_tab[i*m_smY+j].afficher(rendu, coucheAffichage, tailleParticule, x+dimSM*i, y+dimSM*j);
                }
            }
        }
        else if (coucheAffichage == getProfondeur() && !estVide())
        {
            int taillePixelX = (int)(tailleParticule*(double)m_w);
            int taillePixelY = (int)(tailleParticule*(double)m_h);
            int coordX = (int)(tailleParticule*(double)x);
            int coordY = (int)(tailleParticule*(double)y);
            SDL_SetRenderDrawColor(rendu, m_couleur.r, m_couleur.g, m_couleur.b, m_couleur.a);
            SDL_Rect rect = {coordX, coordY, taillePixelX, taillePixelY};
            SDL_RenderFillRect(rendu, &rect);
        }
    }

    virtual void afficher(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule)
    {
        afficher(rendu, coucheAffichage, tailleParticule, 0, 0);
    }

    virtual void actualiser(double dt)
    {
        if (m_tab != NULL)
        {
            for(int i = 0 ; i < m_smX*m_smY ; i++)
                m_tab[i].actualiser(dt);
        }
    }

    // DEBUG
    void afficherLiaisonsSM() const
    {
        if (m_tab == NULL)
        {
            std::cout << "Matrice vide..." << std::endl;
            return;
        }
        std::cout << "Liaisons des sous-matrices : " << std::endl;
        for(int j = 0 ; j < m_smY ; j++)
        {
            for(int i = 0 ; i < m_smX ; i++)
                std::cout << "." << m_tab[i*m_smY+j].getNbLDroite();
            std::cout << std::endl;
            for(int i = 0 ; i < m_smX ; i++)
                std::cout << m_tab[i*m_smY+j].getNbLBas() << " ";
            std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
    }

    // DEBUG
    SM& getSM(int sx, int sy) const { return m_tab[sx*m_smY+sy]; }

protected:
    SM* m_tab; // Tableaux des sous-éléments

    size_t m_w, m_h; // Dimensions
    int m_smX, m_smY; // Nombre de sous-matrices selon X et Y
    int m_nbLDroite, m_nbLBas; // Nombre de liaisons avec les autres sous-matrices
};

// Spécialisation : Cas de base = matrice simple de particules, n'allouant pas forcément son tableau
template<>
class MatriceCreuse<> : public Conteneur
{
public:
    MatriceCreuse(size_t w, size_t h)
     : Conteneur(), m_tab(NULL),
       m_w(w), m_h(h),
       m_nbLDroite(0), m_nbLBas(0)
    {}

    // Constructeur par défaut (utlisé uniquement par le conteneur !!!)
    MatriceCreuse()
     : Conteneur(), m_tab(NULL),
       m_nbLDroite(0), m_nbLBas(0)
    {}

    // Utilisé après pour définir les dimensions
    void setDim(size_t dim)
    {
        m_w = dim;
        m_h = dim;
    }

    ~MatriceCreuse()
    {
        if (m_tab != NULL)
            delete [] m_tab;
    }

    static inline int getProfondeur() { return 1; }

    virtual void actualiserBarycentre()
    {
        if (m_tab==NULL)
        {
            m_masse = 0.0;
            m_nbP = 0;
        }
        else
        {
            // Valeurs physiques
            m_masse = 0.0;
            m_pos = 0.0;
            m_v = 0.0;

            // Variables pour la couleur
            m_nbP = 0;
            double r = 0.0, g = 0.0, b = 0.0, a = 0.0;

            for(int i = 0 ; i < m_w*m_h ; i++)
            {
                if (m_tab[i] != NULL)
                {
                    m_masse += m_tab[i]->getMasse();
                    m_pos += m_tab[i]->getPos();
                    m_v += m_tab[i]->getV();

                    m_nbP++;
                    SDL_Color c = m_tab[i]->getCouleur();
                    r += (double)c.r;
                    g += (double)c.g;
                    b += (double)c.b;
                    a += (double)c.a;
                }
            }
            m_couleur = {
                    (Uint8)(r/(double)m_nbP),
                    (Uint8)(g/(double)m_nbP),
                    (Uint8)(b/(double)m_nbP),
                    (Uint8)(a/(double)m_nbP) };
        }
    }

    void actualiserAlloc()
    {
        if (m_tab == NULL)
            return;

        if (m_nbP == 0)
        {
            delete [] m_tab;
            m_tab = NULL;
        }
    }

    // Se charge de détecter et d'appliquer les collisions lorsque la particule p essaie de se déplacer aux coordonnées (x,y)
    // (x,y) sont des coordonnées absolues
    // Retourne true s'il y a eu collision
    bool gererCollision(Particule& p, int x, int y, int coucheCollision, bool, bool)
    {
        if (m_tab == NULL)
            return false;

        // Coordonnées ramenées dans la matrice
        int x2 = x%m_w;
        int y2 = y%m_h;

        // Coordonnées de la particule
        Particule* p2 = m_tab[x2*m_h+y2];
        if (p2 != NULL && p2 != &p)
        {
            p.collision(*p2);
            return true;
        }
        else
            return false;
    }

    // Changement de vitesse global
    virtual void appliquerDV(Vecteur dv)
    {
        Conteneur::appliquerDV(dv);

        if (m_tab != NULL)
        {
            for(int i = 0 ; i < m_w*m_h ; i++)
            {
                if (m_tab[i] != NULL)
                    m_tab[i]->appliquerDV(dv);
            }
        }
    }

    int getNbLDroite() { return m_nbLDroite; }
    int getNbLBas() { return m_nbLBas; }

    void lierDroite(int nb = 1) { m_nbLDroite += nb; }
    void lierBas(int nb = 1) { m_nbLBas += nb; }

    void ajouterLiaison(bool ajouter, int x1, int y1, int x2, int y2) {}

    // Retourne true si la matrice se remplit
    bool set(int x, int y, Particule* p)
    {
        if (x < 0 || x >= m_w || y < 0 || y >= m_h)
            return false;

        // Création du tableaux des sous-matrices, s'il n'existait pas encore
        if (m_tab == NULL)
        {
            m_tab = new Particule*[m_w*m_h];
            for(int i = 0 ; i < m_w*m_h ; i++)
                m_tab[i]=NULL;
        }

        // Coordonnées de la particule concernée
        int indP = x*m_h + y;
        Particule*& p2 = m_tab[indP];

        // Insertion du nouvel élément dans la sous-matrice
        if (p2 == NULL)
        {
            p2 = p;
            ajouterParticule(p);
            return true;
        }
        else
            return false;
    }

    // Retourne la particule supprimée
    Particule* suppr(int x, int y)
    {
        if (m_tab == NULL || x < 0 || x >= m_w || y < 0 || y >= m_h)
            return NULL;

        // Coordonnées de la particule concernée
        int indSM = x*m_h + y;

        Particule*& p2 = m_tab[indSM];
        Particule* res = p2;

        // On supprime la particule de la matrice
        p2 = NULL;

        return res;
    }

    virtual void reinit()
    {
        if (m_tab != NULL)
        {
            for(int i = 0 ; i < m_w*m_h ; i++)
                m_tab[i] = NULL;

            delete[] m_tab;
            m_tab = NULL;
            Conteneur::reinit();
        }
    }

    Particule* get(int x, int y)
    {
        if (m_tab == NULL)
            return NULL;

        return (m_tab[x*m_h + y]);
    }

    virtual void afficher(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule)
    {
        afficher(rendu, coucheAffichage, tailleParticule, 0, 0);
    }

    virtual void afficher(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule, int x, int y)
    {
        if (coucheAffichage == 0)
        {
            if (m_tab != NULL)
            {
                for(int i = 0 ; i < m_w ; i++)
                {
                    for(int j = 0 ; j < m_h ; j++)
                    {
                        Particule* p = m_tab[i*m_h+j];
                        if (p != NULL)
                            p->afficher(rendu, coucheAffichage, tailleParticule);
                    }
                }
            }
        }
        else if (coucheAffichage == 1 && !estVide())
        {
            int taillePixelX = (int)(tailleParticule*(double)m_w);
            int taillePixelY = (int)(tailleParticule*(double)m_h);
            int coordX = (int)(tailleParticule*(double)x);
            int coordY = (int)(tailleParticule*(double)y);
            SDL_SetRenderDrawColor(rendu, m_couleur.r, m_couleur.g, m_couleur.b, m_couleur.a);
            SDL_Rect rect = {coordX, coordY, taillePixelX, taillePixelY};
            SDL_RenderFillRect(rendu, &rect);
        }
    }

    // Inutile... mais sait-on jamais
    virtual void actualiser(double dt)
    {
        if (m_tab != NULL)
        {
            for(int i = 0 ; i < m_w*m_h ; i++)
            {
                if (m_tab[i] != NULL)
                    m_tab[i]->actualiser(dt);
            }
        }
    }

protected:
    Particule** m_tab; // Tableaux des particules

    size_t m_w, m_h; // Dimensions
    int m_nbLDroite, m_nbLBas; // Liaisons avec les autres sous-matrices
};

template<>
class MatriceCreuse<1> : public MatriceCreuse<>
{};

// DEBUG : sert à avoir un apperçu du conteneur de particules
template<size_t ...dims>
void afficher(MatriceCreuse<dims...>& m, int w, int h)
{
    std::cout << "Nombre de particules : " << m.getNbP() << std::endl;
    std::cout << "Profondeur : " << m.getProfondeur() << std::endl;
    for(int j = 0 ; j < h ; j++)
    {
        for(int i = 0 ; i < w ; i++)
            std::cout << (m.get(i,j)!=NULL) << " ";
        std::cout << std::endl;
    }
}


#endif //PROTOTYPE_1_MATRICECREUSE_H
