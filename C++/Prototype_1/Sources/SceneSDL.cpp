#include "../Header/SceneSDL.h"

SceneSDL::SceneSDL(Element& e)
 : m_element(e),
   m_clavier(def::NB_TOUCHES, false), m_plusTraite(false), m_moinsTraite(false)
{
    // Chargement de la SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw Erreur(4, "Echec du chargement de la SDL");

    m_config = 0;
}

SceneSDL::~SceneSDL()
{
    SDL_DestroyRenderer(m_rendu);
    SDL_DestroyWindow(m_fenetre);
    SDL_Quit();
}

void SceneSDL::init(int config)
{
    charger(config);

    if (m_config != config)
    {
        // Chargement de la fenetre
        double marge = (def::grilleAffichee) ? 1.0 : 0.0;
        m_fenetre = SDL_CreateWindow(m_titre.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                     (int)(def::tailleParticule*(double)def::width+marge), (int)(def::tailleParticule*(double)def::height+marge), 0);
        if (m_fenetre == NULL)
            throw Erreur(3, "Echec du chargement de la fenetre");

        // Chargement du rendu
        m_rendu = SDL_CreateRenderer(m_fenetre, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (m_rendu == NULL)
            throw Erreur(3, "Echec du chargement du rendu");

        if (SDL_SetRenderDrawBlendMode(m_rendu, SDL_BLENDMODE_BLEND))
            throw Erreur(3, "Echec du chargement du mode de blending");

        m_config = config;
    }
}

void SceneSDL::reinit(int config)
{
    if (m_config != config)
    {
        // Vide la scene
        SDL_DestroyRenderer(m_rendu);
        SDL_DestroyWindow(m_fenetre);
    }

    // La reinitialise
    init(config);
}

void SceneSDL::bouclePrincipale()
{
    // Première initialisation de la scène
    init(-1);

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
            m_element.actualiser(dt);

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
                case SDLK_SPACE:
                    m_clavier[def::K_ESPACE] = true;
                    reinit(m_config);
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

                case SDLK_m:
                    def::liaisonsSMAffichees = !def::liaisonsSMAffichees;
                    m_clavier[def::K_M] = true;
                    break;
                case SDLK_l:
                    def::liaisonsAffichees = !def::liaisonsAffichees;
                    m_clavier[def::K_L] = true;
                    break;
                case SDLK_g:
                    def::grilleAffichee = !def::grilleAffichee;
                    m_clavier[def::K_G] = true;
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

            case SDLK_m:
                m_clavier[def::K_M] = false;
                break;
            case SDLK_l:
                m_clavier[def::K_L] = false;
                break;
            case SDLK_g:
                m_clavier[def::K_G] = false;
                break;

            case SDLK_RSHIFT:
            case SDLK_LSHIFT:
                m_clavier[def::K_SHIFT] = false;
                break;
        }
        break;
    }

    actionClavier(continuer, recommencer);
    m_clavier[def::K_ESPACE] = false; // Pour éviter de faire bugger la détection de ESPACE
}

// Actions par défaut
void SceneSDL::actionClavier(bool& continuer, bool& recommencer)
{
    if (m_event.type == SDL_KEYDOWN)
    {
        switch(m_event.key.keysym.sym)
        {
            case SDLK_ESCAPE:
                continuer = false;
                recommencer = true;
                break;

            case SDLK_KP_PLUS:
                def::echellePlus();
                break;
            case SDLK_KP_MINUS:
                def::echelleMoins();
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
            case SDLK_KP_4:
                reinit(4);
                recommencer = true;
                break;
            case SDLK_KP_5:
                reinit(5);
                recommencer = true;
                break;
        }
    }
}

void SceneSDL::affichage(bool& continuer)
{
    // Nettoyage du rendu
    SDL_SetRenderDrawColor(m_rendu, 255, 255, 255, 255);
    SDL_RenderClear(m_rendu);

    // Construction du rendu
    m_element.afficher(m_rendu, def::coucheAffichage, def::tailleParticule);

    // Affichage de la grille
    if (def::grilleAffichee)
        afficherGrille();

    //if (def::liaisonsAffichees)
        m_element.afficherLiaisons(m_rendu, def::coucheAffichage, def::tailleParticule);

    // Actualisation du rendu
    SDL_RenderPresent(m_rendu);
}

void SceneSDL::afficherGrille()
{
    // Lignes verticales
    SDL_SetRenderDrawColor(m_rendu, 0, 0, 0, 200);
    for(int i = 0 ; i <= def::width ; i += def::pasGrille)
        SDL_RenderDrawLine(m_rendu, i*def::tailleParticule, 0, i*def::tailleParticule, def::height*def::tailleParticule);
    SDL_SetRenderDrawColor(m_rendu, 0, 0, 0, 128);
    if (def::divisionsAffichees)
    {
        for(int i = 0 ; i <= def::width ; i += def::divisionGrille)
        {
            SDL_RenderDrawLine(m_rendu, i*def::tailleParticule+1, 0, i*def::tailleParticule+1, def::height*def::tailleParticule);
            SDL_RenderDrawLine(m_rendu, i*def::tailleParticule-1, 0, i*def::tailleParticule-1, def::height*def::tailleParticule);
        }
    }

    // Lignes horizontales
    SDL_SetRenderDrawColor(m_rendu, 0, 0, 0, 200);
    for(int j = 0 ; j <= def::height ; j += def::pasGrille)
        SDL_RenderDrawLine(m_rendu, 0, j*def::tailleParticule, def::width*def::tailleParticule, j*def::tailleParticule);
    SDL_SetRenderDrawColor(m_rendu, 0, 0, 0, 128);
    if (def::divisionsAffichees)
    {
        for(int j = 0 ; j <= def::height ; j += def::divisionGrille)
        {
            SDL_RenderDrawLine(m_rendu, 0, j*def::tailleParticule+1, def::width*def::tailleParticule, j*def::tailleParticule+1);
            SDL_RenderDrawLine(m_rendu, 0, j*def::tailleParticule-1, def::width*def::tailleParticule, j*def::tailleParticule-1);
        }
    }
}
