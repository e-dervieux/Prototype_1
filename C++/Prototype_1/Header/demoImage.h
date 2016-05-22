#ifndef PROTOTYPE_1_DEMOIMAGE_H
#define PROTOTYPE_1_DEMOIMAGE_H

#include "SolideImage.h"
#include "SceneSDL.h"
#include "MatriceParticules.h"

using MatriceDemoImage = MatriceParticules<16,8,4>;

class SceneDemoImage : public SceneSDL
{
public:
    SceneDemoImage(Particule* p, MatriceDemoImage& mat, SolideImage& img, Matiere* m)
     : SceneSDL(mat), m_part(p), m_img(img), m_m(m)
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
        switch(config)
        {
            default:
                m_img.init(Vecteur(20.0,20.0), 1.0, 2.0, m_m);
        }

        m_element.reinit();
    }

private:
    Particule* m_part;
    SolideImage& m_img;
    Matiere* m_m;
};

void demoImage()
{
    def::redefGrille(90,50,10.0,0,2,3,true,true,8,16);
    def::redefTemp(true, 0.03, 0);
    def::nbIterationsEuler = 1;

    Matiere matiere({255,0,0,255},1.0,2.0,0.0);
    SolideImage img1(1.0,2.0,&matiere,"E:/imgtest.bmp");

    int nbPart = img1.getNbPart();
    Particule* part = new Particule[nbPart];
    img1.setPart(part);

    MatriceDemoImage mat(200,200,1,part,nbPart);

    try
    {
        SceneDemoImage scene(part,mat,img1,&matiere);
        scene.bouclePrincipale();
    }
    catch(const Erreur& e) { std::cout << "Erreur !!" << std::endl << e.what() << std::endl; }
}

#endif //PROTOTYPE_1_DEMOIMAGE_H
