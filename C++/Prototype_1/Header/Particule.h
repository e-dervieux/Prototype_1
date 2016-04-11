#ifndef PARTICULE_H_INCLUDED
#define PARTICULE_H_INCLUDED

#include <iostream> // DEBUG
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
    void setInt(int x, int y); // Donne les nouvelles coordonnées entières de la particule
    void setPosInt(Vecteur pos); // Donne les nouvelles coordonnées double, et actualise les int correspondants
    Vecteur getPos() const { return m_pos; }
    void setPos(Vecteur pos);
    Vecteur getV() const { return m_v; }
    void setV(Vecteur v) { m_v = v; }
    SDL_Color getCouleur() const { return m_matiere->getCouleur(); };
    double getMasse() const;

    void appliquerForce(Vecteur f);
    void appliquerForcesLiaison();
    void annulerForces() { m_resf = Vecteur(); }
    void collision(Particule& p, double dt); // Applique la force de collision avec p ET repositionne cette particule
    // Change la position, en fonction des forces prealablement appliquees, et de la duree de deplacement
    void calculerDeplacement(double dt);
    void surligner(SDL_Renderer* rendu, int partPP, int taillePixel, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    void afficherLiaisons(SDL_Renderer* rendu, int partPP, int taillePixel);

private:
    int m_x, m_y; // Pas sur que ca serve...
    Vecteur m_pos, m_v, m_resf; // resf est la resultante des forces a l'instant considere

    Particule** m_liaisons;

    Matiere* m_matiere;
};

#endif // PARTICULE_H_INCLUDED
