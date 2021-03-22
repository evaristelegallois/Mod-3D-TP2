/*THIEL Samantha, PAREL Gabriel, M1 Informatique - TP2 Modélisation de surfaces 3D (22/03/2021)*/

#include "discretisation.h"
#include<QDebug>

Discretisation::Discretisation(Segment segment, Parametre p): segment(segment), p(p)
{
    tablePoint = new float[((int) p.getPValue()+1)*3]; //tableau de sommets
}

/**
 * @brief Discretisation::getValue
 * @param p un paramètre variant de 0 à 1 sur la longueur du segment
 * @return la position (Point) sur le segment en fonction du paramètre p
 */
Point Discretisation::getValue(Parametre p)
{
    Point newPoint;

    //vérifier si p est bien entre 0 et 1
    if(0 < p.getPValue() && p.getPValue() < 1){
        newPoint.setX((1-p.getPValue())*segment.getPointList(0).getX() + p.getPValue()*segment.getPointList(1).getX());
        newPoint.setY((1-p.getPValue())*segment.getPointList(0).getY() + p.getPValue()*segment.getPointList(1).getY());
        newPoint.setZ((1-p.getPValue())*segment.getPointList(0).getZ() + p.getPValue()*segment.getPointList(1).getZ());

    } else if (p.getPValue() == 0) return segment.getStart();
    else if (p.getPValue() == 1) return segment.getEnd();

    qDebug() << "segment values " << newPoint.getX();
    return newPoint;

}

/**
 * @brief Discretisation::segmentToTable : fonction de discrétisation d'un segment
 * @return le segment sous forme de tableau de floats
 */
float* Discretisation::segmentToTable()
{
   for(int i = 0; i<(int)p.getPValue(); i++)
   {
       tablePoint[3*i]=getValue((float) i/p.getPValue()).getX();
       tablePoint[1+3*i]=getValue((float) i/p.getPValue()).getY();
       tablePoint[2+3*i]=getValue((float) i/p.getPValue()).getZ();

       qDebug() << "tablepoint segment" << tablePoint[3*i];
   }
   return tablePoint;
}

float Discretisation::getP()
{
    return p.getPValue();
}


void Discretisation::courbeBezier(Point P0){

    float deltaT = 0.1;
    float t = 0.0;

    float seuilBas = 0.1;  //seuil minimal de déviation
    float seuilHaut = 0.9;  //seuil maximal de déviation

    while(t < 1){

    }
}

