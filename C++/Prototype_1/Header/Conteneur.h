#ifndef PROTOTYPE_1_CONTENEUR_H
#define PROTOTYPE_1_CONTENEUR_H

#include "Particule.h"
#include "LiaisonsMC.h"

namespace mcprive
{
    // Conteneur de particules
    class Conteneur : public Element {
    public:
        Conteneur() ;
        virtual void actualiserBarycentre() = 0;

        inline bool estVide() { return m_nbP == 0; }
        inline int getNbP() const { return m_nbP; }
        inline LiaisonsMC getL() const { return m_liaisons; }
        inline void lier(Direction dir, int nb) { m_liaisons.lier(dir,nb); }
        virtual SDL_Color getCouleur() const;
        // Vide le conteneur et le re-remplit s'il est attaché à d'autres données (cf MatriceParticulesOld)
        virtual void reinit();

        virtual void ajouterParticule(Particule *p); // Rajoute une particule dans les calculs barycentriques
        virtual void supprimerParticule(Particule *p); // Supprime une particule dans les calculs barycentriques

    protected:
        int m_nbP; // Nombre de particules total

        // Pour les calculs de couleur
        int m_rTot, m_gTot, m_bTot, m_aTot;

        mcprive::LiaisonsMC m_liaisons; // Liaisons avec les conteneurs adjacents
    };

}
#endif //PROTOTYPE_1_CONTENEUR_H
