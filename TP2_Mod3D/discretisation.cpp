/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#include "discretisation.h"
#include<QDebug>
#include<cmath>

Discretisation::Discretisation(Segment segment, Parametre p): segment(segment), courbe(courbe), p(p){
    tablePoint = new float[((int) p.getPValue()+1)*3]; //tableau de sommets
}

Discretisation::Discretisation(CourbeParametrique courbe, Parametre p): courbe(courbe), p(p){
    tablePoint = new float[((int) p.getPValue()+1)*100]; //tableau de sommets
}

/**
 * @brief Discretisation::getValueFromSegment
 * @param p un paramètre variant de 0 à 1 sur la longueur du segment
 * @return la position (Point) sur le segment en fonction du paramètre p
 */
/*Point Discretisation::getValueFromSegment(Parametre p){

    Point newPoint;

    //vérifier si p est bien entre 0 et 1
    if(0 < p.getPValue() && p.getPValue() < 1){
        newPoint.setX((1-p.getPValue())*segment.getPointList(0).getX() + p.getPValue()*segment.getPointList(1).getX());

        newPoint.setY((1-p.getPValue())*segment.getPointList(0).getY() + p.getPValue()*segment.getPointList(1).getY());

        newPoint.setZ((1-p.getPValue())*segment.getPointList(0).getZ() + p.getPValue()*segment.getPointList(1).getZ());
    }
    else if (p.getPValue() == 0) return segment.getStart();  //cas où t=0 -> début du segment
    else if (p.getPValue() == 1) return segment.getEnd();    //cas où t=1 -> fin du segment

    qDebug() << "segment values " << newPoint.getX();
    return newPoint;
}*/

Point Discretisation::getValueFromSegment(Parametre p){

    Point newPoint;
    newPoint.setX((1-p.getPValue())*segment.getStart().getX() + p.getPValue()*segment.getEnd().getX());

    newPoint.setY((1-p.getPValue())*segment.getStart().getY() + p.getPValue()*segment.getEnd().getY());

    newPoint.setZ((1-p.getPValue())*segment.getStart().getZ() + p.getPValue()*segment.getEnd().getZ());


    qDebug() << "segment values " << newPoint.getX();
    return newPoint;
}

/**
 * @brief Discretisation::segmentToTable : fonction de discrétisation d'un segment
 * @return le segment sous forme de tableau de floats
 */
float* Discretisation::segmentToTable(){

    for(int i = 0; i<(int)getP(); i++){

        tablePoint[3*i]   = getValueFromSegment((float) i/(getP()-1)).getX();
        tablePoint[1+3*i] = getValueFromSegment((float) i/(getP()-1)).getY();
        tablePoint[2+3*i] = getValueFromSegment((float) i/(getP()-1)).getZ();

        qDebug() << "tablepoint segment" << tablePoint[3*i];
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
    newPoint = courbe.getStart();

    //vérifier si t est bien entre 0 et 1
    if(0 < t.getPValue() && t.getPValue() < 1){
        for (int i=0; i<courbe.getOrder()-3; i++){ //-2 car - end et - start -1

            newPoint.setX(newPoint.getX() + (binomialCoeff(courbe.getOrder(), i)
                                             *pow(((float)1-t.getPValue()),courbe.getOrder()-i)
                                                *pow((float)t.getPValue(),i)*courbe.getCtrlPointList(i).getX()));
            newPoint.setY(newPoint.getY() + (binomialCoeff(courbe.getOrder(), i)
                                             *pow(((float)1-t.getPValue()),courbe.getOrder()-i)
                                                *pow((float)t.getPValue(),i)*courbe.getCtrlPointList(i).getY()));
            newPoint.setZ(newPoint.getZ() + (binomialCoeff(courbe.getOrder(), i)
                                             *pow(((float)1-t.getPValue()),courbe.getOrder()-i)
                                                *pow((float)t.getPValue(),i)*courbe.getCtrlPointList(i).getZ()));
        }
    }
    else if (t.getPValue() == 0) return courbe.getStart();  //cas où t=0 -> début de la courbe
    else if (t.getPValue() == 1) return courbe.getEnd();    //cas où t=1 -> fin de la courbe

    return newPoint;
}


/**
 * @brief Discretisation::bezierToTable : fonction de discrétisation d'une courbe
 * @return la courbe de Bézier sous forme de tableau de floats
 */
float * Discretisation::bezierToTable(){

    float deltaT = 0.1;

    float seuilBas = 0.1;  //seuil minimal de déviation
    float seuilHaut = 0.3;  //seuil maximal de déviation

    Point q0;  //point de départ
    Point q1;

    QVector3D vec0;
    QVector3D vec1;
    QVector3D vec;

    int i = 0;

    //paramètre t
    float t = p.getPValue();

    while(t < 1){
        q0 = getValueFromBezierCurve(t);
        q1 = getValueFromBezierCurve(t+deltaT);

        //vecteur Oq0 et Oq1; besoin de trouver points sur les tangentes associées à la place
        vec0 = QVector3D(q0.getX()-0, q0.getY()-0, q0.getZ()-0);
        vec1 = QVector3D(q1.getX()-0, q1.getY()-0, q1.getZ()-0);

        //angle entre Q(t=0) et Q(t+deltaT)
        float angle = acos(vec.dotProduct(vec0, vec1)/vec0.length()*vec1.length());

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

                qDebug() << "tablePointntntn" << tablePoint;

                t += deltaT;
            }
        }
        i++;
    }
    compteur = i;
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
    return (recursiveFact(end))/((recursiveFact(end-1)*recursiveFact(start)));
}

/**
 * @brief Discretisation::getP
 * @return le nombre de points de la discrétisation pour le segment
 */
float Discretisation::getP(){
    return p.getPValue();
}

/**
 * @brief Discretisation::getCount
 * @return le nombre de points de la discrétisation pour Bézier
 */
int Discretisation::getCount(){
    return compteur;
}


