#ifndef SCENESDL_H_INCLUDED
#define SCENESDL_H_INCLUDED

#include <vector>
#include <string>
#include <ctime>
#include <SDL2/SDL.h>

#include "Definitions.h"
#include "Erreur.h"

class SceneSDL
{
public:
    SceneSDL(Element& e);
    virtual ~SceneSDL();

    virtual void charger(int config) = 0; // Charge la configuration config
    virtual void init(int config); // Initialise la scène après avoir chargé la configuration
    virtual void reinit(int config); // Réinitialise la scène : reset, puis init()
    void bouclePrincipale(); // BOUCLE A APPELER POUR FAIRE TOURNER LA SCENE
    void affichage(bool& continuer); // Fonction d'affichage
    void afficherGrille(); // Affichage de la grille de debug
    void gererEvent(bool& continuer, bool& recommencer); // Gère les évènements SDL : actions de base, actualisation du clavier
    virtual void actionClavier(bool& continuer, bool& recommencer); // Action à faire à partir du clavier

protected:
    std::string m_titre;
    Element& m_element;
    int m_config;

    SDL_Window* m_fenetre;
    SDL_Renderer* m_rendu;
    SDL_Event m_event;

    std::vector<bool> m_clavier; // Un booléen par touche : touche appuyée ou non
    bool m_plusTraite, m_moinsTraite; // Pour gérer le moment d'appui d'une touche
};

#endif // SCENESDL_H_INCLUDED
