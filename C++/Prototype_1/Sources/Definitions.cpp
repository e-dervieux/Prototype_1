#include "../Header/Definitions.h"

namespace def
{
    int width;
    int height;
    int taillePixel;
    int partPP;
    bool grilleAffichee;
    int pasGrille;
    int divisionGrille;

    int nbLiaisons = 6;
    double dtMax = 0.2; // Intervalle max consid�r� entre 2 frames (en s)

    void redefinir(int w, int h, int tp, bool ga, int pg, int dg)
    {
        width = w;
        height = h;
        taillePixel = tp;
        grilleAffichee = ga;
        pasGrille = pg;
        divisionGrille = dg;
    }
}
