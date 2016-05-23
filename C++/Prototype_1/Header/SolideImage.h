#ifndef PROTOTYPE_1_DEMOCHARGEMENT_H
#define PROTOTYPE_1_DEMOCHARGEMENT_H

#include <sstream>
#include <SDL2/SDL.h>
#include "Particule.h"
#include "Erreur.h"

typedef struct
{Uint8 b,g,r;}
PixelSDL;

class SolideImage
{
public:
    // /!\ : Nécessite que l'image soit un bmp 24 bits, aux dimensions multiples de 4 !
    SolideImage(double echelle, double l, const char* image);

    void init(Vecteur&& o, double taille, Matiere* m);
    void setPart(Particule* p0); // Mettre les particules dans un autre tableau
    void appliquerDV(Vecteur&& v);
    int getNbPart() const { return m_nbPart; }

private:
    const char* m_fichier;
    Particule* m_part;
    int m_nbPart;
    double m_echelle;
    double m_l;
};

#endif //PROTOTYPE_1_DEMOCHARGEMENT_H
