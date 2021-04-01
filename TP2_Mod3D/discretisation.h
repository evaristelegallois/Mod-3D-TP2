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
    Discretisation(Segment segment, int step);
    Discretisation(CourbeParametrique courbe, int step);

    Point getValueFromSegment(float p);
    vector<float>  segmentToTable();

    Point getValueFromBezierCurve(float p);
    Point derivedBezierCurve(float p);
    vector<float> bezierToTable();

    int recursiveFact(int value);
    int binomialCoeff(int end, int start);
    int getP();
    int getCount();
    vector<float> getTablePoint();

private:
    Segment segment;
    int step;
    CourbeParametrique courbe;
    vector<float> tablePoint;
    int compteur=0;

};

#endif // DISCRETISATION_H
