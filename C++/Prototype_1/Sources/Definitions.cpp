#include "../Header/Definitions.h"

namespace def
{
    int width;
    int height;
    int taillePixel;
    int partPP = 1;
    bool grilleAffichee = false;
    bool divisionsAffichees = false;
    int pasGrille = 5;
    int divisionGrille = 5;

    bool liaisonsAffichees = false;

    bool pasFixe = false;
    double dtMax = 0.1;
    Uint32 delaiEntreFrames = 0;

    int nbLiaisons = 6;

    void redefGrille(int width, int height, int taillePixel, int partPP,
                     bool grilleAffichee, bool divisionsAffichees, int pasGrille, int divisionGrille)
    {
        def::width = width;
        def::height = height;
        def::taillePixel = taillePixel;
        def::partPP = partPP;
        def::grilleAffichee = grilleAffichee;
        def::divisionsAffichees = divisionsAffichees;
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
        if (taillePixel > 1)
        {
            partPP *= 2;
            pasGrille /= 2;
            taillePixel *= 2;
            width /= 2;
            height /= 2;
        }
    }

    void echelleMoins()
    {
        if (partPP > 1)
        {
            partPP /= 2;
            pasGrille *= 2;
            taillePixel /= 2;
            width *= 2;
            height *= 2;
        }
    }
}
