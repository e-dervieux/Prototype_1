#include "../Header/Particule.h"
#include "../Header/Definitions.h"

Particule::Particule(int x, int y, Matiere* matiere)
 : m_x(x), m_y(y), m_pos(x+0.5, y+0.5), m_v(), m_resf(), m_matiere(matiere)
{
    m_liaisons = new Particule*[def::nbLiaisons];
    for(int i = 0 ; i < def::nbLiaisons ; i++)
        m_liaisons[i] = NULL;
}

Particule::Particule(int x, int y, double xd, double yd, Matiere* matiere)
 : m_x(x), m_y(y), m_pos(xd,yd), m_v(), m_resf(), m_matiere(matiere)
{
    m_liaisons = new Particule*[def::nbLiaisons];
    for(int i = 0 ; i < def::nbLiaisons ; i++)
        m_liaisons[i] = NULL;
}

Particule::~Particule()
{
    delete[] m_liaisons;
}

Particule& Particule::operator=(const Particule& p)
{
    m_x = p.m_x;
    m_y = p.m_y;
    m_pos = p.m_pos;
    m_v = p.m_v;
    m_resf = p.m_resf;
    m_matiere = p.m_matiere;

    creerLiaisons(p.m_liaisons);

    return *this;
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
    // Une particule ne peut être liée à elle-même
    if (this == p)
        return false;

    //Recherche des indices dans les tableaux de liaisons
    int i, j;
    for(i = 0 ; i < def::nbLiaisons && m_liaisons[i] != NULL && m_liaisons[i] != p ; i++) ;
    if (i == def::nbLiaisons)
        return false;

    for(j = 0 ; j < def::nbLiaisons && p->m_liaisons[j] != NULL && p->m_liaisons[j] != this ; j++) ;
    if (j == def::nbLiaisons)
        return false;

    m_liaisons[i] = p;
    p->m_liaisons[j] = this;

    return true;
}

void Particule::setInt(int x, int y)
{
    m_x = x;
    m_y = y;
}

void Particule::setPos(Vecteur pos)
{
    m_pos = pos;
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
    // Supprime les liaisons, de cette particule et des particules qui y sont liées
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
            m_liaisons[i] = NULL;
        }
    }
}

void Particule::appliquerForcesLiaison()
{
    if (m_liaisons == NULL)
        return;

    for(int i = 0 ; i < def::nbLiaisons ; i++)
    {
        Particule* p = m_liaisons[i];
        if (p != NULL)
            appliquerForce(m_matiere->forceLiaison(this, p));
    }
}

void Particule::setPosInt(Vecteur pos)
{
    m_pos = pos;
    m_x = (int)pos.getX();
    m_y = (int)pos.getY();
}
