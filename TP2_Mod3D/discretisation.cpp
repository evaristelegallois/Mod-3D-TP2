/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#include "discretisation.h"
#include<QDebug>
#include<cmath>

using namespace std;

Discretisation::Discretisation(Segment segment, Parametre p): segment(segment), p(p){

}

Discretisation::Discretisation(CourbeParametrique curve, Parametre p): curve(curve), p(p){

}

/**
 * @brief Discretisation::segmentToTable : fonction de discrétisation d'un segment
 * @return le segment sous forme de tableau de floats
 */
vector<float> Discretisation::segmentToTable(){

    for(float i = 0; i< (int)p.getPValue(); i++){
        Point newPoint = segment.getValueFromSegment(i/(p.getPValue()-1));
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
Point Discretisation::getValueFromBezierCurve(Parametre t){

    Point newPoint;
    newPoint = curve.getStart();

    //vérifier si t est bien entre 0 et 1
    if(0 < (float) t.getPValue() && (float) t.getPValue() < 1){

        for (int i=0; i<curve.getOrder()+1; i++){
            newPoint.setX(newPoint.getX() + (bernsteinPolynomial(t, i)*
                                             curve.getIndexCtrlPointList(i)->getValueFromSegment(t).getX()));
            newPoint.setY(newPoint.getY() + (bernsteinPolynomial(t, i)*
                                             curve.getIndexCtrlPointList(i)->getValueFromSegment(t).getY()));
            newPoint.setZ(newPoint.getZ() + (bernsteinPolynomial(t, i)*
                                             curve.getIndexCtrlPointList(i)->getValueFromSegment(t).getZ()));

        }
    }
    else if (t.getPValue() == 0) return curve.getStart();  //cas où t=0 -> début de la courbe
    else if (t.getPValue() >= 1) return curve.getEnd();    //cas où t=1 -> fin de la courbe

    return newPoint;
}

float Discretisation::bernsteinPolynomial(Parametre t, int i){
    return binomialCoeff(curve.getOrder(), i)*pow(((float)1-t.getPValue()),
                                                   curve.getOrder()-i)*pow((float)t.getPValue(),i);
}

/**
 * @brief Discretisation::bezierToTable : fonction de discrétisation uniforme d'une courbe
 * @return la courbe de Bézier sous forme de vecteur de floats
 */
vector<float> Discretisation::bezierToTable(){

    for (float i = 0; i < 1 + 0.05; i = i + 0.05) {
        Point newPoint = curve.getValueFromBezierCurve(i);
        tablePoint.push_back(newPoint.getX());
        tablePoint.push_back(newPoint.getY());
        tablePoint.push_back(newPoint.getZ());
        count ++;
    }
    return tablePoint;
}

/**
 * @brief Discretisation::bezierToTable : fonction de discrétisation d'une courbe
 * par avancée de front (voir TP2)
 * @return la courbe de Bézier sous forme de vecteur de floats
 */
vector<float> Discretisation::altBezierToTable(){

    float deltaT = 0.1;

    float seuilBas = 0.0001;  //seuil minimal de déviation
    float seuilHaut = 0.5;  //seuil maximal de déviation
    float h = 0.001; //pour calcul du taux d'accroissement

    Point q0;  //point de départ
    Point q1;
    Point q2, q3;

    QVector3D vec0;
    QVector3D vec1;
    QVector3D vec;

    int i = 0;

    //paramètre t
    Parametre parT = 0;
    float t = parT.getPValue();

    while(t < 1){
        q0 = getValueFromBezierCurve(t);
        q1 = getValueFromBezierCurve(t+deltaT);

        //on calcule les vecteurs q0q2 et q1q3 pour déterminer le taux d'accroissement plutôt
        //que de calculer la dérivée de la courbe de bézier, cela revient au même
        q2 = getValueFromBezierCurve(t+h);
        q3 = getValueFromBezierCurve(t+deltaT+h);
        vec0 = QVector3D(q2.getX()-q0.getX(), q2.getY()-q0.getY(), q2.getZ()-q0.getZ());
        vec1 = QVector3D(q3.getX()-q1.getX(), q3.getY()-q1.getY(), q3.getZ()-q1.getZ());

        //qDebug() << "vec0"<< vec0<<"vec1"<< vec1;

        //angle entre Q(t=0) et Q(t+deltaT)
        float angle = (float) acos(vec.dotProduct(vec0, vec1)/(vec0.length()*vec1.length()));
        //qDebug() << "angle"<< fabsf(angle);

        //si la déviation est trop importante
        if(fabsf(angle) > seuilHaut) {
            deltaT = deltaT/2;  //dichotomie
        }

        //si la déviation est trop minime
        else {
            if(fabsf(angle) < seuilBas) {
                deltaT += deltaT/2;  //on augmente le pas
            }
            else{
                //fabrication d'un nouveau point
                tablePoint[3*i]   = getValueFromBezierCurve((float) i/p.getPValue()).getX();
                tablePoint[1+3*i] = getValueFromBezierCurve((float) i/p.getPValue()).getY();
                tablePoint[2+3*i] = getValueFromBezierCurve((float) i/p.getPValue()).getZ();

                //qDebug() << "tablePointntntn" << tablePoint[3*i];
                //qDebug() << "i" << i;

                t += deltaT;
                count++;
            }
        }
        //i++;
    }

    //parce qu'on atteint jamais 1, on ajoute le dernier point de la courbe manuellement
    tablePoint[3*i]   = curve.getEnd().getX();
    tablePoint[1+3*i] = curve.getEnd().getY();
    tablePoint[2+3*i] = curve.getEnd().getZ();

    return tablePoint;
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
float Discretisation::getP(){
    return p.getPValue();
}

vector<float> Discretisation::getTablePoint(){
    return tablePoint;
}

/**
 * @brief Discretisation::getCount
 * @return le nombre de points de la discrétisation pour Bézier
 */
int Discretisation::getCount(){
    return count;
}

