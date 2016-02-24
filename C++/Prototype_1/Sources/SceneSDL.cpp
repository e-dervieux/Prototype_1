#include "../Header/SceneSDL.h"

SceneSDL::SceneSDL(Grille& grille, ActionClavier& actionClavier, int config)
 : m_grille(grille), m_actionClavier(actionClavier), m_clavier(def::NB_TOUCHES, false)
{
    // Chargement de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw Erreur(4, "Echec du chargement de la SDL");

    m_config = -1;
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
    switch(config)
    {
    default:
        m_titre = "Test de cohésion";

        if (m_config != config)
        {
            def::pasGrille = 16;
            def::partPP = 1;
        }
        def::divisionGrille = 5;
        break;
    }

    m_config = config;

    // Chargement de la fenetre
    m_fenetre = SDL_CreateWindow(m_titre.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 def::taillePixel*def::width+1, def::taillePixel*def::height+1, 0);
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
    SDL_DestroyRenderer(m_rendu);
    SDL_DestroyWindow(m_fenetre);

    // La reinitialise
    init(config);
}

void SceneSDL::bouclePrincipale()
{
    bool continuer = true, recommencer = false;
    time_t t1, t2;
    double dt;

    t1 = clock();
    while (continuer)
    {
        SDL_PollEvent(&m_event);
        gererEvent(continuer, recommencer);

        if (!recommencer)
        {
            t2 = clock();
            dt = (double)(t2 - t1)/CLOCKS_PER_SEC;
            t1 = t2;
            if (dt > def::dtMax)
                dt = def::dtMax;

            // Mouvement
            m_grille.actualiser(0.017); // DEBUG (mettre dt sinon)

            // Actualisation du rendu
            affichage(continuer);
        }
        else
            recommencer = false;
    }
}


void SceneSDL::gererEvent(bool & continuer, bool& recommencer)
{
    switch(m_event.type)
    {
    case SDL_QUIT:
        continuer = false;
        recommencer = false;
        break;

    // Attribution des touches du clavier dans le tableau clavier (pour le multi-touches)
    case SDL_KEYDOWN:
        switch(m_event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            continuer = false;
            recommencer = true;
            break;

        case SDLK_SPACE:
            m_clavier[def::K_ESPACE] = true;
            reinit(m_config);
            recommencer = true;
            break;

        case SDLK_KP_1:
            reinit(1);
            recommencer = true;
            break;

        case SDLK_KP_2:
            reinit(2);
            recommencer = true;
            break;

        case SDLK_KP_3:
            reinit(3);
            recommencer = true;
            break;

        case SDLK_KP_PLUS:
            echellePlus();
            break;
        case SDLK_KP_MINUS:
            echelleMoins();
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

    m_actionClavier(m_clavier, continuer);
    m_clavier[def::K_ESPACE] = false;
}

void SceneSDL::affichage(bool& continuer)
{
    // Nettoyage du rendu
    SDL_SetRenderDrawColor(m_rendu, 255, 255, 255, 255);
    SDL_RenderClear(m_rendu);

    // Construction du rendu
    m_grille.afficher(m_rendu, def::partPP, def::taillePixel);

    // Affichage de la grille
    if (def::grilleAffichee)
        afficherGrille();

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
    if (def::divisionsAffichees)
    {
        for(int i = 0 ; i <= def::width ; i += def::divisionGrille*def::pasGrille)
        {
            SDL_RenderDrawLine(m_rendu, i*def::taillePixel+1, 0, i*def::taillePixel+1, def::height*def::taillePixel);
            SDL_RenderDrawLine(m_rendu, i*def::taillePixel-1, 0, i*def::taillePixel-1, def::height*def::taillePixel);
        }
    }

    // Lignes horizontales
    SDL_SetRenderDrawColor(m_rendu, 0, 0, 0, 200);
    for(int j = 0 ; j <= def::height ; j += def::pasGrille)
        SDL_RenderDrawLine(m_rendu, 0, j*def::taillePixel, def::width*def::taillePixel, j*def::taillePixel);
    SDL_SetRenderDrawColor(m_rendu, 0, 0, 0, 128);
    if (def::divisionsAffichees)
    {
        for(int j = 0 ; j <= def::height ; j += def::divisionGrille*def::pasGrille)
        {
            SDL_RenderDrawLine(m_rendu, 0, j*def::taillePixel+1, def::width*def::taillePixel, j*def::taillePixel+1);
            SDL_RenderDrawLine(m_rendu, 0, j*def::taillePixel-1, def::width*def::taillePixel, j*def::taillePixel-1);
        }
    }
}

void SceneSDL::echellePlus()
{
    if (def::taillePixel > 1)
    {
        def::partPP *= 2;
        def::pasGrille /= 2;
        def::taillePixel *= 2;
        def::width /= 2;
        def::height /= 2;
    }
}

void SceneSDL::echelleMoins()
{
    if (def::partPP > 1)
    {
        def::partPP /= 2;
        def::pasGrille *= 2;
        def::taillePixel /= 2;
        def::width *= 2;
        def::height *= 2;
    }
}