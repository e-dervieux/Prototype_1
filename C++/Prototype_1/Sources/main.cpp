#include <iostream>
#include "../Header/SceneSDL.h"

#include "../Header/demoMatriceCreuse.h"
#include "../Header/demoMatriceParticules.h"
#include "../Header/demoGraphique1.h"

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
    bool continuer = true;
    while(continuer)
    {
        int situation;
        std::cout << "Quelle demo realiser ? (-1 = stop)" << std::endl
        << " 0 : Entiers dans une matrice creuse" << std::endl
        << " 1 : Matrice de particules" << std::endl
        << " 2 : Premier rendu graphique de la situation 1" << std::endl;
        std::cin >> situation;
        switch(situation)
        {
            case 0:
                demoMatriceCreuse();
                break;

            case 1:
                demoMatriceParticules();
                break;

            case 2:
                demoGraphique1();
                break;

            default:
                continuer=false;
        }
    }

    return 0;
}
