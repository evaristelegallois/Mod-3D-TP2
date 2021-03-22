/*THIEL Samantha, PAREL Gabriel, M1 Informatique - TP2 Modélisation de surfaces 3D (22/03/2021)*/

#ifndef DISCRETISATION_H
#define DISCRETISATION_H

#include "segment.h"
#include "parametre.h"
#include "courbeparametrique.h"

class Discretisation : public CourbeParametrique
{
public:
    Discretisation(Segment segment, Parametre p);
    Point getValue(Parametre p);
    float * segmentToTable();
    float getP();
    void courbeBezier(Point P0);

private:
    Segment segment;
    Parametre p;
    float * tablePoint;

};

#endif // DISCRETISATION_H
