#ifndef PARTICULE_H_INCLUDED
#define PARTICULE_H_INCLUDED

#include <iostream> // DEBUG
#include <vector>
#include "Vecteur.h"
#include "Matiere.h"
#include "Element.h"

class Particule : public Element
{
public:
    Particule();
    Particule(int x, int y, Matiere* matiere = NULL);
    Particule(int x, int y, double xd, double yd, Matiere* matiere = NULL);
    ~Particule();

    Particule& operator=(const Particule& p);

    void creerLiaisons(Particule** liaisons);
    Particule** getLiaisons() { return m_liaisons; }
    bool lier(Particule* p); // Lie la particule à p. Retourne false si ce n'est pas possible
    void supprimerLiaisons(); // Supprime la particule de la grille (la remettre vide, et supprimer les liaisons)

    int getXInt() const { return m_x; }
    int getYInt() const { return m_y; }
    virtual void setPos(Vecteur pos);
    virtual void setV(Vecteur v);
    virtual void appliquerDV(Vecteur dv);
    void setInt(int x, int y); // Donne les nouvelles coordonnées entières de la particule
    void setPosInt(Vecteur pos); // Donne les nouvelles coordonnées double, et actualise les int correspondants
    virtual SDL_Color getCouleur() const { return m_matiere->getCouleur(); }
    virtual double getMasse() const;

    void appliquerForce(Vecteur f);
    void appliquerForcesLiaison();
    void annulerForces() { m_resf = Vecteur(); }
    bool detecterCollisionSM(int x, int y, int tailleSM); // (INUTILE ?) Détecte si une particule liée est dans la sous-matrice d'arrivée de cette particule
    void collision(Particule& p); // Applique la force de collision avec p ET repositionne cette particule
    // Idem avec une sous-matrice, en envoyant les liaisons de la SM
    void collision(Element& e, int x, int y, int taille, bool haut, bool gauche, bool bas, bool droite);
    void calculerDeplacement(double dt);
    // Change la position, en fonction des forces prealablement appliquees, et de la duree de deplacement
    virtual void actualiser(double dt);
    virtual void afficher(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule);
    void surligner(SDL_Renderer* rendu, int partPP, int taillePixel, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    virtual void afficherLiaisons(SDL_Renderer* rendu, int coucheAffichage, double tailleParticule);

private:
    int m_x, m_y;
    Vecteur m_pos2, m_v2; // Positions préparées pour la prochaine frame
    Vecteur m_resf; // resf est la resultante des forces a l'instant considere

    Particule** m_liaisons;

    Matiere* m_matiere;
};

#endif // PARTICULE_H_INCLUDED
