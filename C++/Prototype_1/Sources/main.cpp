#include <iostream>
#include "../Header/SceneSDL.h"

#include "../Header/Particule.h"
#include "../Header/MatriceCreuse.h"

void matricecreuse();

std::ostream& operator<<(std::ostream& f, Particule p)
{
    return (f << "(" << p.m_pos.getX() << "," << p.m_pos.getY() << " ; " << p.m_matiere << ")");
}

template<typename T>
void afficherMat(T* tab, int n, int p)
{
    if (tab == NULL)
    {
        std::cout << "Matrice vide..." << std::endl;
        return;
    }
    for(int j = 0 ; j < p ; j++)
    {
        for(int i = 0 ; i < n ; i++)
            std::cout << tab[i*p + j] << " ";

        std::cout << std::endl;
    }
    std::cout << std::endl;
}

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
        matricecreuse();
        break;
    default:
        matricecreuse();
    }

    return 0;
}

void matricecreuse()
{
    std::cout << std::endl;

    MatriceCreuse<Particule> m(4,4,4,4,def::testPart,def::defautPart);

    afficherMat(m.m_tabCnt,4,4);
    SDL_Color rouge;
    rouge.r=255; rouge.g=0; rouge.b=0; rouge.unused=0;

    std::cout << "Ajout d'un pixel aux coordonnees (5,7) :" << std::endl;
    Matiere matiere(rouge, 1.0,4.0,1.0,0.0);
    m.set(5,7,Particule(5,7,&matiere));

    afficherMat(m.m_tabCnt,4,4);
    afficherMat(m.getSM(5/4,7/4),4,4);

    std::cout << "Deplacement : " << std::endl;
    Vecteur f(1.0,1.0);
    Particule& p(m.get(5,7));
    p.appliquerForce(f);
    p.calculerDeplacement(0.5);
    afficherMat(m.m_tabCnt,4,4);
    afficherMat(m.getSM(5/4,7/4),4,4);

    std::cout << "Suppression : " << std::endl;
    m.suppr(5,7);
    afficherMat(m.m_tabCnt,4,4);
    afficherMat(m.getSM(5/4,7/4),4,4);
}
