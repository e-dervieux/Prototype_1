#include "../Header/Boite.h"

Boite::Boite()
 : m_tab(NULL), m_nbPart(0)
{

}

Boite::~Boite()
{
    if (m_tab != NULL)
        delete[] m_tab;
}

void Boite::set(int x, int y, Particule& p)
{
    if (m_tab == NULL)
    {
        m_tab = new Particule[def::partPB];
    }
}
