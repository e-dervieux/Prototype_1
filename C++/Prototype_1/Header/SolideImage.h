#ifndef PROTOTYPE_1_DEMOCHARGEMENT_H
#define PROTOTYPE_1_DEMOCHARGEMENT_H

#include <sstream>
#include <SDL2/SDL.h>
#include "Particule.h"
#include "Erreur.h"

typedef struct
{Uint8 r,g,b;}
PixelSDL;

class SolideImage
{
public:
    SolideImage(double echelle, double l, Matiere* m, const char* image);

    void init(Vecteur&& o, double echelle, double l, Matiere* m);
    void setPart(Particule* p0); // Mettre les particules dans un autre tableau
    void appliquerDV(Vecteur&& v);
    int getNbPart() const { return m_nbPart; }

private:
    const char* m_fichier;
    Particule* m_part;
    int m_nbPart;
};

#endif //PROTOTYPE_1_DEMOCHARGEMENT_H
