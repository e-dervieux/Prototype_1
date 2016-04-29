#include "../Header/Particule.h"
#include "../Header/Definitions.h"

Particule::Particule()
 : Element(), m_matiere(NULL),
   m_x(-1), m_y(-1)
{
    m_liaisons = new Particule*[def::nbLiaisons];
    for(int i = 0 ; i < def::nbLiaisons ; i++)
        m_liaisons[i] = NULL;
}

Particule::Particule(int x, int y, Matiere* matiere)
 : Element(Vecteur(x+0.5, y+0.5)), m_x(x), m_y(y),
   m_resf(), m_matiere(matiere)
{
    m_pos2 = m_pos;
    m_v2 = m_v;
    m_liaisons = new Particule*[def::nbLiaisons];
    for(int i = 0 ; i < def::nbLiaisons ; i++)
        m_liaisons[i] = NULL;
}

Particule::Particule(int x, int y, double xd, double yd, Matiere* matiere)
 : Element(Vecteur(xd,yd)), m_x(x), m_y(y),
   m_resf(), m_matiere(matiere)
{
    m_pos2 = m_pos;
    m_v2 = m_v;
    m_liaisons = new Particule*[def::nbLiaisons];
    for(int i = 0 ; i < def::nbLiaisons ; i++)
        m_liaisons[i] = NULL;
}

Particule::Particule(Vecteur&& pos, Matiere* matiere)
: Element(std::move(pos)), m_x((int)pos.getX()), m_y((int)pos.getY()),
  m_resf(), m_matiere(matiere)
{
    m_pos2 = m_pos;
    m_v2 = m_v;
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
    m_pos2 = p.m_pos2;
    m_v = p.m_v;
    m_v2 = p.m_v2;
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

// EULER EXPLICITE
void Particule::calculerDeplacement(double dt)
{
    if (m_matiere != NULL)
    {
        Vecteur a = 1.0/getMasse() * m_resf;
        m_pos2 += dt*(m_v + 0.5*dt*a);
        m_v2 += dt*a;
        m_resf = Vecteur();
    }
}

void Particule::actualiser(double dt)
{
    m_pos = m_pos2;
    m_v = m_v2;
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
    if (m_liaisons == NULL) // Utile ?
        return;

    if (m_matiere == NULL)
        return;

    for(int i = 0 ; i < def::nbLiaisons ; i++)
    {
        Particule* p = m_liaisons[i];
        if (p != NULL)
            appliquerForce(m_matiere->forceLiaison(this, p));
    }
}

void Particule::setPos(Vecteur pos)
{
    m_pos = pos;
    m_pos2 = pos;
}

void Particule::setV(Vecteur v)
{
    m_v = v;
    m_v2 = v;
}

void Particule::setPosInt(Vecteur pos)
{
    m_pos = pos;
    m_pos2 = pos;
    m_x = (int)pos.getX();
    m_y = (int)pos.getY();
}

void Particule::appliquerDV(Vecteur dv)
{
    m_v2 += dv;
    m_v = m_v2;
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

bool Particule::detecterCollisionSM(int x, int y, int tailleSM)
{
    // Suppose que la SM est non vide

    // Si on ne change pas de SM
    if (m_x/tailleSM == x/tailleSM && m_y/tailleSM == y/tailleSM)
        return false;

    // res = collision (il n'y a pas de particule liée à celle-ci dans la nouvelle SM)
    bool res = true;
    for(int i = 0 ; res && i < def::nbLiaisons ; i++)
    {
        Particule* p = m_liaisons[i];
        if (p != NULL)
        {
            res = !(p->m_x/tailleSM == x/tailleSM &&
                    p->m_y/tailleSM == y/tailleSM);
        }
    }

    return res;
}

void Particule::collision(Particule& p)
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
        yCol = m_pos.getY() + vy/vx*(xCol-m_pos.getX());
        deplacementX = true;
    }
    else if (p.m_x > m_x)
    {
        newX = (double)(m_x+1) - OFFSET_COLLISION;

        // Logiquement, vx != 0.0
        xCol = (double)p.m_x;
        yCol = m_pos.getY() + vy/vx*(xCol-m_pos.getX());
        deplacementX = true;
    }

    if (p.m_y < m_y)
    {
        newY = (double)m_y + OFFSET_COLLISION;

        if (!deplacementX || yCol > (double)(p.m_y+1))
        {
            // Logiquement, vy != 0.0
            yCol = (double)(p.m_y+1);
            xCol = m_pos.getX() + vx/vy*(yCol-m_pos.getY());
        }
    }
    else if (p.m_y > m_y)
    {
        newY = (double)(m_y+1) - OFFSET_COLLISION;

        if (!deplacementX || yCol < (double)p.m_y)
        {
            // Logiquement, vy != 0.0
            yCol = (double)p.m_y;
            xCol = m_pos.getX() + vx/vy*(yCol-m_pos.getY());
        }
    }

    setPos(Vecteur(newX,newY));

    // Calcul de la force de collision (peut être optimisé ?)
    Vecteur n(
            p.getPos(), // Centre de p
            Point(xCol,yCol) ); // Point de collision
    Vecteur vr = m_v - p.getV(); // Vitesse relative
    double m1 = getMasse();
    double m2 = p.getMasse();
    Vecteur dvm = -2.0 / (m1+m2) / n.normeCarre()*(vr*n)*n; // Variation de vitesse, à la masse de la particule opposée près

    // Application de la force de collision
    appliquerDV(m2*dvm);
    p.appliquerDV(-m1*dvm);
}

void Particule::collision(Element& e, int x, int y, int taille,
                          bool haut, bool gauche, bool bas, bool droite)
{
    // A ce stade, p et cette particule sont dans la même "boîte"
    // d'après les coordonnées en double, les coordonnées entières ne sont pas les mêmes

    double xCol=0.0, yCol=0.0; // Coordonnées double de la collision (au bord de la "boîte" de p)
    double vx = m_v.getX();
    double vy = m_v.getY();
    bool ch = false, cg  = false, cb = false, cd = false; // Direction par laquelle arrive la particule

    // Réglage de la position : au bord de la "boîte"
    double newX = m_pos.getX();
    double newY = m_pos.getY();

    if (x+taille-1 < m_x)
    {
        newX = (double)m_x + OFFSET_COLLISION;

        // Logiquement, vx != 0.0
        xCol = (double)(x+taille);
        yCol = m_pos.getY() + vy/vx*(xCol-m_pos.getX());
        cd = true;
    }
    else if (x > m_x)
    {
        newX = (double)(m_x+1) - OFFSET_COLLISION;

        // Logiquement, vx != 0.0
        xCol = (double)x;
        yCol = m_pos.getY() + vy/vx*(xCol-m_pos.getX());
        cg = true;
    }

    if (y+taille-1 < m_y)
    {
        newY = (double)m_y + OFFSET_COLLISION;

        if (!(cg||cd) || yCol > (double)(y+taille))
        {
            // Logiquement, vy != 0.0
            yCol = (double)(y+taille);
            xCol = m_pos.getX() + vx/vy*(yCol-m_pos.getY());
        }
        cb = true;
    }
    else if (y > m_y)
    {
        newY = (double)(m_y+1) - OFFSET_COLLISION;

        if (!(cg||cd) || yCol < (double)y)
        {
            // Logiquement, vy != 0.0
            yCol = (double)y;
            xCol = m_pos.getX() + vx/vy*(yCol-m_pos.getY());
        }
        ch = true;
    }

    setPos(Vecteur(newX,newY));

    // Modèle sphérique
    Vecteur n(
            e.getPos(), // Centre de p
            Point(xCol,yCol) ); // Point de collision

    // Gestion des liaisons, rebond sur une surface plane
    if (haut && m_y-y < taille/2 || bas && m_y-y >= taille/2)
        n = Vecteur(cd ? 1.0 : -1.0, 0.0);
    else if (gauche && m_x-x < taille/2 || droite && m_x-x >= taille/2)
        n = Vecteur(0.0, cb ? 1.0 : -1.0);
    Vecteur vr = m_v - e.getV(); // Vitesse relative
    double m1 = getMasse();
    double m2 = e.getMasse();
    Vecteur dvm = -2.0 / (m1+m2) / n.normeCarre()*(vr*n)*n; // Variation de vitesse, à la masse de la particule opposée près

    // Application de la force de collision
    appliquerDV(m2*dvm);
    e.appliquerDV(-m1*dvm);
}

void Particule::afficher(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule)
{
    SDL_Color c = getCouleur();
    SDL_SetRenderDrawColor(rendu, c.r, c.g, c.b, c.a);
    SDL_Rect rect = {(int)(tailleParticule*(double)m_x), (int)(tailleParticule*(double)m_y),(int)tailleParticule,(int)tailleParticule};
    SDL_RenderFillRect(rendu, &rect);
}