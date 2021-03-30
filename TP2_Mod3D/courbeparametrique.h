/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#ifndef COURBEPARAMETRIQUE_H
#define COURBEPARAMETRIQUE_H

#include "parametre.h"
#include "point.h"

class CourbeParametrique
{
public:
    CourbeParametrique(Point start, Point end, Point pointList[], int n);
    ~CourbeParametrique();

    void setStart(const Point&);
    void setEnd(const Point&);

    Point getStart() const;
    Point getEnd() const;
    int getOrder(); //degré de la courbe

    Point getPointList(int index);
    Point getCtrlPointList(int index);

private:

    Point start;
    Point end;
    Point * ctrlPointList;
    Point * pointList;
    int n; //degré de la courbe (nombre de points de contrôles - 1)
};

#endif // COURBEPARAMETRIQUE_H
