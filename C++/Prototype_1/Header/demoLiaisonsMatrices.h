#ifndef PROTOTYPE_1_DEMOLIAISONSMATRICES_H
#define PROTOTYPE_1_DEMOLIAISONSMATRICES_H

#include "MatriceParticules.h"

#define PART_LIEES(n1,n2, x1,y1, x2,y2) \
    std::cout << "Liaison entre (" << x1 << "," << y1 << ") et (" << x2 << "," << y2 << ")." << std::endl;\
    part[n1] = Particule(x1,y1,&matiere, 2); \
    part[n2] = Particule(x2,y2,&matiere, 2); \
    part[n1].lier(&part[n2]);

void demoLiaisonsMatrices()
{
    Matiere matiere;
    Particule* part = new Particule[10];

    PART_LIEES(0, 1, 2,2, 2,6)
    PART_LIEES(2, 3, 3,3, 5,3)
    PART_LIEES(4, 5, 6,6, 9,10)
    PART_LIEES(6, 7, 13,13, 13,13)

    MatriceParticules<8,4> m(16,16,1,part,8);
    std::cout << "Matrice principale : ";
    m.afficherLiaisonsSM(8,2);
    std::cout << "Sous-matrices : ";
    m.afficherLiaisonsSM(4,1);

    delete[] part;
}

#endif //PROTOTYPE_1_DEMOLIAISONSMATRICES_H
