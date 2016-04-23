#ifndef PROTOTYPE_1_DEMOLIAISON_H
#define PROTOTYPE_1_DEMOLIAISON_H

#include <sstream>
#include "demoGraphique1.h"
#include "SceneSDL.h"

class SceneDemoLiaison : public SceneSDL
{
public:
    SceneDemoLiaison(Element& e, Particule* part)
     : SceneSDL(e), m_part(part)
    {}

    virtual void charger(int config)
    {
        SDL_Color c;
        double k,cc;

        switch(config)
        {
            case 2:
                c = {255,0,255,255};
                k = 10.0;
                cc = 0.2;
                break;

            case 3:
                c = {0,255,255};
                k = 30.0;
                cc = 1.0;
                break;

            default:
                c = {255,0,0,255};
                k = 3.0;
                cc = 0.1;
        }
        m_matiere = Matiere(c, 1.0, 20.0, k, cc);

        std::stringstream s;
        s << "Liaison elastique : k=" << k << ", c=" << cc;
        m_titre = s.str();

        m_part[0] = Particule(6,6,&m_matiere);
        m_part[1] = Particule(24,24,&m_matiere);
        m_part[0].lier(&m_part[1]);

        m_element.reinit();
    }

private:
    Particule* m_part;
    Matiere m_matiere;
};

void demoLiaison()
{
    def::redefGrille(32,32,10.0,0,0,3,true,true,4,8);
    def::liaisonsAffichees = true;
    def::redefTemp(true,0.05,50);

    // On crée au préalable les particules
    Particule* particules = new Particule[2];
    particules[0] = Particule(-1,1);
    particules[0] = Particule(-1,-1);

    particules[0].lier(&particules[1]);

    // Création de la matrice
    MatriceParticules<8,4,2> m(32,32, 0, particules, 2);

    SceneDemoLiaison scene(m, particules);
    try { scene.bouclePrincipale(); }
    catch(Erreur& e) { std::cout << "Erreur !!" << std::endl << e.what() << std::endl; }

    delete[] particules;
}

#endif //PROTOTYPE_1_DEMOLIAISON_H
