/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#ifndef DISCRETISATION_H
#define DISCRETISATION_H

#include "segment.h"
#include "parametre.h"
#include "courbeparametrique.h"

#include<QVector3D>

class Discretisation
{
public:
    Discretisation(Segment segment, Parametre p);
    Discretisation(CourbeParametrique courbe, Parametre p);

    Point getValueFromSegment(Parametre p);
    float * segmentToTable();

    Point getValueFromBezierCurve(Parametre p);
    Point getValueFromBezierSurface(Parametre t, Parametre s);
    Point derivedBezierCurve(Parametre p);
    float * bezierToTable();
    float * altBezierToTable();

    float bernsteinPolynomial(Parametre t, int i);

    int recursiveFact(int value);
    int binomialCoeff(int end, int start);
    float getP();
    int getCount();

private:
    Segment segment;
    CourbeParametrique courbe;
    Parametre p;
    float * tablePoint = nullptr;
    int compteur;

};

#endif // DISCRETISATION_H
