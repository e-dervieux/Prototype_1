#ifndef SCENESDL_H_INCLUDED
#define SCENESDL_H_INCLUDED

#include <vector>
#include <string>
#include <ctime>
#include <SDL2/SDL.h>

#include "Definitions.h"
#include "Erreur.h"
#include "Grille.h"

class SceneSDL
{
public:
    SceneSDL(Grille& grille, int config = 1);
    ~SceneSDL();

    void init(int config);
    void reinit(int config);
    void bouclePrincipale();
    void affichage(bool& continuer);
    void afficherGrille();
    void gererEvent(bool& continuer);

private:
    std::string m_titre;
    Grille& m_grille;
    int m_config;

    SDL_Window* m_fenetre;
    SDL_Renderer* m_rendu;
    SDL_Event m_event;

    std::vector<bool> m_clavier;

    void echellePlus();
    void echelleMoins();
};

#endif // SCENESDL_H_INCLUDED
