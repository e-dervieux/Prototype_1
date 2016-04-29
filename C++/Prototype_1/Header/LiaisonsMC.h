#ifndef PROTOTYPE_1_LIAISONSMC_H
#define PROTOTYPE_1_LIAISONSMC_H

namespace mcprive
{
    typedef unsigned char Direction;

    // Valeurs correspondant aux différentes directions. Peut s'utiliser comme des flags
    namespace dir
    {
        // Chacune des directions
        const Direction haut = 0b00000001;
        const Direction gauche = 0b00000010;
        const Direction bas = 0b00000100;
        const Direction droite = 0b00001000;
    }

    // Contient un ensemble de liaisons
    class LiaisonsMC
    {
    public:
        LiaisonsMC();
        void lier(Direction direction, int nb); // Ajouter/retirer des liaisons selon une direction
        int get(Direction direction) const;
        int getTot() const;
        void reset();

    private:
        int h,g,b,d, hg,bg,bd,hd; // 8 directions (en diagonal)
    };
}

#endif //PROTOTYPE_1_LIAISONSMC_H
