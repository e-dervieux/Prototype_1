#include "../Header/SceneSDL.h"

SceneSDL::SceneSDL(Grille& g, int config)
 : m_g(g), m_clavier(def::NB_TOUCHES, false)
{
    // Chargement de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw Erreur(4, "Echec du chargement de la SDL");

    init(config); // Chargement de la configuration par defaut
}

SceneSDL::~SceneSDL()
{
    SDL_DestroyRenderer(m_rendu);
    SDL_DestroyWindow(m_fenetre);
    SDL_Quit();
}

void SceneSDL::init(int config)
{
    m_config = config;

    switch(m_config)
    {
    default:
        m_titre = "Test 1";
        def::redefinir(200, 125, 5, true, 5);
        break;
    }

    // Chargement de la fenetre
    m_fenetre = SDL_CreateWindow(m_titre.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 def::taillePixel*def::width, def::taillePixel*def::height, 0);
    if (m_fenetre == NULL)
        throw Erreur(3, "Echec du chargement de la fenetre");

    // Chargement du rendu
    m_rendu = SDL_CreateRenderer(m_fenetre, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_rendu == NULL)
        throw Erreur(3, "Echec du chargement du rendu");

}

void SceneSDL::reinit(int config)
{
    // Vide la scene
    SDL_DestroyWindow(m_fenetre);
    SDL_DestroyRenderer(m_rendu);

    // La reinitialise
    init(config);
}

void SceneSDL::bouclePrincipale()
{
    bool continuer = true;
    time_t t1, t2;
    double dt;

    t1 = clock();
    while (continuer)
    {
        SDL_PollEvent(&m_event);
        gererEvent(continuer);

        // Calul du dt
        t2 = clock();
        dt = (double)(t2 - t1)/CLOCKS_PER_SEC;
        t1 = t2;
        if (dt > def::dtMax)
            dt = def::dtMax;

        // Mouvement

        // Actualisation du rendu
        affichage(continuer);
    }
}


void SceneSDL::gererEvent(bool & continuer)
{
    switch(m_event.type)
    {
    case SDL_QUIT:
        continuer = false;
        break;

    // Attribution des touches du clavier dans le tableau clavier (pour le multi-touches)
    case SDL_KEYDOWN:
        switch(m_event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            continuer = false;
            break;

        case SDLK_SPACE:
            reinit(m_config);
            break;

        case SDLK_KP_1:
            reinit(1);
            break;

        case SDLK_KP_2:
            reinit(2);
            break;

        case SDLK_KP_3:
            reinit(3);
            break;


        case SDLK_RIGHT:
            m_clavier[def::K_DROITE] = true;
            break;
        case SDLK_LEFT:
            m_clavier[def::K_GAUCHE] = true;
            break;
        case SDLK_UP:
            m_clavier[def::K_HAUT] = true;
            break;
        case SDLK_DOWN:
            m_clavier[def::K_BAS] = true;
            break;

        case SDLK_z:
            m_clavier[def::K_Z] = true;
            break;
        case SDLK_q:
            m_clavier[def::K_Q] = true;
            break;
        case SDLK_s:
            m_clavier[def::K_S] = true;
            break;
        case SDLK_d:
            m_clavier[def::K_D] = true;
            break;

        case SDLK_RSHIFT:
        case SDLK_LSHIFT:
            m_clavier[def::K_SHIFT] = true;
            break;
        }
        break;

    case SDL_KEYUP:
        switch(m_event.key.keysym.sym)
        {
        case SDLK_RIGHT:
            m_clavier[def::K_DROITE] = false;
            break;
        case SDLK_LEFT:
            m_clavier[def::K_GAUCHE] = false;
            break;
        case SDLK_UP:
            m_clavier[def::K_HAUT] = false;
            break;
        case SDLK_DOWN:
            m_clavier[def::K_BAS] = false;
            break;

        case SDLK_z:
            m_clavier[def::K_Z] = false;
            break;
        case SDLK_q:
            m_clavier[def::K_Q] = false;
            break;
        case SDLK_s:
            m_clavier[def::K_S] = false;
            break;
        case SDLK_d:
            m_clavier[def::K_D] = false;
            break;

        case SDLK_RSHIFT:
        case SDLK_LSHIFT:
            m_clavier[def::K_SHIFT] = false;
            break;
        }
        break;
    }
}

void SceneSDL::affichage(bool& continuer)
{
    // Nettoyage du rendu
    SDL_SetRenderDrawColor(m_rendu, 255, 255, 255, 255);
    SDL_RenderClear(m_rendu);

    // Affichage de la grille
    if (def::grilleAffichee)
        afficherGrille();

    // Construction du rendu

    // Actualisation du rendu
    SDL_RenderPresent(m_rendu);
}

void SceneSDL::afficherGrille()
{
    // Lignes verticales
    SDL_SetRenderDrawColor(m_rendu, 0, 0, 0, 200);
    for(int i = 0 ; i <= def::width ; i += def::pasGrille)
        SDL_RenderDrawLine(m_rendu, i*def::taillePixel, 0, i*def::taillePixel, def::height*def::taillePixel);
    SDL_SetRenderDrawColor(m_rendu, 0, 0, 0, 128);
    for(int i = 0 ; i <= def::width ; i += def::divisionGrille*def::pasGrille)
    {
        SDL_RenderDrawLine(m_rendu, i*def::taillePixel+1, 0, i*def::taillePixel+1, def::height*def::taillePixel);
        SDL_RenderDrawLine(m_rendu, i*def::taillePixel-1, 0, i*def::taillePixel-1, def::height*def::taillePixel);
    }

    // Lignes horizontales
    SDL_SetRenderDrawColor(m_rendu, 0, 0, 0, 200);
    for(int j = 0 ; j <= def::height ; j += def::pasGrille)
        SDL_RenderDrawLine(m_rendu, 0, j*def::taillePixel, def::width*def::taillePixel, j*def::taillePixel);
    SDL_SetRenderDrawColor(m_rendu, 0, 0, 0, 128);
    for(int j = 0 ; j <= def::height ; j += def::divisionGrille*def::pasGrille)
    {
        SDL_RenderDrawLine(m_rendu, 0, j*def::taillePixel+1, def::width*def::taillePixel, j*def::taillePixel+1);
        SDL_RenderDrawLine(m_rendu, 0, j*def::taillePixel-1, def::width*def::taillePixel, j*def::taillePixel-1);
    }
}
