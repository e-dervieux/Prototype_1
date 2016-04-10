#include "../Header/SceneSDL.h"

// Définition des actions par défaut de la scène
void ActionClavierDefaut::operator()(std::vector<bool> &clavier, bool &continuer)
{
    if (clavier[def::K_KP_PLUS] && !m_plusTraite)
    {
        def::echellePlus();
        m_plusTraite = true;
    }
    else if (!clavier[def::K_KP_PLUS])
        m_plusTraite = false;

    if (clavier[def::K_KP_MOINS] && !m_moinsTraite)
    {
        def::echelleMoins();
        m_moinsTraite = true;
    }
    else if (!clavier[def::K_KP_MOINS])
        m_moinsTraite = false;
}
ActionClavierDefaut SceneSDL::acDefaut;

SceneSDL::SceneSDL(MatriceParticules& mat, ActionClavier& actionClavier = acDefaut, int config)
 : m_mat(mat), m_actionClavier(actionClavier), m_clavier(def::NB_TOUCHES, false)
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
    int marge = (def::grilleAffichee) ? 1 : 0;
    m_fenetre = SDL_CreateWindow(m_titre.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 def::taillePixel*def::width+marge, def::taillePixel*def::height+marge, 0);
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
            // Délai entre frames
            SDL_Delay(def::delaiEntreFrames);

            if (!def::pasFixe)
            {
                t2 = clock();
                dt = (double)(t2 - t1)/CLOCKS_PER_SEC;
                t1 = t2;
                if (dt > def::dtMax)
                    dt = def::dtMax;
            }
            else
                dt = def::dtMax;

            // Mouvement
            m_mat.actualiser(dt);

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
            m_clavier[def::K_KP_PLUS] = true;
            break;
        case SDLK_KP_MINUS:
            m_clavier[def::K_KP_MOINS] = true;
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
        case SDLK_KP_PLUS:
            m_clavier[def::K_KP_PLUS] = false;
            break;
        case SDLK_KP_MINUS:
            m_clavier[def::K_KP_MOINS] = false;
            break;

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
    m_clavier[def::K_ESPACE] = false; // Pour éviter de faire bugger la détection de ESPACE
}

void SceneSDL::affichage(bool& continuer)
{
    // Nettoyage du rendu
    SDL_SetRenderDrawColor(m_rendu, 255, 255, 255, 255);
    SDL_RenderClear(m_rendu);

    // Construction du rendu
    m_mat.afficher(m_rendu, def::partPP, def::taillePixel);

    // Affichage de la grille
    if (def::grilleAffichee)
        afficherGrille();

    if (def::liaisonsAffichees)
        m_mat.afficherLiaisons(m_rendu, def::partPP, def::taillePixel);

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
