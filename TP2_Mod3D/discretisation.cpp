/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#include "discretisation.h"
#include<QDebug>
#include<cmath>

Discretisation::Discretisation(Segment segment, Parametre p): segment(segment), courbe(courbe), p(p){
    tablePoint = new float[((int) p.getPValue()+1)*3]; //tableau de sommets
}

//constructeur de surface de Bézier spécifiquement
Discretisation::Discretisation(CourbeParametrique courbe, Parametre p, bool isSurface):
    courbe(courbe), p(p), isSurface(isSurface){

    tablePoint = new float[(int)pow(((1/p.getPValue())+1),2)*3]; //tableau de sommets
}

/**
 * @brief Discretisation::getValueFromSegment
 * @param p un paramètre variant de 0 à 1 sur la longueur du segment
 * @return la position (Point) sur le segment en fonction du paramètre p
 */
Point Discretisation::getValueFromSegment(Parametre p){

    Point newPoint;
    newPoint.setX((1-p.getPValue())*segment.getStart().getX() + p.getPValue()*segment.getEnd().getX());
    newPoint.setY((1-p.getPValue())*segment.getStart().getY() + p.getPValue()*segment.getEnd().getY());
    newPoint.setZ((1-p.getPValue())*segment.getStart().getZ() + p.getPValue()*segment.getEnd().getZ());

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
    if(0 < (float) t.getPValue() && (float) t.getPValue() < 1){
        for (int i=0; i<courbe.getOrder()+1; i++){
            newPoint.setX(newPoint.getX() + (bernsteinPolynomial(t, i)*courbe.getCtrlPointList(i).getX()));
            newPoint.setY(newPoint.getY() + (bernsteinPolynomial(t,i)*courbe.getCtrlPointList(i).getY()));
            newPoint.setZ(newPoint.getZ() + (bernsteinPolynomial(t,i)*courbe.getCtrlPointList(i).getZ()));
        }
    }
    else if (t.getPValue() == 0) return courbe.getStart();  //cas où t=0 -> début de la courbe
    else if (t.getPValue() >= 1) return courbe.getEnd();    //cas où t=1 -> fin de la courbe

    return newPoint;
}

/**
 * @brief Discretisation::getValueFromBezierSurface
 * @param t et s deux paramètres variant de 0 à 1 sur la longueur de la surface de Bézier
 * @return la position (Point) sur la courbe en fonction du paramètre t, s
 */
Point Discretisation::getValueFromBezierSurface(Parametre t, Parametre s){

    Point newPoint;
    newPoint.setX(0);
    newPoint.setY(0);
    newPoint.setZ(0);

    for (int i=0; i<courbe.getOrder()+1; i++){
        for (int j=0; j<courbe.getOrder()+1; j++){
            newPoint.setX(newPoint.getX() + (bernsteinPolynomial(t,i)*bernsteinPolynomial(s,j)
                                                 *courbe.getCtrlPointList((courbe.getOrder()+1)*i+j).getX()));
            newPoint.setY(newPoint.getY() + (bernsteinPolynomial(t,i)*bernsteinPolynomial(s,j)
                                                 *courbe.getCtrlPointList((courbe.getOrder()+1)*i+j).getY()));
            newPoint.setZ(newPoint.getZ() + (bernsteinPolynomial(t,i)*bernsteinPolynomial(s,j)
                                                 *courbe.getCtrlPointList((courbe.getOrder()+1)*i+j).getZ()));  
        }
    }
    return newPoint;
}

/**
 * @brief Discretisation::bezierToTable : fonction de discrétisation d'une courbe par avancée de front
 * @return la courbe de Bézier sous forme de tableau de floats
 */
float * Discretisation::bezierToTable(){

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
    float t = 0;

    while(t < 1){
        q0 = getValueFromBezierCurve(t);
        q1 = getValueFromBezierCurve(t+deltaT);

        //on calcule les vecteurs q0q2 et q1q3 pour déterminer le taux d'accroissement plutôt
        //que de calculer la dérivée de la courbe de bézier, cela revient au même
        q2 = getValueFromBezierCurve(t+h);
        q3 = getValueFromBezierCurve(t+deltaT+h);
        vec0 = QVector3D(q2.getX()-q0.getX(), q2.getY()-q0.getY(), q2.getZ()-q0.getZ());
        vec1 = QVector3D(q3.getX()-q1.getX(), q3.getY()-q1.getY(), q3.getZ()-q1.getZ());

        //angle entre Q(t=0) et Q(t+deltaT)
        float angle = (float) acos(vec.dotProduct(vec0, vec1)/(vec0.length()*vec1.length()));

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

                t += deltaT;
                compteur++; //on détermine dynamiquement le nb de points de la discrétisation
            }
        }
    }
    //parce qu'on atteint jamais 1, on ajoute le dernier point manuellement
    tablePoint[3*i]   = courbe.getEnd().getX();
    tablePoint[1+3*i] = courbe.getEnd().getY();
    tablePoint[2+3*i] = courbe.getEnd().getZ();

    return tablePoint;
}

/**
 * @brief Discretisation::altBezierToTable : fonction de discrétisation uniforme d'une courbe
 * @return la surface de Bézier sous forme de tableau de floats
 */
float * Discretisation::altBezierToTable(){
    //compteur = 0;
    float step = p.getPValue();
    for (int i=0; i<= (int) 1/step; i++){
        for (int j=0; j<= (int) 1/step; j++){

                //fabrication d'un nouveau point
                tablePoint[3*(int)(1/step)*i+3*j]   = getValueFromBezierSurface((float) step*i, (float) step*j).getX();
                tablePoint[1+3*(int)(1/step)*i+3*j] = getValueFromBezierSurface((float) step*i, (float) step*j).getY();
                tablePoint[2+3*(int)(1/step)*i+3*j] = getValueFromBezierSurface((float) step*i, (float) step*j).getZ();
                //compteur++;

        }
    }
    //qDebug()<<"count"<<compteur;
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
    return (recursiveFact(end)/(recursiveFact(end-start)*recursiveFact(start)));
}

float Discretisation::bernsteinPolynomial(Parametre t, int i){
    return binomialCoeff(courbe.getOrder(), i)*pow(((float)1-t.getPValue()),
                                                   courbe.getOrder()-i)*pow((float)t.getPValue(),i);
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
    if (isSurface!=true) return compteur;
    else return (int)pow(((1/p.getPValue())+1),2);
}

/**
 * @brief Discretisation::isBezierSurface
 * @return true si l'objet voulu est une surface de Bézier, false sinon
 */
bool Discretisation::isBezierSurface(){
    return isSurface;
}

/**
 * @brief Discretisation::getPoint
 * @return un point S(t,s) sur la surface de Bézier
 */
Point Discretisation::getPoint(Parametre t, Parametre s){
    return getValueFromBezierSurface(t,s);
}
