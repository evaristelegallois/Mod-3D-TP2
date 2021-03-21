#ifndef DISCRETISATION_H
#define DISCRETISATION_H

#include "segment.h"
#include "parametre.h"

class Discretisation
{
public:
    Discretisation(Segment segment, Parametre p);
    Point getValue(Parametre p);
    float * segmentToTable();
    float getP();

private:
    Segment segment;
    Parametre p;
    float * tablePoint;

};

#endif // DISCRETISATION_H
