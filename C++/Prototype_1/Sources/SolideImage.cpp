#include "../Header/SolideImage.h"

SolideImage::SolideImage(double echelle, double l, Matiere* m, const char* image)
 : m_fichier(image), m_part(NULL)
{
    init(Vecteur(), echelle, l, m);
}

void SolideImage::init(Vecteur&& o, double echelle, double l, Matiere* m)
{
    SDL_Surface* img = SDL_LoadBMP(m_fichier);
    if (img == NULL)
    {
        std::stringstream err;
        err << "Le fichier " << m_fichier << " ne peut etre charge : " << std::endl << SDL_GetError();
        throw Erreur(5, err.str());
    }

    int w1 = (int)((double)(img->w)*echelle/l);
    int w2 = (int)(((double)(img->w)*echelle-0.5)/l);
    int h1 = (int)((double)(img->h)*echelle/l/sqrt(3.0));
    int h2 = (int)(((double)(img->h)*echelle/sqrt(3.0)-0.5)/l);

    Vecteur v1 = l*Vecteur(1.0,0.0);
    Vecteur v2 = l*Vecteur(0.5,0.5*sqrt(3.0));
    Vecteur v3 = l*Vecteur(0.0,sqrt(3.0));

    Particule* partTmp = new Particule[w1*h1 + w2*h2];
    for(int i = 0 ; i < w1*h1 + w2*h2 ; i++)
        partTmp[i] = Particule(m,6);

    // Conversion
    PixelSDL* pixels = (PixelSDL*)img->pixels;

    // Première maille
    for(int i = 0 ; i < w1 ; i++)
    {
        for(int j = 0 ; j < h1 ; j++)
        {
            Vecteur pos = i*v1 + j*v3; // Position de la particule en cours sur l'image

            // Coordonnées du pixel associées
            int x = (int)(pos.getX()/echelle);
            int y = (int)(pos.getY()/echelle);

            // Conversion vers SDL_Pixel
            PixelSDL c = pixels[y*img->w + x];

            // Couleur non transparente
            if (c.r != 255 || c.g != 0 || c.b != 255)
            {
                partTmp[i*h1+j].setPosInt(pos+o);
                partTmp[i*h1+j].setCouleur({c.r,c.g,c.b,255});
            }
        }
    }

    // Seconde maille
    for(int i = 0 ; i < w2 ; i++)
    {
        for(int j = 0 ; j < h2 ; j++)
        {
            Vecteur pos = v2 + i*v1 + j*v3; // Position de la particule en cours sur l'image

            // Coordonnées du pixel associées
            int x = (int)(pos.getX()/echelle);
            int y = (int)(pos.getY()/echelle);

            // Conversion vers SDL_Pixel
            PixelSDL c = pixels[y*img->w + x];

            // Couleur non transparente
            if (c.r != 255 || c.g != 0 || c.b != 255)
            {
                partTmp[w1*h1 + i*h2+j].setPosInt(pos+o);
                partTmp[w1*h1 + i*h2+j].setCouleur({c.r,c.g,c.b,255});
            }
        }
    }

    // Liaisons horizontales (1)
    for(int i = 0 ; i < w1-1 ; i++)
    {
        for(int j = 0 ; j < h1 ; j++)
        {
            Particule& p1 = partTmp[i*h1+j];
            Particule& p2 = partTmp[(i+1)*h1+j];
            if (p1.estValide() && p2.estValide())
                p1.lier(&p2);
        }
    }

    // Liaisons horizontales (2)
    for(int i = 0 ; i < w2-1 ; i++)
    {
        for(int j = 0 ; j < h2 ; j++)
        {
            Particule& p1 = partTmp[w1*h1 + i*h2+j];
            Particule& p2 = partTmp[w1*h1 + (i+1)*h2+j];
            if (p1.estValide() && p2.estValide())
                p1.lier(&p2);
        }
    }

    // Diagonale bas-droite
    for(int i = 0 ; i < w2 ; i++)
    {
        for(int j = 0 ; j < h2 ; j++)
        {
            Particule& p1 = partTmp[i*h1+j];
            Particule& p2 = partTmp[w1*h1 + i*h2+j];
            if (p1.estValide() && p2.estValide())
                p1.lier(&p2);
        }
    }

    // Diagonale bas-droite
    for(int i = 0 ; i < w2 ; i++)
    {
        for(int j = 0 ; j < h2 ; j++)
        {
            Particule& p1 = partTmp[i*h1+j];
            Particule& p2 = partTmp[w1*h1 + i*h2+j];
            if (p1.estValide() && p2.estValide())
                p1.lier(&p2);
        }
    }

    // Diagonale haut-gauche
    for(int i = 0 ; i < w1-1 ; i++)
    {
        for(int j = 0 ; j < h1-1 ; j++)
        {
            Particule& p1 = partTmp[(i+1)*h1+(j+1)];
            Particule& p2 = partTmp[w1*h1 + i*h2+j];
            if (p1.estValide() && p2.estValide())
                p1.lier(&p2);
        }
    }

    // Diagonale bas-gauche
    for(int i = 0 ; i < w1-1 ; i++)
    {
        for(int j = 0 ; j < h2 ; j++)
        {
            Particule& p1 = partTmp[(i+1)*h1+j];
            Particule& p2 = partTmp[w1*h1 + i*h2+j];
            if (p1.estValide() && p2.estValide())
                p1.lier(&p2);
        }
    }

    // Diagonale haut-droite
    for(int i = 0 ; i < w2 ; i++)
    {
        for(int j = 0 ; j < h1-1 ; j++)
        {
            Particule& p1 = partTmp[i*h1+(j+1)];
            Particule& p2 = partTmp[w1*h1 + i*h2+j];
            if (p1.estValide() && p2.estValide())
                p1.lier(&p2);
        }
    }
Particule* part = partTmp;
    Particule::rearrangerParticules(partTmp, w1*h1 + w2*h2, m_part, m_nbPart,(m_part==NULL));
    delete[] partTmp;
    SDL_FreeSurface(img);
}

void SolideImage::setPart(Particule* p0)
{
    Particule::rearrangerParticules(m_part,m_nbPart,p0,m_nbPart,false);
    delete[] m_part;
    m_part = p0;
}

void SolideImage::appliquerDV(Vecteur&& v)
{
    for(int i = 0 ; i < m_nbPart ; i++)
        m_part[i].appliquerDV(v);
}