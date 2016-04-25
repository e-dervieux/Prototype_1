#ifndef PROTOTYPE_1_DEMOCOLLISIONMATRICES_H
#define PROTOTYPE_1_DEMOCOLLISIONMATRICES_H

#include "MatriceParticules.h"
#include "SceneSDL.h"

#ifdef NB_PART
    #undef NB_PART
#endif
#define NB_PART 20

// Pour créer une particule et éliminer les voisines
#define PART_PROJ(p,a1,a2,x,y,vx,vy) \
    m_part[a1].setPosInt(Vecteur(-1.0,-1.0)); \
    m_part[a2].setPosInt(Vecteur(-1.0,-1.0)); \
    m_part[p].setPosInt(Vecteur(x,y)); \
    m_part[p].setV(Vecteur(vx,vy)); \
    m_element.reinit();

using MatriceDemoCM = MatriceParticules<16,8>;

class SceneDemoCM : public SceneSDL
{
public:
    SceneDemoCM(MatriceDemoCM& mat, Particule* part)
    : SceneSDL(mat), m_part(part), m_mat(mat),
      m_m1({255, 0, 0, 255}, 1.0, 3.0, 0.0, 0.0),
      m_m2({0, 0, 255, 255}, 1.0, 3.0, 0.0, 0.0),
      m_appui(false), m_h(false), m_g(false), m_b(false), m_d(false)
    {}

    virtual void actionClavier(bool& continuer, bool& recommencer)
    {
        SceneSDL::actionClavier(continuer, recommencer);

        if (m_clavier[def::K_SHIFT])
            def::delaiEntreFrames = 500;
        else
            def::delaiEntreFrames = 100;

        if (m_config == 2)
        {
            if (m_clavier[def::K_Q] && !m_appui)
            {
                m_appui = true;
                m_mat.afficherLiaisonsSM(8,1);
            }
            else if (!m_clavier[def::K_BAS])
                m_appui = false;

            //    4  5  6
            //    7     8
            //    9 10 11

            m_h = m_h || m_clavier[def::K_HAUT];
            m_b = m_b || m_clavier[def::K_BAS];
            m_g = m_g || m_clavier[def::K_GAUCHE];
            m_d = m_d || m_clavier[def::K_DROITE];

            if (!m_clavier[def::K_HAUT] && !m_clavier[def::K_BAS] && !m_clavier[def::K_GAUCHE] && !m_clavier[def::K_DROITE])
            {
                m_h = false;
                m_b = false;
                m_g = false;
                m_d = false;
            }

            double s = 10.0; // Vitesse des particules projetées

            // Particules qui apparaissent pour entrer en collision
            if (m_b)
            {
                if (!m_d && !m_g) // Bas
                {
                    PART_PROJ(5,4,6, 20.0,12.0, 0.0,s)
                }
                else if (m_d && !m_g) // Bas droite
                {
                    PART_PROJ(4,5,7, 12.0,12.0, s,s);
                }
                else if (!m_d && m_g) // Bas gauche
                {
                    PART_PROJ(6,5,8, 28.0,12.0, -s,s)
                }
            }
            if (m_h)
            {
                if (!m_d && !m_g) // Haut
                {
                    PART_PROJ(10,9,11, 20.0,28.0, 0.0,-s)
                }
                else if (m_d && !m_g) // Haut droite
                {
                    PART_PROJ(9,10,7, 12.0,28.0, s,-s)
                }
                else if (!m_d && m_g) // Haut gauche
                {
                    PART_PROJ(11,8,10, 28.0,28.0, -s,-s)
                }
            }
            else if (!m_h && !m_b)
            {
                if (m_d) // Droite
                {
                    PART_PROJ(7,4,9, 12.0,20.0, s,0.0)
                }
                if (m_g) // Gauche
                {
                    PART_PROJ(8,6,11, 28.0,20.0, -s,0.0)
                }
            }
        }
    }

    virtual void charger(int config)
    {
        for(int i = 0 ; i < NB_PART ; i++)
        {
            m_part[i].setPosInt(Vecteur(-1.0,-1.0));
            m_part[i].supprimerLiaisons();
        }

        switch(config)
        {
            case 2:
                // Carré central
                m_part[0] = Particule(18,18,18.5,18.5,&m_m2);
                m_part[1] = Particule(21,18,21.5,18.5,&m_m2);
                m_part[2] = Particule(18,21,18.5,21.5,&m_m2);
                m_part[3] = Particule(21,21,21.5,21.5,&m_m2);
                m_part[0].lier(&m_part[1]);
                m_part[0].lier(&m_part[2]);
                m_part[1].lier(&m_part[3]);
                m_part[2].lier(&m_part[3]);

                // Autres particules qui rentrent en collision
                for(int i = 4 ; i < 12 ; i++)
                    m_part[i] = Particule(-1,-1,-1.0,-1.0,&m_m1);
                break;

            default:
                m_part[0] = Particule(12,12,12.5,12.5,&m_m1);
                m_part[0].setV(Vecteur(3.0,0.0));
                m_part[1] = Particule(20,12,20.5,12.5,&m_m2);

                m_part[2] = Particule(12,20,12.5,20.5,&m_m1);
                m_part[2].setV(Vecteur(0.0,0.0));
                m_part[3] = Particule(20,20,20.5,20.5,&m_m2);
                m_part[2].lier(&m_part[3]);

                m_titre = "Collision simple entre 2 matrices / 2 particules";
        }

        m_element.reinit();
    }

private:
    Matiere m_m1;
    Matiere m_m2;

    MatriceDemoCM& m_mat;

    Particule* m_part;
    bool m_appui;
    bool m_h, m_g, m_b, m_d;
};

void demoCollisionMatrices()
{
    // Définitions
    def::redefGrille(40,40, 10.0, 0, 1, 2, true, true, 8, 16);
    def::redefTemp(true, 0.1, 100);
    def::liaisonsAffichees = true;

    // Création des particules
    Particule* part = new Particule[NB_PART];

    // Matrice de particules
    MatriceDemoCM mat(40,40, 1, part, NB_PART);

    // Création de la scène
    SceneDemoCM scene(mat, part);

    // Simulation de la scène
    try { scene.bouclePrincipale(); }
    catch(Erreur& err) { std::cout << "Erreur !!" << std::endl << err.what() << std::endl; }

    delete[] part;
}

#endif //PROTOTYPE_1_DEMOCOLLISIONMATRICES_H
