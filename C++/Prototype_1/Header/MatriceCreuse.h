#ifndef PROTOTYPE_1_MATRICECREUSE_H
#define PROTOTYPE_1_MATRICECREUSE_H

#include "Conteneur.h"
#include "Definitions.h"

// TODO Pouvoir utiliser le rendu de def pour afficher les collisions, etc...

// TODO Utiliser un test de préprocesseur pour les fonctions de déboggage (#ifdef DEBUG)

namespace mcprive
{

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
       m_w(w), m_h(h), m_smX( (w-1)/dimSM + 1), m_smY( (h-1)/dimSM + 1),
       m_nbLDroite(0), m_nbLBas(0), m_nbLTot(0)
    {}

    // Constructeur par défaut (utlisé uniquement par le conteneur !!!)
    MatriceCreuse<dimSM, autres...>()
     : Conteneur(), m_tab(NULL),
       m_nbLDroite(0), m_nbLBas(0), m_nbLTot(0)
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
    static inline int getDim(int couche) { return (couche==getProfondeur()-1) ? dimSM : SM::getDim(couche); }

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
        }
    }

    void actualiserAlloc()
    {
        if (m_tab == NULL)
            return;

        if (m_nbP == 0 && m_nbLTot == 0)
        {
            delete [] m_tab;
            m_tab = NULL;
            return;
        }

        for(int i = 0 ; i < m_smX*m_smY ; i++)
            m_tab[i].actualiserAlloc();
    }

    void lierSMDroite(int x, int y, int nb, int couche)
    {
        m_nbLTot += nb;

        if (getProfondeur() == couche)
            lierDroite(nb);
        else
        {
            int sx = x/dimSM;
            int sy = y/dimSM;
            creerTab();
            m_tab[sx*m_smY+sy].lierSMDroite(x-sx*dimSM, y-sy*dimSM, nb, couche);
        }
    }

    void lierSMBas(int x, int y, int nb, int couche)
    {
        m_nbLTot += nb;

        if (getProfondeur() == couche)
            lierBas(nb);
        else
        {
            int sx = x/dimSM;
            int sy = y/dimSM;
            creerTab();
            m_tab[sx*m_smY+sy].lierSMBas(x%dimSM, y%dimSM, nb, couche);
        }
    }

    // Nombre de liaisons à droite de la SM concernée par les coordonnées (x,y), à un niveau de couche donné
    int liaisonSMDroite(int x, int y, int couche)
    {
        if (getProfondeur() == couche)
            return m_nbLDroite;
        else
        {
            if (m_tab == NULL)
                return 0;
            else
            {
                int sx = x/dimSM;
                int sy = y/dimSM;
                return m_tab[sx*m_smY+sy].liaisonSMDroite(x%dimSM,y%dimSM, couche);
            }
        }
    }

    // Nombre de liaisons en bas de la SM concernée par les coordonnées (x,y), à un niveau de couche donné
    int liaisonSMBas(int x, int y, int couche)
    {
        if (getProfondeur() == couche)
            return m_nbLBas;
        else
        {
            if (m_tab == NULL)
                return 0;
            else
            {
                int sx = x/dimSM;
                int sy = y/dimSM;
                return m_tab[sx*m_smY+sy].liaisonSMBas(x-sx*dimSM,y-sy*dimSM, couche);
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

    inline int getNbLDroite() const { return m_nbLDroite; }
    inline int getNbLBas() const { return m_nbLBas; }
    inline int getNbLTot() const { return m_nbLTot; }

    inline void lierDroite(int nb = 1) { m_nbLDroite += nb; m_nbLTot += nb; }
    inline void lierBas(int nb = 1) { m_nbLBas += nb; m_nbLTot += nb; }

    // Création du tableaux des sous-matrices, s'il n'existait pas encore
    void creerTab()
    {
        if (m_tab == NULL)
        {
            m_tab = new SM[m_smX*m_smY];
            for(int i = 0 ; i < m_smX*m_smY ; i++)
                m_tab[i].setDim(dimSM);
        }
    }

    // Retourne true si la matrice se remplit
    bool set(int x, int y, Particule* p)
    {
        if (x < 0 || x >= m_w || y < 0 || y >= m_h)
            return false;

        creerTab();

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
            supprimerParticule(p);

        return p;
    }

    // Supprime tous les sous-éléments, après les avoir vidés
    virtual void reinit()
    {
        if (m_tab != NULL)
        {
            delete[] m_tab;
            m_tab = NULL;
            Conteneur::reinit();
        }
        m_nbLBas=0;
        m_nbLDroite=0;
        m_nbLTot=0;
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

    // Retourne la SM contenant les coordonnées (x,y), à un certain niveau de couche
    Conteneur* getSM(int x, int y, int couche)
    {
        if (couche == getProfondeur())
            return this;
        else if (m_tab == NULL)
            return NULL;
        else
        {
            int sx = x/dimSM;
            int sy = y/dimSM;
            return m_tab[sx*m_smY+sy].getSM(x%dimSM,y%dimSM, couche);
        }
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
            SDL_Color c = getCouleur();
            SDL_SetRenderDrawColor(rendu, c.r, c.g, c.b, c.a);
            SDL_Rect rect = {coordX, coordY, taillePixelX, taillePixelY};
            SDL_RenderFillRect(rendu, &rect);
        }
    }

    virtual inline void afficher(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule)
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
                std::cout << "-" << m_tab[i*m_smY+j].getNbLTot() << "-" << m_tab[i*m_smY+j].getNbLDroite();
            std::cout << std::endl;
            for(int i = 0 ; i < m_smX ; i++)
                std::cout << " " << m_tab[i*m_smY+j].getNbLBas() << "  ";
            std::cout << std::endl;
        }
    }

    // DEBUG : idem, mais on peut choisir le niveau de couche à afficher
    void afficherLiaisonsSM(int dimSSM, int couche)
    {
        if (m_tab == NULL)
        {
            std::cout << "Matrice vide..." << std::endl;
            return;
        }
        std::cout << "Liaisons des sous-matrices : " << std::endl;
        for(int j = 0 ; j < m_h ; j += dimSSM)
        {
            for(int i = 0 ; i < m_w ; i += dimSSM)
            {
                Conteneur* sm = getSM(i,j, couche);
                std::cout << "-" << ( (sm==NULL) ? 0 : sm->getNbLTot() ) << "-" << ( (sm==NULL) ? 0 : sm->getNbLDroite() );
            }
            std::cout << std::endl;
            for(int i = 0 ; i < m_w ; i += dimSSM)
            {
                Conteneur* sm = getSM(i,j, couche);
                std::cout << " " << ( (sm==NULL) ? 0 : sm->getNbLBas() ) << "  ";
            }
            std::cout << std::endl;
        }
    }

    // DEBUG : sert à avoir un apperçu du conteneur de particules
    void afficher(std::ostream& flux = std::cout)
    {
        flux << "Nombre de particules : " << m_nbP << std::endl;
        flux << "Profondeur : " << getProfondeur() << std::endl;
        for(int j = 0 ; j < m_h ; j++)
        {
            for(int i = 0 ; i < m_w ; i++)
                flux << (get(i,j) != NULL) << " ";
            flux << std::endl;
        }
    }

    // DEBUG : sert à avoir un apperçu du conteneur de particules
    void afficherP(std::ostream& flux = std::cout)
    {
        flux << "Nombre de particules au total : " << m_nbP << std::endl;

        if (m_tab == NULL)
            return;

        for(int j = 0 ; j < m_smY ; j++)
        {
            for(int i = 0 ; i < m_smX ; i++)
                flux << m_tab[i*m_smY+j].getNbP() << " ";
            flux << std::endl;
        }
    }

    // DEBUG : sous-matrices directes, aux coordonnées (sx,sy) dans m_tab
    SM& getSM(int sx, int sy) const { return m_tab[sx*m_smY+sy]; }

protected:
    SM* m_tab; // Tableaux des sous-éléments

    size_t m_w, m_h; // Dimensions
    int m_smX, m_smY; // Nombre de sous-matrices selon X et Y
    int m_nbLDroite, m_nbLBas; // Nombre de liaisons avec les autres sous-matrices
    int m_nbLTot; // Nombre de liaisons internes total
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
    static inline int getDim(int couche) { return 1; }

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
                }
            }
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

    inline int getNbLDroite() const { return m_nbLDroite; }
    inline int getNbLBas() const { return m_nbLBas; }
    inline int getNbLTot() const { return m_nbLBas+m_nbLDroite; }

    inline void lierDroite(int nb = 1) { m_nbLDroite += nb; }
    inline void lierBas(int nb = 1) { m_nbLBas += nb; }

    inline void lierSMDroite(int x, int y, int nb, int couche) { lierDroite(nb); }
    inline void lierSMBas(int x, int y, int nb, int couche) { lierBas(nb); }

    // Nombre de liaisons à droite de la SM concernée par les coordonnées (x,y), à un niveau de couche donné
    inline int liaisonSMDroite(int x, int y, int couche) { return m_nbLDroite; }

    // Nombre de liaisons en bas de la SM concernée par les coordonnées (x,y), à un niveau de couche donné
    inline int liaisonSMBas(int x, int y, int couche) { return m_nbLBas; }

    inline void ajouterLiaison(int x1, int y1, int x2, int y2, int nb, int couche) {}

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

        if (p2 != NULL)
            supprimerParticule(p2);

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
        m_nbLBas=0;
        m_nbLDroite=0;
    }

    // Retourne la particule aux coordonnées (x,y)
    Particule* get(int x, int y)
    {
        if (m_tab == NULL)
            return NULL;

        return (m_tab[x*m_h + y]);
    }

    inline Conteneur* getSM(int x, int y, int couche) { return this; }

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
            SDL_Color c = getCouleur();
            SDL_SetRenderDrawColor(rendu, c.r, c.g, c.b, c.a);
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

    // DEBUG : sert à avoir un apperçu du conteneur de particules
    void afficher(std::ostream& flux = std::cout)
    {
        flux << "Nombre de particules : " << m_nbP << std::endl;
        flux << "Profondeur : " << getProfondeur() << std::endl;
        for(int j = 0 ; j < m_h ; j++)
        {
            for(int i = 0 ; i < m_w ; i++)
                flux << (get(i,j) != NULL) << " ";
            flux << std::endl;
        }
    }

    void afficherP(std::ostream& flux = std::cout)
    {
        flux << "Nombre de particules au total : " << m_nbP << std::endl;

        if (m_tab == NULL)
            return;

        for(int j = 0 ; j < m_h ; j++)
        {
            for(int i = 0 ; i < m_w ; i++)
                flux << (get(i,j) != NULL) << " ";
            flux << std::endl;
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

} // namespace mc
#endif //PROTOTYPE_1_MATRICECREUSE_H
