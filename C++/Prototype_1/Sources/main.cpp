#include "../Header/demoMatriceParticules.h"
#include "../Header/demoLiaison.h"
#include "../Header/demoCohesion.h"
#include "../Header/demoCollision1.h"
#include "../Header/demoLiaisonsMatrices.h"
#include "../Header/demoCollisionMatrices.h"
#include "../Header/demoCollision2.h"

int main(int argv, char** argc)
{
    bool continuer = true;
    while(continuer)
    {
        int situation;
        std::cout << "Quelle demo realiser ? (0 = stop)" << std::endl
        << " 1 : Matrice de particules" << std::endl
        << " 2 : Rendu graphique d'une liaison entre 2 particules" << std::endl
        << " 3 : Test de cohesion interne de 2 formes" << std::endl
        << " 4 : Premier test de collision entre particules" << std::endl
        << " 5 : Test de liaisons entre sous-matrices" << std::endl
        << " 6 : Collision avec des sous-matrices" << std::endl
        << " 7 : Collision entre 2 formes coherentes" << std::endl;
        std::cin >> situation;
        switch(situation)
        {
            case 1:
                demoMatriceParticules();
                break;
            case 2:
                demoLiaison();
                break;
            case 3:
                demoCohesion();
                break;
            case 4:
                demoCollision1();
                break;
            case 5:
                demoLiaisonsMatrices();
                break;
            case 6:
                demoCollisionMatrices();
                break;
            case 7:
                demoCollision2();
                break;
            default:
                continuer=false;
        }
    }

    return 0;
}
