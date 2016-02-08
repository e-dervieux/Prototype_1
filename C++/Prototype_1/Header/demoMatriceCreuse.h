//
// Created by Marc on 08/02/2016.
//

#ifndef PROTOTYPE_1_DEMOMATRICECREUSE_H_H
#define PROTOTYPE_1_DEMOMATRICECREUSE_H_H

#include <iostream>
#include "Particule.h"

class DefautInt : public Defaut<int>
{
public:
    int operator()(int,int) { return 0; }
};

class EstNulInt : public EstNul<int>
{
public:
    bool operator()(int& x) { return x==0; }
};

class SupprInt : public Suppression<int>
{
    void operator()(int& x) { x = 0; }
};

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


void demoMatriceCreuse()
{
    std::cout << std::endl;
    DefautInt defaut;
    EstNulInt estNul;
    SupprInt suppr;

    MatriceCreuse<int> m(4,4,4,4,estNul,defaut,suppr);

    afficherMat(m.m_tabCnt,4,4);


    std::cout << "On met 42 aux coordonnees (5,7) :" << std::endl;
    m.set(5,7,42);
    afficherMat(m.m_tabCnt,4,4);
    afficherMat(m.getSM(5/4,7/4),4,4);

    std::cout << "On met 10 aux coordonnees (2,2) :" << std::endl;
    m.set(2,2,10);
    afficherMat(m.m_tabCnt,4,4);
    afficherMat(m.getSM(2/4,2/4),4,4);

    std::cout << "Suppression de 10 en (2,2) : " << std::endl;
    m.suppr(2,2);
    afficherMat(m.m_tabCnt,4,4);
    afficherMat(m.getSM(2/4,2/4),4,4);
}

#endif //PROTOTYPE_1_DEMOMATRICECREUSE_H_H
