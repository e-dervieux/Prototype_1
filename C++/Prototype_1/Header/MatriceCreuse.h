#ifndef MATRICECREUSE_H_INCLUDED
#define MATRICECREUSE_H_INCLUDED

#include <vector>
#include "Erreur.h"

template<typename T>
class EstNul
{
public:
    virtual bool operator()(T&) = 0;
};

template<typename T>
class Defaut
{
public:
    virtual T operator()(int i, int j) = 0;
};

template <typename T>
class Suppression
{
public:
    virtual void operator()(T&) = 0;
};

template<typename T>
class MatriceCreuse
{
public:
    typedef T* SousMatrice;

    /*
     - dimSMX, dimSMY : dimension des sous-matrices
     - dimMPX, dimMPY : nombre en X et Y de sous-matrices

    Utilisation de foncteurs :
     - EstNul(T e) : renvoie si un �l�ment est non-vide
     - defaut(int x, int y) : renvoie l'�l�ment par d�faut mis � la position (x,y)
    */

    MatriceCreuse(int dimSMX, int dimSMY, int dimMPX, int dimMPY, EstNul<T>& estNul, Defaut<T>& defaut, Suppression<T>& suppr)
     : m_dimSMX(dimSMX), m_dimSMY(dimSMY), m_dimMPX(dimMPX), m_dimMPY(dimMPY), m_estNul(estNul), m_defaut(defaut), m_suppr(suppr)
    {
        m_tabSM = new SousMatrice[m_dimMPX*m_dimMPY];
        m_tabCnt = new int[m_dimMPX*m_dimMPY];
        for(int i = 0 ; i < m_dimMPX*m_dimMPY ; i++)
        {
            m_tabSM[i] = NULL;
            m_tabCnt[i] = 0;
        }
    }

    ~MatriceCreuse()
    {
        for(int i = 0 ; i < m_dimMPX*m_dimMPY ; i++)
        {
            if(m_tabSM[i] != NULL)
                delete[] m_tabSM[i];
        }

        delete[] m_tabSM;
        delete[] m_tabCnt;
    }

    virtual bool estNul(int x, int y)
    {
        SousMatrice& mat = m_tabSM[(x/m_dimSMX)*m_dimMPY + y/m_dimSMY];
        if (mat != NULL)
            return m_estNul(mat[(x%m_dimSMX)*m_dimSMY + (y%m_dimSMY)]);
        else
            return true;
    }

    T& get(int x, int y) // A priori pas "const"
    {
        SousMatrice& mat = m_tabSM[(x/m_dimSMX)*m_dimMPY + y/m_dimSMY];
        if (mat != NULL)
            return mat[(x%m_dimSMX)*m_dimSMY + (y%m_dimSMY)];
        else
            throw Erreur(0, "Acces a un element inexistant");
    }

    void set(int x, int y, T e)
    {
        int ind = (x/m_dimSMX)*m_dimMPY + y/m_dimSMY;
        SousMatrice& mat = m_tabSM[ind];

        // V�rifie si la sous-matrice existe d�j�
        if (mat == NULL)
        {
            int xmin = m_dimSMX*(x/m_dimSMX);
            int ymin = m_dimSMY*(y/m_dimSMY);

            mat = new T[m_dimSMX*m_dimSMY];
            for(int i = 0 ; i < m_dimSMX ; i++)
            {
                for(int j = 0 ; j < m_dimSMY ; j++)
                    mat[i*m_dimSMY + j] = m_defaut(xmin+i,ymin+j);
            }
        }

        T& tmp = mat[(x%m_dimSMX)*m_dimMPY + (y%m_dimSMY)];
        if (m_estNul(tmp)) // Compte le nombre d'�l�ments dans la sous-matrice
            m_tabCnt[ind]++;
        tmp = e;
    }

    void suppr(int x, int y)
    {
        int ind = (x/m_dimSMX)*m_dimMPY + y/m_dimSMY;
        SousMatrice& mat = m_tabSM[ind];

        if (mat != NULL)
        {
            T& tmp = mat[(x%m_dimSMX)*m_dimMPY+(y%m_dimSMY)];

            // Compte le nb d'�l�ments dans la sous-matrice
            if (!m_estNul(tmp))
            {
                m_tabCnt[ind]--;
                if (m_tabCnt[ind] == 0)
                {
                    // Supprime la sous-matrice
                    delete[] mat;
                    mat = NULL;
                }
            }

            m_suppr(tmp);
        }
    }

    /*void supprimerSM(int i, int j)
    {
        SousMatrice* mat = m_tabSM[i*m_dimMPY + j];
        if (mat != NULL)
            delete[] mat;
        mat = NULL;
    }*/

    SousMatrice getSM(int i, int j)
    {
        return m_tabSM[i*m_dimMPY + j];
    }

    // Supprime la sous-matrice si tous les �l�ments y sont nuls (par m_EstNul)
    /*void nettoyer(int i, int j)
    {
        SousMatrice& mat = m_tab[i*m_dimMPY + j];
        if(mat != NULL)
        {
            bool tmp = false;
            for(int k = 0 ; k < m_dimSMX*m_dimSMY && !tmp ; k++)
                tmp = m_EstNul(mat[k]);
            if (!tmp) // Si tous les �l�ments sont nuls
            {
                delete[] mat;
                mat = NULL;
            }
        }
    }

    void nettoyerTout()
    {
        for(int i = 0 ; i < m_dimMPX ; i++)
        {
            for(int j = 0 ; j < m_dimMPY ; j++)
                nettoyer(i,j);
        }
    }*/

//protected:
    SousMatrice* m_tabSM;
    int* m_tabCnt;
    const int m_dimSMX, m_dimSMY;
    const int m_dimMPX, m_dimMPY;

    EstNul<T>& m_estNul;
    Defaut<T>& m_defaut;
    Suppression<T>& m_suppr;
};

#endif // MATRICECREUSE_H_INCLUDED
