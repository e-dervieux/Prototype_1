/* Utilis� en compl�ment � SDL_Point. Cette classe g�re les coordonn�es en virgule flottante,
et propose des fonctions utiles comme la conversion vers des SDL_Rect et SDL_Point, et des
op�rateurs. Peut �tre vu comme des coordonn�es de points. */

#ifndef VECTEUR_H_INCLUDED
#define VECTEUR_H_INCLUDED

#include <SDL2/SDL.h>
#include <cmath>
#include <algorithm>

typedef class Vecteur Point;

class Vecteur
{
public:
    Vecteur(double x = 0.0, double y = 0.0);
    Vecteur(const Vecteur& p);
    Vecteur(const Point& a, const Point& b);
    bool estNul() const;
    SDL_Rect toRect() const;
    SDL_Point toPoint() const; // Converti un point en un SDL_Point
    void toPoint(SDL_Point* p) const; // Stocke la conversion en SDL_Point dans un pointeur
    double norme() const;
    double normeCarre() const;
    Vecteur unitaire() const; // M�me vecteur norm� � 1
    Vecteur normal() const; // Retourne un vecteur normal de m�me norme, (v,n) �tant direct
    double mixte(const Vecteur& v) const; // Produit mixte

    // Fonction qui calcule s'il y a intersection entre 2 segments AB et CD
    static bool intersection(Point const& a, Point const& b, Point const& c, Point const& d);

    // Fonction qui calcule si M est dans le triangle ABC
    static bool appartientTriangle(Point const& a, Point const& b, Point const& c, Point const& m);

    double getX() const { return m_x; }
    double getY() const { return m_y; }

    Vecteur& operator*=(double n);
    Vecteur& operator+=(const Vecteur& v);
    Vecteur& operator-=(const Vecteur& v);
    Vecteur& operator=(const Vecteur& p);
    friend Vecteur operator*(const double* mat, const Vecteur& v);

    ~Vecteur();

private:
    double m_x;
    double m_y;
};

bool operator==(const Vecteur& a, const Vecteur& b);
Point operator+(const Point& a, const Vecteur& v);
Vecteur operator-(const Point& a, const Point& b);
Vecteur operator-(const Vecteur& v);
double operator*(Vecteur const& a, Vecteur const& b); // Produit scalaire
Vecteur operator*(Uint32 n, const Vecteur& v);
Vecteur operator*(int n, const Vecteur& v);
Vecteur operator*(double n, const Vecteur& v);

/* Produit matriciel
mat = [a,b,c,d], soit  a c
                       b d
*/
Vecteur operator*(const double* mat, const Vecteur& v);


#endif // VECTEUR_H_INCLUDED
