/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#ifndef COURBEPARAMETRIQUE_H
#define COURBEPARAMETRIQUE_H

#include "parametre.h"
#include "point.h"

/**
 * @brief The CourbeParametrique class : création d'une courbe de Bézier
 */
class CourbeParametrique
{
public:
    CourbeParametrique(Point start, Point end, Point pointList[], int n);
    ~CourbeParametrique();

    //getters et setters des points extrêmes de la courbe
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

    //liste de points/points de ctrl
    Point * ctrlPointList;
    Point * pointList;

    int n; //degré de la courbe (nombre de points de ctrl - 1)
};

#endif // COURBEPARAMETRIQUE_H
