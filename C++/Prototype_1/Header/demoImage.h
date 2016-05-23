#ifndef PROTOTYPE_1_DEMOIMAGE_H
#define PROTOTYPE_1_DEMOIMAGE_H

#include "SolideImage.h"
#include "SceneSDL.h"
#include "MatriceParticules.h"

#define L0 2.5
#define L0_TERRAIN 3.0

using MatriceDemoImage = MatriceParticules<16,8,4,2>;

class SceneDemoImage : public SceneSDL
{
public:
    SceneDemoImage(Particule* p, MatriceDemoImage& mat, std::vector<SolideImage>& images, int nbPart)
     : SceneSDL(mat), m_part(p), m_img(images), m_m(2), m_nbPart(nbPart)
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

        int cas = 1;
        switch(config)
        {
            case 2:
                cas = 2;
                break;
            default:
                cas = 1;
        }
        m_m[1] = Matiere({255,255,255,255}, 1.0, L0, 400.0, 8.0);

        m_img[cas].init(Vecteur(30.0,30.0), 1.0, &m_m[1]);
        m_img[cas].appliquerDV(Vecteur(-15.0,0.0));
        m_titre = "Donuts contre un mur";

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

void demoImage()
{
    def::redefGrille(360,200,3.0,2,2,4,true,true,8,16);
    def::redefTemp(true, 0.03, 0);
    def::nbIterationsEuler = 1;

    std::vector<SolideImage> images;
    try
    {
        images.push_back(SolideImage(1.0,L0_TERRAIN,"Images/terrain.bmp"));
        images.push_back(SolideImage(2.5,L0,"Images/imgtest.bmp"));
        images.push_back(SolideImage(0.09,L0,"Images/logo.bmp"));
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

    MatriceDemoImage mat(360,200,2,part,nbPart);

    try
    {
        SceneDemoImage scene(part,mat,images,nbPart);
        scene.bouclePrincipale();
    }
    catch(const Erreur& e) { std::cout << "Erreur !!" << std::endl << e.what() << std::endl; }

    delete[] part;
}

#endif //PROTOTYPE_1_DEMOIMAGE_H
