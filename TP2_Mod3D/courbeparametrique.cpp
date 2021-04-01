/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#include "courbeparametrique.h"
#include <cmath>
#include <QDebug>


CourbeParametrique::CourbeParametrique(){

}

CourbeParametrique::CourbeParametrique(Point start, Point end, vector<Segment*> ctrlPointList, int n):
    start(start), end(end), m_control_poly(ctrlPointList), n(n)
{
    this->setStart(start);
    this->setEnd(end);
}

CourbeParametrique::~CourbeParametrique(){
}

Point CourbeParametrique::getValueFromBezierCurve(Parametre t){
    vector<Segment*> sub;
    sub = m_control_poly;
    while(sub.size() > 1){
        vector<Segment*> child;
        for(int i =0; i < (int) sub.size()-1 ; i++){
            child.push_back(new Segment(sub[i]->getValueFromSegment(t.getPValue()), sub[i+1]->getValueFromSegment(t.getPValue())));
        }
        sub = child;
    }
    qDebug()<< "BEZIER";
    return sub.front()->getValueFromSegment(t.getPValue());
}

void CourbeParametrique::setStart(const Point & p)
{
    pointList.push_back(p);
}

void CourbeParametrique::setEnd(const Point & p)
{
    pointList.push_back(p);
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

Segment* CourbeParametrique::getIndexCtrlPointList(int index){
    return m_control_poly[index];
}

vector<Segment*> CourbeParametrique::getCtrlPointList(){
    return m_control_poly;
}

int CourbeParametrique::getOrder(){
    return n;
}
