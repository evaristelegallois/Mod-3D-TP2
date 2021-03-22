/*THIEL Samantha, PAREL Gabriel, M1 Informatique - TP2 Modélisation de surfaces 3D (22/03/2021)*/

#ifndef COURBEPARAMETRIQUE_H
#define COURBEPARAMETRIQUE_H

#include "parametre.h"


class CourbeParametrique
{
public:
    CourbeParametrique();
    CourbeParametrique(Parametre t, int n);

    int recursiveFact(int value);
    int binomialCoeff(int end, int start);

    float derivedBezierCurve(float pointList[]);
    float bezierCurve(float pointList[]);
    void discretizeBezierCurve();

private:
    Parametre t;
    int n;

};

#endif // COURBEPARAMETRIQUE_H
