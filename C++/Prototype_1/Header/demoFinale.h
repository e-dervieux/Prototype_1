#ifndef PROTOTYPE_1_DEMOFINALE
#define PROTOTYPE_1_DEMOFINALE

#include "SolideImage.h"
#include "SceneSDL.h"
#include "MatriceParticules.h"

#define L0F 2.5
#define L0_TERRAINF 3.0

using MatriceDemoImageF = MatriceParticules<128,32,8,4>;

class SceneDemoImageF : public SceneSDL
{
public:
    SceneDemoImageF(Particule* p, MatriceDemoImageF& mat, std::vector<SolideImage>& images, int nbPart)
            : SceneSDL(mat), m_part(p), m_img(images), m_m(3), m_nbPart(nbPart)
    { }

    virtual void actionClavier(bool& continuer, bool& recommencer)
    {
        SceneSDL::actionClavier(continuer, recommencer);

        if (m_clavier[def::K_SHIFT])
            def::delaiEntreFrames = 150;
        else
            def::delaiEntreFrames = 0;
    }

    virtual void charger(int config)
    {
        for(int i = 0 ; i < m_nbPart ; i++)
            m_part[i] = Particule();

        m_m[1] = Matiere({255,255,255,255}, 1.0, L0F, 200.0, 7.0, 0.5, 12.0);
        m_m[2] = Matiere({255,255,255,255}, 1.0, L0F, 330.0, 8.0, 0.5, 12.0);

        m_img[1].init(Vecteur(80.0,80.0), 1.0, &m_m[1]);

        m_img[2].init(Vecteur(450.0,80.0), 1.0, &m_m[2]);
        m_img[2].appliquerDV(Vecteur(-15.0,0.0));

        m_titre = "Hermine bretonne VS CentraleSupelec";

        m_m[0] = Matiere({0,0,0,255}, 500000000000000000.0);
        m_img[0].init(Vecteur(0.0,0.0),1.0,&m_m[0]);
        m_element.reinit();
    }

private:
    Particule* m_part;
    std::vector<SolideImage>& m_img;
    std::vector<Matiere> m_m;
    int m_nbPart;
};

void demoImageFinale()
{
    def::redefGrille(1024,768,1.0,1,2,4,false,true,32,128);
    def::redefTemp(true, 0.03, 0);
    def::nbIterationsEuler = 1;

    std::vector<SolideImage> images;
    try
    {
        images.push_back(SolideImage(1.0,L0_TERRAINF,"F:/terrain.bmp"));
        images.push_back(SolideImage(0.4,L0F,"F:/cs.bmp"));
        images.push_back(SolideImage(0.7,L0F,"F:/hermine.bmp"));
    }
    catch (const Erreur& e)
    {
        std::cout << "Erreur !!" << std::endl << e.what() << std::endl;
        return;
    }

    // Nombre total de particules
    int nbPart = 0;
    for(int i = 0 ; i < images.size() ; i++)
        nbPart += images[i].getNbPart();

    // Tableau de toutes les particules
    Particule* part = new Particule[nbPart];
    int ind = 0;
    for(int i = 0 ; i < images.size() ; i++)
    {
        images[i].setPart(part+ind);
        ind += images[i].getNbPart();
    }

    MatriceDemoImageF mat(1024, 768,2,part,nbPart);

    try
    {
        SceneDemoImageF scene(part,mat,images,nbPart);
        scene.bouclePrincipale();
    }
    catch(const Erreur& e) { std::cout << "Erreur !!" << std::endl << e.what() << std::endl; }

    delete[] part;
}

#endif //PROTOTYPE_1_DEMOFINALE
