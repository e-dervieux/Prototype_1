#ifndef PROTOTYPE_1_BRISURE_H
#define PROTOTYPE_1_BRISURE_H

class Particule;

class Brisure
{
public:
    Brisure(Particule* part1, Particule* part2)
     : p1(part1), p2(part2) {}

    ~Brisure() throw() {}

    Particule *p1, *p2;
};

#endif //PROTOTYPE_1_BRISURE_H
