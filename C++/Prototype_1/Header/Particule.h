#ifndef PARTICULE_H_INCLUDED
#define PARTICULE_H_INCLUDED

#include <vector>
#include "Vecteur.h"
#include "Matiere.h"

class Particule
{
public:
    Particule() : m_matiere(NULL) {}
    Particule(int x, int y, Matiere* matiere = NULL);
    Particule(int x, int y, double xd, double yd, Matiere* matiere = NULL);
    ~Particule();

    void creerLiaisons(Particule** liaisons);

    Vecteur getPos() const { return m_pos; }
    Vecteur getV() const { return m_v; }
    double getMasse() const;

    void appliquerForce(Vecteur f);
    // Change la position, en fonction des forces prealablement appliquees, et de la duree de deplacement
    void calculerDeplacement(double dt);

//TODO faire des accesseurs pour m_x et m_y afin que ça soit propre
//private:
    int m_x, m_y; // Pas sur que ca serve...
    Vecteur m_pos, m_v, m_resf; // resf est la resultante des forces a l'instant considere

    Particule** m_liaisons;


    Matiere* m_matiere;
};

#endif // PARTICULE_H_INCLUDED
