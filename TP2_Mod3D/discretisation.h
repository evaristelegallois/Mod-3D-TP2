/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#ifndef DISCRETISATION_H
#define DISCRETISATION_H

#include "segment.h"
#include "parametre.h"
#include "courbeparametrique.h"

#include<QVector3D>

using namespace std;
class Discretisation
{
public:
    Discretisation(Segment segment, Parametre p);
    Discretisation(CourbeParametrique courbe, Parametre p);

    //Point getValueFromSegment(Parametre p);
    vector<float> segmentToTable();

    Point getValueFromBezierCurve(Parametre p);
    vector<float> bezierToTable();
    vector<float> altBezierToTable();

    int recursiveFact(int value);
    int binomialCoeff(int end, int start);
    float bernsteinPolynomial(Parametre t, int i);
    float getP();
    int getCount();
    vector<float> getTablePoint();

private:
    Segment segment;
    Parametre p;
    CourbeParametrique curve;
    vector<float> tablePoint;
    int count=0;

};

#endif // DISCRETISATION_H
