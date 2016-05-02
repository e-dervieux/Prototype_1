#include "../Header/LiaisonsMC.h"

mcprive::LiaisonsMC::LiaisonsMC()
 : h(0), g(0), b(0), d(0), hg(0), bg(0), bd(0), hd(0)
{ }

void mcprive::LiaisonsMC::lier(Direction direction, int nb)
{
    switch(direction)
    {
        case dir::haut:
            h+=nb;
            break;
        case dir::gauche:
            g+=nb;
            break;
        case dir::bas:
            b+=nb;
            break;
        case dir::droite:
            d+=nb;
            break;
        case dir::haut|dir::gauche:
            hg+=nb;
            break;
        case dir::bas|dir::gauche:
            bg+=nb;
            break;
        case dir::bas|dir::droite:
            bd+=nb;
            break;
        case dir::haut|dir::droite:
            hd+=nb;
            break;
        default:
            break;
    }
}

int mcprive::LiaisonsMC::get(Direction direction) const
{
    switch(direction)
    {
        case dir::haut:              return h;
        case dir::gauche:            return g;
        case dir::bas:               return b;
        case dir::droite:            return d;
        case dir::haut|dir::gauche:  return hg;
        case dir::bas|dir::gauche:   return bg;
        case dir::bas|dir::droite:   return bd;
        case dir::haut|dir::droite:  return hd;
        default:                     return -1;
    }
}

int mcprive::LiaisonsMC::getTot() const
{
    return h+g+b+d+hg+bg+bd+hd;
}

void mcprive::LiaisonsMC::reset()
{
    h = g = b = d = hg = bg = bd = hd = 0;
}