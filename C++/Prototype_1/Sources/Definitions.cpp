#include "../Header/Definitions.h"

namespace def
{
    int width;
    int height;
    double tailleParticule;
    int coucheAffichage = 0;
    int coucheMax = 1;
    bool grilleAffichee = false;
    bool divisionsAffichees = false;
    int pasGrille = 4;
    int divisionGrille = 4;

    bool liaisonsAffichees = false;

    bool pasFixe = false;
    double dtMax = 0.1;
    Uint32 delaiEntreFrames = 0;

    int nbLiaisons = 6;

    void redefGrille(int width, int height, double tailleParticule, int coucheAffichage, int coucheMax,
                     bool grilleAffichee, bool divisionsAffichees, int pasGrille, int divisionGrille)
    {
        def::width = width;
        def::height = height;
        def::tailleParticule = tailleParticule;
        def::coucheAffichage = coucheAffichage;
        def::coucheMax = coucheMax;
        def::grilleAffichee = grilleAffichee;
        def::divisionsAffichees = divisionsAffichees;
        def::pasGrille = pasGrille;
        def::divisionGrille = divisionGrille;
    }

    void redefTemp(bool pasFixe, double dtMax, Uint32 delaiEntreFrames)
    {
        def::pasFixe = pasFixe;
        def::dtMax = dtMax;
        def::delaiEntreFrames = delaiEntreFrames;
    }


    void echellePlus()
    {
        if (coucheAffichage < coucheMax)
            coucheAffichage++;
    }

    void echelleMoins()
    {
        if (coucheAffichage > 0)
            coucheAffichage--;
    }
}
