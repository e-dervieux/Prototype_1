#include <iostream>
#include "../Header/SceneSDL.h"

#include "../Header/demoMatriceCreuse.h"
#include "../Header/demoMatriceParticules.h"
#include "../Header/demoGraphique1.h"
#include "../Header/demoLiaison.h"
#include "../Header/demoCohesion.h"
#include "../Header/demoCollision1.h"
#include "../Header/demoMatricesRecursives.h"

int main(int argv, char** argc)
{
    demoMatricesRecursives();

    return 0;

    bool continuer = true;
    while(continuer)
    {
        int situation;
        std::cout << "Quelle demo realiser ? (0 = stop)" << std::endl
        << " 1 : Entiers dans une matrice creuse" << std::endl
        << " 2 : Matrice de particules" << std::endl
        << " 3 : Premier rendu graphique de la situation 1" << std::endl
        << " 4 : Test de liaison physique entre 2 particules" << std::endl
        << " 5 : Test de cohesion a petite echelle" << std::endl
        << " 6 : Test de collision entre quelques particules" << std::endl;
        std::cin >> situation;
        switch(situation)
        {
        case 1:
            demoMatriceCreuse();
            break;
        case 2:
            demoMatriceParticules();
            break;
        case 3:
            demoGraphique1();
            break;
        case 4:
            demoLiaison();
            break;
        case 5:
            demoCohesion();
            break;
        case 6:
            demoCollision1();
            break;
        default:
            continuer=false;
        }
    }

    return 0;
}
