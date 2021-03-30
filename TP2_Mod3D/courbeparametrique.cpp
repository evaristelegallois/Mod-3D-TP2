/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#include "courbeparametrique.h"
#include <cmath>

CourbeParametrique::CourbeParametrique(Point start, Point end, Point ctrlPointList[], int n):
    start(start), end(end), ctrlPointList(ctrlPointList), n(n)
{
    //t entre 0 et 1
    pointList = new Point[n]; //liste de points
    this->setStart(start);
    this->setEnd(end);
}

CourbeParametrique::~CourbeParametrique(){
    delete [] pointList;
    pointList = nullptr;
}

/*
Point * CourbeParametrique::derivedBezierCurve(Parametre t, Point pointList[]){
    Point *curve = new Point[3];
    for (int i=0; i<n-1; i++){
        curve[i].setX(binomialCoeff(n-1, i)*pow(((float)1-t.getPValue()),n-1-i)
                *pow((float)t.getPValue(),i)*(pointList[i+1].getX()-pointList[i].getX()));
        curve[i].setY(binomialCoeff(n-1, i)*pow(((float)1-t.getPValue()),n-1-i)
                *pow((float)t.getPValue(),i)*(pointList[i+1].getY()-pointList[i].getY()));
        curve[i].setZ(binomialCoeff(n-1, i)*pow(((float)1-t.getPValue()),n-1-i)
                *pow((float)t.getPValue(),i)*(pointList[i+1].getZ()-pointList[i].getZ()));
    }


    return curve;
}
*/


void CourbeParametrique::setStart(const Point & p)
{
    pointList[0] = p;
}

void CourbeParametrique::setEnd(const Point & p)
{
    pointList[1] = p;
}

Point CourbeParametrique::getStart() const
{
    return start;
}

Point CourbeParametrique::getEnd() const
{
    return end;
}

Point CourbeParametrique::getPointList(int index){
    return pointList[index];
}

Point CourbeParametrique::getCtrlPointList(int index){
    return ctrlPointList[index];
}

int CourbeParametrique::getOrder(){
    return n;
}
