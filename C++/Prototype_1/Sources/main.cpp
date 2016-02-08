#include <iostream>
#include "../Header/SceneSDL.h"

#include "../Header/demoMatriceCreuse.h"

int main(int argv, char** argc)
{
    /*SceneSDL scene;

    try
    {
        scene.bouclePrincipale();
    }
    catch (Erreur& e)
    {
        std::cout << "Erreur de niveau " << e.getNiveau() << " : " << e.what() << std::endl;
    }*/

    int situation;
    std::cout << "Quelle demo realiser ?" << std::endl
              << " 0 : Particules dans une matrice creuse" << std::endl;
    std::cin >> situation;
    switch(situation)
    {
    case 0:
        demoMatriceCreuse();
        break;
    default:
        demoMatriceCreuse();
    }

    return 0;
}
