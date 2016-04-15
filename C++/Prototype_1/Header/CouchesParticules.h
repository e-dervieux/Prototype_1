#ifndef PROTOTYPE_1_CouchesParticules_H
#define PROTOTYPE_1_CouchesParticules_H

#include "MatriceCreuse.h"

// Cette classe sert juste à mieux nommer les classes MatriceCreuse, en fonction de la taille des sous-matrices seulement
template<size_t ...dims>
class CouchesParticules;

template<>
class CouchesParticules<> : public MatriceCreuse<Particule,1>
{
public:
    template<typename ...Args>
    CouchesParticules<>(Args... args)
     : MatriceCreuse<Particule,1>(args...)
    {}
};

// La classe CouchesParticules<a, ..., z, 1> équivaut à CouchesParticules<a, ..., z>
template<>
class CouchesParticules<1> : public MatriceCreuse<Particule,1>
{
public:
    template<typename ...Args>
    CouchesParticules<1>(Args... args)
     : MatriceCreuse<Particule,1>(args...)
    {}
};

template<>
template<size_t dimSM, size_t ...autres>
class CouchesParticules<dimSM, autres...> : public MatriceCreuse< CouchesParticules<autres...>, dimSM >
{
public:
    template<typename ...Args>
    CouchesParticules<dimSM, autres...>(Args... args)
     : MatriceCreuse< CouchesParticules<autres...>, dimSM >(args...)
    {}
};

#endif //PROTOTYPE_1_CouchesParticules_H
