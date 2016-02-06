#include "../Header/Particule.h"
#include "../Header/Definitions.h"

Particule::Particule(int x, int y, Matiere* matiere)
 : m_x(x), m_y(y), m_pos(x+0.5, y+0.5), m_v(), m_resf(), m_matiere(matiere)
{
    m_liaisons=new Particule*[def::nbLiaisons];
    for(int i=0;i<def::nbLiaisons;i++)
    {
        m_liaisons[i]=NULL;
    }

}

Particule::Particule(int x, int y, double xd, double yd, Matiere* matiere)
 : m_x(x), m_y(y), m_pos(xd,yd), m_v(), m_resf(), m_matiere(matiere)
{
    m_liaisons=new Particule*[def::nbLiaisons];
    for(int i=0;i<def::nbLiaisons;i++)
    {
        m_liaisons[i]=NULL;
    }
}

Particule::~Particule()
{

}

void Particule::creerLiaisons(Particule** liaisons)
{
    for(int i=0;i<def::nbLiaisons;i++)
    {
        m_liaisons[i]=liaisons[i];
    }
}

double Particule::getMasse() const
{
    if (m_matiere == NULL)
        return 0.0;
    return m_matiere->getMasse(*this);
}

void Particule::appliquerForce(Vecteur f)
{
    m_resf += f;
}

void Particule::calculerDeplacement(double dt)
{
    if (m_matiere != NULL)
    {
        m_v += (dt/getMasse())*m_resf;
        m_pos += dt*m_v;
        m_resf = Vecteur();
    }
}
