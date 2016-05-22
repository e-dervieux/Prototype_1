#include "../Header/Particule.h"
#include "../Header/Definitions.h"

SDL_Color couleurDefaut = {0,0,0,0};

Particule::Particule(Matiere* matiere, size_t nbLiaisons)
 : Element(), m_nbL(nbLiaisons), m_x(-1), m_y(-1),
   m_matiere(matiere), m_couleur((matiere==NULL) ? couleurDefaut : matiere->getCouleur())
{
    if (nbLiaisons == 0)
        m_liaisons = NULL;
    else
        m_liaisons = new LiaisonPart[nbLiaisons];
}

Particule::Particule(int x, int y, Matiere* matiere, size_t nbLiaisons)
 : Element(Vecteur(x+0.5, y+0.5)), m_nbL(nbLiaisons),
   m_x(x), m_y(y),
   m_resf(), m_matiere(matiere), m_couleur((matiere==NULL) ? couleurDefaut : matiere->getCouleur())
{
    m_pos2 = m_pos;
    m_v2 = m_v;
    if (nbLiaisons == 0)
        m_liaisons = NULL;
    else
        m_liaisons = new LiaisonPart[nbLiaisons];
}

Particule::Particule(Vecteur&& pos, Matiere* matiere, size_t nbLiaisons)
: Element(std::move(pos)), m_nbL(nbLiaisons), m_x((int)pos.getX()), m_y((int)pos.getY()),
  m_resf(), m_matiere(matiere), m_couleur((matiere==NULL) ? couleurDefaut : matiere->getCouleur())
{
    m_pos2 = m_pos;
    m_v2 = m_v;
    if (nbLiaisons == 0)
        m_liaisons = NULL;
    else
        m_liaisons = new LiaisonPart[nbLiaisons];
}

Particule::~Particule()
{
    if (m_liaisons != NULL)
        delete[] m_liaisons;
}

Particule& Particule::operator=(const Particule& p)
{
    // Réattribution et copie des liaisons
    if (m_nbL != p.m_nbL)
    {
        if (m_liaisons != NULL)
            delete[] m_liaisons;

        m_nbL = p.m_nbL;
        m_liaisons = new LiaisonPart[m_nbL];
    }
    for(int i = 0 ; i < m_nbL ; i++)
        m_liaisons[i]=p.m_liaisons[i];

    // Copie de tous les attributs
    m_x = p.m_x;
    m_y = p.m_y;
    m_pos = p.m_pos;
    m_pos2 = p.m_pos2;
    m_v = p.m_v;
    m_v2 = p.m_v2;
    m_resf = p.m_resf;
    m_matiere = p.m_matiere;
    m_couleur = p.m_couleur;

    return *this;
}

bool Particule::estValide() const
{
    return !(m_x == -1 && m_y == -1);
}

bool Particule::lier(Particule* p)
{
    // Une particule ne peut être liée à elle-même
    if (this == p)
        return false;

    //Recherche des indices dans les tableaux de liaisons
    int i, j;
    for(i = 0 ; i < m_nbL && m_liaisons[i].part != NULL && m_liaisons[i].part != p ; i++) ;
    if (i == m_nbL)
        return false;

    for(j = 0 ; j < p->m_nbL && p->m_liaisons[j].part != NULL && p->m_liaisons[j].part != this ; j++) ;
    if (j == p->m_nbL)
        return false;

    // Insère la liaison dans les 2 particules
    m_liaisons[i].part = p;
    m_liaisons[i].bris = false;
    p->m_liaisons[j].part = this;
    p->m_liaisons[j].bris = false;

    // Réorganise les liaisons pour qu'elles soient toujours dans le sens trigo
    reorganiserLiaisons(i);
    p->reorganiserLiaisons(j);

    return true;
}

void Particule::delier(Particule* p)
{
    for(int i = 0 ; i < m_nbL ; i++)
    {
        if (m_liaisons[i].part == p)
        {
            m_liaisons[i] = LiaisonPart();
            return;
        }
    }
}

void Particule::briser(Particule* p)
{
    for(int i = 0 ; i < m_nbL ; i++)
    {
        if (m_liaisons[i].part == p)
        {
            m_liaisons[i].bris = true;
            return;
        }
    }
}

void Particule::reorganiserLiaisons(int k)
{
    bool modif;
    do
    {
        modif = false;

        for (int i = k; i > 0; i--) {
            // Normalement m_liaisons[i] != NULL (cf supprimer liaisons)
            Vecteur v1(m_pos, m_liaisons[i - 1].part->m_pos);
            Vecteur v2(m_pos, m_liaisons[i].part->m_pos);

            // Inversion des 2 liaisons
            if (v1.mixte(v2) < 0) {
                LiaisonPart tmp = m_liaisons[i - 1];
                m_liaisons[i - 1] = m_liaisons[i];
                m_liaisons[i] = tmp;
                modif = true;
            }
        }

        // Si les liaisons forment un cycle
        if (k == m_nbL - 1) {


            Vecteur v1(m_pos, m_liaisons[k].part->m_pos);
            Vecteur v2(m_pos, m_liaisons[0].part->m_pos);

            // Inversion des 2 liaisons
            if (v1.mixte(v2) < 0) {
                LiaisonPart tmp = m_liaisons[k];
                m_liaisons[k] = m_liaisons[0];
                m_liaisons[0] = tmp;
                modif = true;
            }
        }
    } while(modif);
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

void Particule::setMatiere(Matiere* m)
{
    m_matiere = m;
    if (m_couleur.r == 0 && m_couleur.g == 0 && m_couleur.b == 0 && m_couleur.a == 0 && m != NULL)
        m_couleur = m->getCouleur();
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

// Gourmand, mais utile
void Particule::croisementLiaisons()
{
    // On utilise les pos2 pour gérer ça
    for(int i = 0 ; i < m_nbL ; i++)
    {
        Particule* p1 = m_liaisons[i].part;
        Particule* p2 = m_liaisons[(i+1)%m_nbL].part;
        // Se fait même si la liaison est brisée (la liaison est supprimée lorsqu'elle n'est plus considérée)
        if (p1 != NULL && p2 != NULL)
        {
            Vecteur v1(m_pos2,p1->m_pos2);
            Vecteur v2(m_pos2,p2->m_pos2);

            // Si croisement
            if ((v1.mixte(v2) < 0) && (v1*v2 >= 0))
            {
                // On redresse les 2 particules (projection sur l'axe milieu)
                Vecteur v3 = v1+v2;

                // Positions relatives corrigées
                Vecteur v1c = (v1*v3)/v3.normeCarre() * v3;
                Vecteur v2c = (v2*v3)/v3.normeCarre() * v3;

                p1->m_pos2 = m_pos2 + v1c;
                p2->m_pos2 = m_pos2 + v2c;
            }
        }
    }
}

void Particule::actualiser(double dt)
{
    m_pos = m_pos2;
    m_v = m_v2;
}

void Particule::supprimerLiaisons() {
    // Supprime les liaisons, de cette particule et des particules qui y sont liées
    for(int i = 0 ; i < m_nbL ; i++)
    {
        if (m_liaisons[i].part != NULL)
        {
            // Mieux qu'une brisure, suppression de la liaison
            for(int j = 0 ; j < m_liaisons[i].part->m_nbL ; j++)
            {
                if (m_liaisons[i].part->m_liaisons[j].part == this)
                {
                    m_liaisons[i].part->m_liaisons[j] = LiaisonPart();
                    break;
                }
            }
            m_liaisons[i].part = NULL;
            m_liaisons[i].bris = false;
        }
    }
}

std::list<Brisure> Particule::appliquerForcesLiaison()
{
    std::list<Brisure> res;

    if (m_matiere == NULL)
        return res;

    for(int i = 0 ; i < m_nbL ; i++)
    {
        LiaisonPart& lp = m_liaisons[i];
        if (lp.part != NULL)
        {
            try { m_matiere->forceLiaison(this, lp); }
            catch(const Brisure& b)
            {
                // Brise la liaison
                lp.bris = true;
                if (lp.part != NULL)
                    lp.part->briser(this);
                res.push_back(b);
            }
        }
    }

    return res;
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
    bool l = false;

    for(int i = 0 ; i < m_nbL ; i++)
    {
        LiaisonPart& lp = m_liaisons[i];
        if (lp.part != NULL)
        {
            if (lp.bris)
                SDL_SetRenderDrawColor(rendu, 128,128,128,128);
            else
                SDL_SetRenderDrawColor(rendu, 0,255,0,128);
            SDL_RenderDrawLine(rendu,
                               (int)(tailleParticule*m_pos.getX()),(int)(tailleParticule*m_pos.getY()),
                               (int)(tailleParticule*lp.part->m_pos.getX()),(int)(tailleParticule*lp.part->m_pos.getY()));
            l = true;
        }
    }

    // Affichage du centre
    if (!l)
    {
        SDL_SetRenderDrawColor(rendu, 0,255,0,196);

        int x = (int)(tailleParticule*m_pos.getX());
        int y = (int)(tailleParticule*m_pos.getY());

        SDL_RenderDrawPoint(rendu,x,y);
        SDL_RenderDrawPoint(rendu,x+1,y);
        SDL_RenderDrawPoint(rendu,x-1,y);
        SDL_RenderDrawPoint(rendu,x,y+1);
        SDL_RenderDrawPoint(rendu,x,y-1);
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
    for(int i = 0 ; res && i < m_nbL ; i++)
    {
        LiaisonPart& lp = m_liaisons[i];
        if (lp.part != NULL && !lp.bris)
        {
            res = !(lp.part->m_x/tailleSM == x/tailleSM &&
                    lp.part->m_y/tailleSM == y/tailleSM);
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
    Vecteur vr = m_v2 - p.getV2(); // Vitesse relative

    if(vr*n<0)
    {
        double m1 = getMasse();
        double m2 = p.getMasse();
        Vecteur dvm = -2.0 / (m1+m2) / n.normeCarre()*(vr*n)*n; // Variation de vitesse, à la masse de la particule opposée près

        // Application de la force de collision
        appliquerDV(m2*dvm);
        //TODO ça marchera peut-être ! :O
        p.appliquerDV(-m1*dvm);
    }
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
            e.getPos(), // Barycentre de la sous-matrice
            Point(xCol,yCol) ); // Point de collision

    // Gestion des liaisons, rebond sur une surface plane
    if (haut && m_y-y < taille/2 || bas && m_y-y >= taille/2)
        n = Vecteur(cd ? 1.0 : -1.0, 0.0);
    else if (gauche && m_x-x < taille/2 || droite && m_x-x >= taille/2)
        n = Vecteur(0.0, cb ? 1.0 : -1.0);
    //TODO pas de V2 pour les éléments
    Vecteur vr = m_v2 - e.getV(); // Vitesse relative

    if(vr*n<0)
    {
        double m1 = getMasse();
        double m2 = e.getMasse();
        Vecteur dvm = -2.0 / (m1+m2) / n.normeCarre()*(vr*n)*n; // Variation de vitesse, à la masse de la particule opposée près

        // Application de la force de collision
        appliquerDV(m2*dvm);
        e.appliquerDV(-m1*dvm);
    }
}

void Particule::afficher(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule)
{
    SDL_Color c = getCouleur();
    SDL_SetRenderDrawColor(rendu, c.r, c.g, c.b, c.a);
    SDL_Rect rect = {(int)(tailleParticule*(double)m_x), (int)(tailleParticule*(double)m_y),(int)tailleParticule,(int)tailleParticule};
    SDL_RenderFillRect(rendu, &rect);
}

void Particule::rearrangerParticules(Particule* part, int nb, Particule*& newPart, int& newNb, bool allouer)
{
    // Tableaux des correlations d'indices
    int* cor = new int[nb];
    int ind = 0;

    // Correlations, et nombre total de particules valides
    for(int i = 0 ; i < nb ; i++)
    {
        if (part[i].estValide())
        {
            cor[i] = ind;
            ind++;
        }
    }

    // Allocation
    newNb = ind;
    ind = 0;
    if (allouer)
        newPart = new Particule[newNb];

    // Copie des particules, une par une
    for(int i = 0 ; i < nb ; i++)
    {
        if (part[i].estValide())
        {
            Particule& p = newPart[ind]; // Nouvelle particule
            Particule& p2 = part[i]; // Particule à copier
            ind++;

            p=p2;
            // Copie des liaisons
            for(int j = 0 ; j < p.m_nbL ; j++)
            {
                if (p2.m_liaisons[j].part!=NULL)
                    p.m_liaisons[j].part=newPart+cor[p2.m_liaisons[j].part-part];
            }
        }
    }

    delete[] cor;
}