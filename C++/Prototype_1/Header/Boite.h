#ifndef BOITE_H_INCLUDED
#define BOITE_H_INCLUDED

#include "Definitions.h"
#include "Particule.h"

class Boite
{
public:
    Boite();

    ~Boite();

    bool estNul() const { return (m_tab==NULL); }

    void set(int x, int y, Particule& p);
    void suppr(int x, int y);

//private:
    Particule* m_tab;
    int m_nbPart;
};

#endif // BOITE_H_INCLUDED
