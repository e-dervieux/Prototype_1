#include "../Header/Particule.h"
#include "../Header/Definitions.h"

Particule::Particule()
 : Element(), m_matiere(NULL)
{
    m_liaisons = new Particule*[def::nbLiaisons];
    for(int i = 0 ; i < def::nbLiaisons ; i++)
        m_liaisons[i] = NULL;
}

Particule::Particule(int x, int y, Matiere* matiere)
 : Element(Vecteur(x+0.5, y+0.5)), m_x(x), m_y(y), m_resf(), m_matiere(matiere)
{
    m_liaisons = new Particule*[def::nbLiaisons];
    for(int i = 0 ; i < def::nbLiaisons ; i++)
        m_liaisons[i] = NULL;
}

Particule::Particule(int x, int y, double xd, double yd, Matiere* matiere)
 : Element(Vecteur(xd,yd)), m_x(x), m_y(y), m_resf(), m_matiere(matiere)
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

double Particule::getMasse() const
{
    if (m_matiere == NULL)
        return 0.0;
    return m_matiere->getMasse();
}

void Particule::appliquerForce(Vecteur f)
{
    m_resf += f;
}

// EULER
void Particule::calculerDeplacement(double dt)
{
    if (m_matiere != NULL)
    {
        Vecteur a = 1.0/getMasse() * m_resf;
        m_pos += dt*(m_v + 0.5*dt*a);
        m_v += dt*a;
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

void Particule::surligner(SDL_Renderer* rendu, int partPP, int taillePixel, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(rendu, 0, 0, 255, 255);
    SDL_Rect rect = {taillePixel*m_x, taillePixel*m_y,taillePixel,taillePixel};
    SDL_RenderFillRect(rendu, &rect);
}

void Particule::afficherLiaisons(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule)
{
    for(int i = 0 ; i < def::nbLiaisons ; i++)
    {
        Particule* p = m_liaisons[i];
        if (p != NULL)
            SDL_RenderDrawLine(rendu,
                               (int)(tailleParticule*m_pos.getX()),(int)(tailleParticule*m_pos.getY()),
                               (int)(tailleParticule*p->m_pos.getX()),(int)(tailleParticule*p->m_pos.getY()));
    }
}

void Particule::collision(Particule& p, double dt)
{
    // A ce stade, p et cette particule sont dans la même "boîte"
    // d'après les coordonnées en double, les coordonnées entières ne sont pas les mêmes

    double xCol=0.0, yCol=0.0; // Coordonnées double de la collision (au bord de la "boîte" de p)
    double vx = m_v.getX();
    double vy = m_v.getY();
    bool deplacementX = false; // Détecte si cette particule s'est déplacé selon m_x (int)

    // Réglage de la position : au bord de la "boîte"
    double newX = m_pos.getX();
    double newY = m_pos.getY();

    if (p.m_x < m_x)
    {
        newX = (double)m_x + OFFSET_COLLISION;

        // Logiquement, vx != 0.0
        xCol = (double)(p.m_x+1);
        yCol = m_pos.getY() + vy/vx*((double)(p.m_x+1)-m_pos.getX());
        deplacementX = true;
    }
    else if (p.m_x > m_x)
    {
        newX = (double)(m_x+1) - OFFSET_COLLISION;

        // Logiquement, vx != 0.0
        xCol = (double)p.m_x;
        yCol = m_pos.getY() + vy/vx*((double)p.m_x-m_pos.getX());
        deplacementX = true;
    }

    if (p.m_y < m_y)
    {
        newY = (double)m_y + OFFSET_COLLISION;

        if (!deplacementX || yCol > (double)(p.m_y+1))
        {
            // Logiquement, vy != 0.0
            yCol = (double)(p.m_y+1);
            xCol = m_pos.getX() + vx/vy*((double)(p.m_y+1)-m_pos.getY());
        }
    }
    else if (p.m_y > m_y)
    {
        newY = (double)(m_y+1) - OFFSET_COLLISION;

        if (!deplacementX || yCol < (double)p.m_y)
        {
            // Logiquement, vy != 0.0
            yCol = (double)p.m_y;
            xCol = m_pos.getX() + vx/vy*((double)p.m_y-m_pos.getY());
        }
    }

    m_pos = Vecteur(newX,newY);

    // Calcul de la force de collision (peut être optimisé ?)
    Vecteur n(
            p.m_pos, // Centre de p
            Point(xCol,yCol) ); // Point de collision
    Vecteur vr = m_v - p.m_v; // Vitesse relative
    double m1 = getMasse();
    double m2 = p.getMasse();
    Vecteur dvm = -2.0 / (m1+m2) / n.normeCarre()*(vr*n)*n; // Variation de vitesse, à la masse de la particule opposée près

    // Application de la force de collision
    m_v += dvm*m2;
    p.m_v -= dvm*m1;

    std::cout << "Collision : dvm=(" << dvm.getX() << ", " << dvm.getY() << ")" << std::endl;
}

void Particule::afficher(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule)
{
    SDL_Color c = getCouleur();
    SDL_SetRenderDrawColor(rendu, c.r, c.g, c.b, c.a);
    SDL_Rect rect = {(int)(tailleParticule*(double)m_x), (int)(tailleParticule*(double)m_y),(int)tailleParticule,(int)tailleParticule};
    SDL_RenderFillRect(rendu, &rect);
}