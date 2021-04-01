/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#ifndef COURBEPARAMETRIQUE_H
#define COURBEPARAMETRIQUE_H

#include "parametre.h"
#include "point.h"
#include "segment.h"
using namespace std;

class CourbeParametrique
{
public:
    CourbeParametrique();
    CourbeParametrique(Point start, Point end,  vector<Segment*> ctrlPointList, int n);
    ~CourbeParametrique();

    void setStart(const Point&);
    void setEnd(const Point&);

    Point getStart() const;
    Point getEnd() const;
    int getOrder(); //degré de la courbe
    Point getValueFromBezierCurve(Parametre t);

    Point getPointList(int index);
    Segment* getIndexCtrlPointList(int index);
    vector<Segment*> getCtrlPointList();

private:

    Point start;
    Point end;
    vector<Segment*> m_control_poly;
    vector<Point> ctrlPointList;
    vector<Point> pointList;
    int n;
};

#endif
