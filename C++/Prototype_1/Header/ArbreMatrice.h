#ifndef ARBREMATRICE_H_INCLUDED
#define ARBREMATRICE_H_INCLUDED

#include <vector>
#include <iostream> // DEBUG

template<typename T>
class ArbreMatrice
{
public:
    ArbreMatrice(int dimX, int dimY, int nbX, int nbY)
    {
        //int nbMax = std::max(nbX, nbY);
        //int profondeur = ceil(log2(nbMax));
        //int tailleTab = ((int)pow(4,profondeur+1)-1)/3;


    }

    ~ArbreMatrice()
    {

    }

//private:
    std::vector< std::pair<int,bool> > arbre;
};

#endif // ARBREMATRICE_H_INCLUDED
