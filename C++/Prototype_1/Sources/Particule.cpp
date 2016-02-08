#include "../Header/Particule.h"
#include "../Header/Definitions.h"

Particule::Particule(int x, int y, Matiere* matiere)
 : m_x(x), m_y(y), m_pos(x+0.5, y+0.5), m_v(), m_resf(), m_matiere(matiere)
{
    m_liaisons=NULL;
}

Particule::Particule(int x, int y, double xd, double yd, Matiere* matiere)
 : m_x(x), m_y(y), m_pos(xd,yd), m_v(), m_resf(), m_matiere(matiere)
{
    m_liaisons=NULL;
}

Particule::~Particule()
{
    if (m_liaisons != NULL)
    {
        delete[] m_liaisons;
    }
}

void Particule::creerLiaisons(Particule** liaisons)
{


    m_liaisons=new Particule*[def::nbLiaisons];
    for(int i=0;i<def::nbLiaisons;i++)
    {
        m_liaisons[i]=liaisons[i];
    }
}

bool Particule::lier(Particule* p)
{
    if (m_liaisons == NULL)
        m_liaisons = new Particule*[def::nbLiaisons];

    if (p->m_liaisons == NULL)
        p->m_liaisons = new Particule*[def::nbLiaisons];

    int i, j;
    for(i = 0 ; i < def::nbLiaisons && m_liaisons[i] != NULL ; i++) ;
    if (i == def::nbLiaisons)
        return false;

    for(j = 0 ; j < def::nbLiaisons && m_liaisons[j] != NULL ; j++) ;
    if (j == def::nbLiaisons)
        return false;

    m_liaisons[i] = p;
    p->m_liaisons[j] = this;
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

void Particule::supprimerLiaisons() {
    // Supprime les liaisons
    for(int i = 0 ; i < def::nbLiaisons ; i++)
    {
        if (m_liaisons[i] != NULL)
        {
            for(int j = 0 ; j < def::nbLiaisons ; j++)
            {
                if (m_liaisons[i]->m_liaisons[j] == this)
                {
                    m_liaisons[i]->m_liaisons[j] = NULL;
                    break;
                }
            }
        }
        m_liaisons[i] = NULL;
    }
}
