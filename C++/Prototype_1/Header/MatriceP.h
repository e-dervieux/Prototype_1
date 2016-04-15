#ifndef PROTOTYPE_1_MATRICEP_H
#define PROTOTYPE_1_MATRICEP_H

#include "CouchesParticules.h"

// Classe définie à partir de CouchesParticules(MatriceCreuse)
// Les méthodes de gestion des particules sont hybrides : soit à partir du tableau de particules,
// soit en parcours en profondeur
template<size_t ...dims>
class MatriceP : public CouchesParticules<dims...>
{
public:
    MatriceP(size_t w, size_t h, Particule* particules, int nbParticules)
     : CouchesParticules<dims...>(w,h), m_part(particules), m_nbPart(nbParticules)
    {
        ajouterParticules();
    }

    // Ajoute les particules de m_particules dans la matrice
    void ajouterParticules()
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

    // Réinitialise la matrice de particules à partir du tableau
    // afin qu'elle soit gérée de manière correcte.
    virtual void reinit()
    {
        // Suppression du contenu de la matrice Creuse
        CouchesParticules<dims...>::reinit();

        // Rajout des particules dans la matrice
        ajouterParticules();

        // Suppression des forces rémanentes
        for(int i = 0 ; i < m_nbPart ; i++)
        m_part[i].annulerForces();
    }

    // Indique si une particule est bien dans la matrice
    bool estValide(Particule& p)
    {
        int x = p.getXInt();
        int y = p.getYInt();
        return (x >= 0 && x < m_w && y >= 0 && y < m_h);
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

    //  Calcule la prochaine position des particules
    void calculerDeplacement(double dt)
    {
        for(int i = 0 ; i < m_nbPart ; i++)
        {
            Particule& p = m_part[i];
            if (estValide(p))
                p.calculerDeplacement(dt);
        }
    }

    // Effectue le déplacement des particules dans la matrice
    // TODO
    void deplacer(double dt)
    {

    }

    // Calcule la frame suivante, à partir des méthodes ci-dessus
    void actualiser(double dt)
    {
        // Calculer la force à appliquer et l'appliquer à chaque particule
        forcesLiaison();

        // Modifier les coordonnées de ces particules
        calculerDeplacement(dt);

        // Deplacer effectivement ces coordonnées dans la grille
        deplacer(dt);
    }

    void afficherLiaisons(SDL_Renderer* rendu, int partPP, int taillePixel)
    {
        SDL_SetRenderDrawColor(rendu, 0,255,0,60);

        for(int i = 0 ; i < m_nbPart ; i++)
            m_part[i].afficherLiaisons(rendu, partPP, taillePixel);
    }

private:
    Particule* m_part; // Tableau des particules à gérer
    int m_nbPart; // Nombre de particules dans le tableau
};

#endif //PROTOTYPE_1_MATRICEP_H
