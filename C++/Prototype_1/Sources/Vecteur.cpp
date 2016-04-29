#include "../Header/Vecteur.h"

Vecteur::Vecteur()
 : m_x(0.0), m_y(0.0)
{}

Vecteur::Vecteur(double x, double y)
 : m_x(x), m_y(y)
{}

Vecteur::Vecteur(const Vecteur& p)
 : m_x(p.m_x), m_y(p.m_y)
{}

Vecteur::Vecteur(const Vecteur&& p)
 : m_x(p.m_x), m_y(p.m_y)
{}

Vecteur::Vecteur(const Point& a, const Point& b)
 : m_x(b.m_x - a.m_x),  m_y(b.m_y - a.m_y)
{}

Vecteur::~Vecteur()
{}

bool Vecteur::estNul() const
{
    return (m_x==0) && (m_y==0);
}

SDL_Rect Vecteur::toRect() const
{
    SDL_Rect res;
    res.x = (int)m_x;
    res.y = (int)m_y;

    return res;
}

SDL_Point Point::toPoint() const
{
    SDL_Point res;
    res.x = (int)m_x;
    res.y = (int)m_y;

    return res;
}

void Point::toPoint(SDL_Point *p) const
{
    p->x = (int)m_x;
    p->y = (int)m_y;
}

double Vecteur::norme() const
{
    return sqrt(m_x*m_x + m_y*m_y);
}

double Vecteur::normeCarre() const
{
    return m_x*m_x + m_y*m_y;
}

Vecteur Vecteur::unitaire() const
{
    if (m_x == 0 && m_y == 0)
        return Vecteur();
    else
        return (1.0/norme())*(*this);
}

Vecteur Vecteur::normal() const
{
    return Vecteur(-m_y, m_x);
}

double Vecteur::mixte(const Vecteur& v) const
{
    return m_x*v.m_y - m_y*v.m_x;
}

bool Vecteur::intersection(Point const& a, Point const& b, Point const& c, Point const& d)
{
    Vecteur ab(a,b);
    Vecteur cd(c,d);

    if (ab.estNul())
    {
        if (cd.estNul())
            return false;
        else
            return ( cd.mixte(Vecteur(c,a)) == 0 && cd*Vecteur(c,a) >= 0 && cd*Vecteur(d,a) <= 0 );
    }
    else if (cd.estNul())
        return ( ab.mixte(Vecteur(a,c)) == 0 && ab*Vecteur(a,c) >= 0 && ab*Vecteur(b,c) <= 0 );
    else
        return ( ab.mixte(Vecteur(a,c))*ab.mixte(Vecteur(a,d)) <= 0 && cd.mixte(Vecteur(c,a))*cd.mixte(Vecteur(c,b)) <= 0 );
}

bool Vecteur::appartientTriangle(Point const& a, Point const& b, Point const& c, Point const& m)
{
    Vecteur ab(a,b);
    Vecteur bc(b,c);
    Vecteur ac(a,c);
    Vecteur am(a,m);
    Vecteur bm(b,m);
    Vecteur cm(c,m);

    if (ab.estNul() || bc.estNul() || ac.estNul())
        return false;

    return (ab.mixte(ac)*ab.mixte(am) >= 0) && (bc.mixte(ab)*bc.mixte(bm) <= 0) && (ac.mixte(bc)*ac.mixte(cm) <= 0);
}

bool operator==(const Vecteur& a, const Vecteur& b)
{
    return (a.getX() == b.getX() && a.getY() == b.getY());
}

Vecteur operator+(const Point& a, const Vecteur& v)
{
    return Vecteur(a.getX() + v.getX(), a.getY() + v.getY());
}

Vecteur operator-(const Point& a, const Point& b)
{
    return Vecteur(a.getX() - b.getX(), a.getY() - b.getY());
}

Vecteur operator-(const Vecteur& v)
{
    return Vecteur(-v.getX(), -v.getY());
}

Vecteur operator*(Uint32 n, const Vecteur& v)
{
    return Vecteur(v.getX() * n, v.getY() * n);
}

Vecteur operator*(double n, const Vecteur& v)
{
    return Vecteur(v.getX() * n, v.getY() * n);
}

Vecteur operator*(int n, const Vecteur& v)
{
    return Vecteur(v.getX() * n, v.getY() * n);
}

Vecteur& Vecteur::operator+=(const Vecteur& v)
{
    m_x += v.m_x;
    m_y += v.m_y;

    return *this;
}

Vecteur& Vecteur::operator-=(const Vecteur& v)
{
    m_x -= v.m_x;
    m_y -= v.m_y;

    return *this;
}

Vecteur& Vecteur::operator=(const Vecteur& p)
{
    m_x = p.m_x;
    m_y = p.m_y;

    return *this;
}

Vecteur& Vecteur::operator*=(double n)
{
    m_x *= n;
    m_y *= n;

    return *this;
}

double operator*(Vecteur const& a, Vecteur const& b)
{
    return a.getX()*b.getX() + a.getY()*b.getY();
}

// Suppose que la matrice est 2x2
Vecteur operator*(const double* mat, const Vecteur& v)
{
    return Vecteur(mat[0]*v.m_x + mat[2]*v.m_y,
                   mat[1]*v.m_x + mat[3]*v.m_y);
}
