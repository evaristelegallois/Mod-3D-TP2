/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#include "discretisation.h"
#include<QDebug>
#include<cmath>

using namespace std;

Discretisation::Discretisation(Segment segment, int step): segment(segment), step(step){

}

Discretisation::Discretisation(CourbeParametrique courbe,int step): courbe(courbe), step(step){

}


/**
 * @brief Discretisation::segmentToTable : fonction de discrétisation d'un segment
 * @return le segment sous forme de tableau de floats
 */
vector<float> Discretisation::segmentToTable(){

    for(float i = 0; i< step; i++){
        Point newPoint = segment.getValueFromSegment(i/(step-1));
        tablePoint.push_back(newPoint.getX());
        tablePoint.push_back(newPoint.getY());
        tablePoint.push_back(newPoint.getZ());

        //qDebug() << "tablepoint segment" << tablePoint[3*i];
    }
    return tablePoint;
}


/**
 * @brief Discretisation::getValueFromBezierCurve
 * @param p un paramètre variant de 0 à 1 sur la longueur de la courbe de Bézier
 * @return la position (Point) sur la courbe en fonction du paramètre p
 */


vector<float> Discretisation::bezierToTable(){


    for (float i = 0; i < 1 + 0.05; i = i + 0.05) {
        Point point = courbe.getValueFromBezierCurve(i);
        tablePoint.push_back(point.getX());
        tablePoint.push_back(point.getY());
        tablePoint.push_back(point.getZ());
        compteur ++;
    }
    qDebug()<< " COUNT : "<<compteur;
    return tablePoint;
}


float get_angle(Point A, Point B) {

    QVector3D AB = B-A;
    float length = sqrt(pow(AB.x(),2) + pow(AB.y(),2));

    return acos(AB.x()/length);
}




/**
 * @brief Discretisation::recursiveFact
 * @param value
 * @return la factorielle de value
 */
int Discretisation::recursiveFact(int value){
    if(value == 0) return 1;
    else return value*recursiveFact(value-1);
}

/**
 * @brief Discretisation::binomialCoeff
 * @param end
 * @param start
 * @return le calcul du coefficient binomial entre end (n) et start (k)
 */
int Discretisation::binomialCoeff(int end, int start){
    return (recursiveFact(end))/((recursiveFact(end-start)*recursiveFact(start)));
}

/**
 * @brief Discretisation::getP
 * @return le nombre de points de la discrétisation pour le segment
 */
int Discretisation::getP(){
    return step;
}

vector<float> Discretisation::getTablePoint(){
    return tablePoint;
}

/**
 * @brief Discretisation::getCount
 * @return le nombre de points de la discrétisation pour Bézier
 */
int Discretisation::getCount(){
    return compteur;
}

