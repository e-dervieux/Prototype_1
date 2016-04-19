#ifndef PROTOTYPE_1_MATRICECREUSE_H
#define PROTOTYPE_1_MATRICECREUSE_H

#include "Conteneur.h"

template<size_t ...dims>
class MatriceCreuse;

// Conteneur défini de manière récursive
template<>
template<size_t dimSM, size_t ...autres>
class MatriceCreuse<dimSM, autres...> : public Conteneur
{
    // On suppose que T est ici forcément un type de MatriceCreuse !
    // Le cas de Particule sera géré plus tard
    using SM = MatriceCreuse<autres...>;

public:
    MatriceCreuse<dimSM, autres...>(size_t w, size_t h)
     : Conteneur(), m_tab(NULL),
       m_w(w), m_h(h), m_smX((int)ceil((double)w/(double)dimSM)), m_smY((int)ceil((double)h/(double)dimSM))
    {}

    // Constructeur par défaut (utlisé uniquement par le conteneur !!!)
    MatriceCreuse<dimSM, autres...>()
     : Conteneur(), m_tab(NULL)
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
    bool gererCollision(Particule& p, int x, int y, int coucheCollision)
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
            // Collision au niveau des sous-matrices
            if (coucheCollision+1 == getProfondeur())
            {
                // Si collision au niveau des SM
                if (p.detecterCollisionSM(x, y, dimSM))
                {
                    int xSM = (x/dimSM)*dimSM;
                    int ySM = (y/dimSM)*dimSM;
                    p.collision(sm,xSM,ySM,dimSM);
                    return true;
                }
                else // Sinon, collision au niveau des particules
                    return sm.gererCollision(p, x, y, 0);
            }
            else // Si on n'est pas au bon niveau de couches, on continue de descendre dans la hiérarchie des sous-matrices
                return sm.gererCollision(p, x, y, coucheCollision);
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
        else if (coucheAffichage == getProfondeur())
        {
            int taillePixelX = (int)(tailleParticule*(double)m_w);
            int taillePixelY = (int)(tailleParticule*(double)m_h);
            SDL_SetRenderDrawColor(rendu, m_couleur.r, m_couleur.g, m_couleur.b, m_couleur.a);
            SDL_Rect rect = {taillePixelX*x, taillePixelY*y,taillePixelX,taillePixelY};
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

protected:
    SM* m_tab; // Tableaux des sous-éléments

    size_t m_w, m_h; // Dimensions
    int m_smX, m_smY; // Nombre de sous-matrices selon X et Y
};

// Spécialisation : Cas de base = matrice simple de particules, n'allouant pas forcément son tableau
template<>
class MatriceCreuse<> : public Conteneur
{
public:
    MatriceCreuse(size_t w, size_t h)
     : Conteneur(), m_tab(NULL),
       m_w(w), m_h(h)
    {}

    // Constructeur par défaut (utlisé uniquement par le conteneur !!!)
    MatriceCreuse()
     : Conteneur(), m_tab(NULL)
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
    bool gererCollision(Particule& p, int x, int y, int coucheCollision)
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
        else if (coucheAffichage == 1)
        {
            SDL_SetRenderDrawColor(rendu, m_couleur.r, m_couleur.g, m_couleur.b, m_couleur.a);
            SDL_Rect rect = {(int)(tailleParticule*(double)x), (int)(tailleParticule*(double)y),(int)tailleParticule,(int)tailleParticule};
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
