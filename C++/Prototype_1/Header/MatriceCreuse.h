#ifndef PROTOTYPE_1_MATRICECREUSE_H
#define PROTOTYPE_1_MATRICECREUSE_H

#include "Vecteur.h"

template<typename T, size_t dimSM>
class MatriceCreuse
{
public:
    MatriceCreuse() {}

    virtual size_t getDimSM() { return dimSM; }

protected:
    double masse;
    Vecteur m_pos;
};

#endif //PROTOTYPE_1_MATRICECREUSE_H
