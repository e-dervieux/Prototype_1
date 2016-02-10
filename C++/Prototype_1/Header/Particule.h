#ifndef PARTICULE_H_INCLUDED
#define PARTICULE_H_INCLUDED

#include <vector>
#include "Vecteur.h"
#include "Matiere.h"

class Particule
{
public:
    Particule() : m_liaisons(NULL), m_matiere(NULL) {}
    Particule(int x, int y, Matiere* matiere = NULL);
    Particule(int x, int y, double xd, double yd, Matiere* matiere = NULL);
    ~Particule();

    Particule& operator=(const Particule& p);

    void creerLiaisons(Particule** liaisons);
    bool lier(Particule* p); // Lie la particule à p. Retourne false si ce n'est pas possible
    void supprimerLiaisons(); // Supprime la particule de la grille (la remettre vide, et supprimer les liaisons)

    int getXInt() const { return m_x; }
    int getYInt() const { return m_y; }
    void setPosInt(int x, int y); // Donne les nouvelles coordonnées entières de la particule
    Vecteur getPos() const { return m_pos; }
    void setPos(Vecteur pos);
    Vecteur getV() const { return m_v; }
    SDL_Color getCouleur() const { return m_matiere->getCouleur(); };
    double getMasse() const;

    void appliquerForce(Vecteur f);
    void appliquerForcesLiaison();
    // Change la position, en fonction des forces prealablement appliquees, et de la duree de deplacement
    void calculerDeplacement(double dt);

private:
    int m_x, m_y; // Pas sur que ca serve...
    Vecteur m_pos, m_v, m_resf; // resf est la resultante des forces a l'instant considere

    //TODO Il faut gérer correctement les passages de l'attribut m_liaisons d'une particule à une autre
    // /!\ Cet attribut n'est pas encore géré par le constructeur de copie
    // S'il est copié, il y a une fuite mémoire !!!
    Particule** m_liaisons;

    Matiere* m_matiere;
};

#endif // PARTICULE_H_INCLUDED
