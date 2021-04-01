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
