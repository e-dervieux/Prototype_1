#include <exception>

#ifndef PROTOTYPE_1_BRISURE_H
#define PROTOTYPE_1_BRISURE_H

class Particule;

class Brisure : public std::exception
{
public:
    Brisure(Particule* part1, Particule* part2)
     : p1(p1), p2(p2) {}

    ~Brisure() throw() {}

    Particule *p1, *p2;
};

#endif //PROTOTYPE_1_BRISURE_H
