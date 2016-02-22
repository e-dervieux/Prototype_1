#include "../Header/Definitions.h"

namespace def
{
    int width;
    int height;
    int taillePixel;
    int partPP;
    bool grilleAffichee;
    bool divisionsAffichees;
    int pasGrille;
    int divisionGrille;

    int nbLiaisons = 6;
    double dtMax = 0.15; // Intervalle max consid�r� entre 2 frames (en s)

    void redefinir(int w, int h, int tp, bool ga, bool da, int pg, int dg)
    {
        width = w;
        height = h;
        taillePixel = tp;
        grilleAffichee = ga;
        divisionsAffichees = da;
        pasGrille = pg;
        divisionGrille = dg;
    }
}
