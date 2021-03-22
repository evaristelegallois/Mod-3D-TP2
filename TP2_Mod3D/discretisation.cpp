/*THIEL Samantha, PAREL Gabriel, M1 Informatique - TP2 Modélisation de surfaces 3D (22/03/2021)*/

#include "discretisation.h"
#include<QDebug>
#include<cmath>

Discretisation::Discretisation(Segment segment, Parametre p): segment(segment), courbe(courbe), p(p)
{
    tablePoint = new float[((int) p.getPValue()+1)*3]; //tableau de sommets
}

Discretisation::Discretisation(CourbeParametrique courbe, Parametre p): courbe(courbe), p(p)
{
    tablePoint = new float[100*3]; //tableau de sommets
}

/**
 * @brief Discretisation::getValue
 * @param p un paramètre variant de 0 à 1 sur la longueur du segment
 * @return la position (Point) sur le segment en fonction du paramètre p
 */
Point Discretisation::getValueFromSegment(Parametre p)
{
    Point newPoint;

    //vérifier si p est bien entre 0 et 1
    if(0 < p.getPValue() && p.getPValue() < 1){
        newPoint.setX((1-p.getPValue())*segment.getPointList(0).getX()
                      + p.getPValue()*segment.getPointList(1).getX());
        newPoint.setY((1-p.getPValue())*segment.getPointList(0).getY()
                      + p.getPValue()*segment.getPointList(1).getY());
        newPoint.setZ((1-p.getPValue())*segment.getPointList(0).getZ()
                      + p.getPValue()*segment.getPointList(1).getZ());

    } else if (p.getPValue() == 0) return segment.getStart();
    else if (p.getPValue() == 1) return segment.getEnd();

    qDebug() << "segment values " << newPoint.getX();
    return newPoint;

}

/**
 * @brief Discretisation::segmentToTable : fonction de discrétisation d'un segment
 * @return le segment sous forme de tableau de floats
 */
//fonction de discretisation
float* Discretisation::segmentToTable()
{
   for(int i = 0; i<(int)p.getPValue(); i++)
   {
       tablePoint[3*i]=getValueFromSegment((float) i/p.getPValue()).getX();
       tablePoint[1+3*i]=getValueFromSegment((float) i/p.getPValue()).getY();
       tablePoint[2+3*i]=getValueFromSegment((float) i/p.getPValue()).getZ();

       qDebug() << "tablepoint segment" << tablePoint[3*i];
   }
   return tablePoint;
}

Point Discretisation::getValueFromBezierCurve(Parametre t)
{
    Point newPoint;
    newPoint = courbe.getStart();

        //vérifier si t est bien entre 0 et 1
        if(0 < t.getPValue() && t.getPValue() < 1){
            //qDebug()<< "index t" << t.getPValue();
            for (int i=0; i<courbe.getOrder()-3; i++){ //-2 car - end et - start -1
                //qDebug()<< "index i" << i;
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
        } else if (t.getPValue() == 0) return courbe.getStart();
        else if (t.getPValue() == 1) return courbe.getEnd();

        return newPoint;
}

float * Discretisation::bezierToTable(){
    //initialisation
    float deltaT = 0.1;
    float seuilBas = 0.1;
    float seuilHaut = 0.9;
    Point q0;
    Point q1;
    QVector3D vec0;
    QVector3D vec1;
    QVector3D vec;
    int i = 0;

    while (p.getPValue()<1){
        qDebug()<< "p" << p.getPValue();
        q0 = getValueFromBezierCurve(p.getPValue());
        q1 = getValueFromBezierCurve(p.getPValue()+deltaT);

        //vecteur Oq0 et Oq1; besoin de trouver points sur les tangentes associées à la place
        vec0 = QVector3D(q0.getX()-0, q0.getY()-0, q0.getZ()-0);
        vec1 = QVector3D(q1.getX()-0, q1.getY()-0, q1.getZ()-0);

        float angle = acos(vec.dotProduct(vec0, vec1)/vec0.length()*vec1.length());

        if (abs(angle) > seuilHaut) {
            qDebug()<< "premier if";
            deltaT = deltaT/2;
        }

        else {
            if (abs(angle) < seuilBas) {
                qDebug()<< "if 2 le retour";
                deltaT += deltaT/2;
            }
            else {
                qDebug()<< "if 3: electric boogaloo";
                tablePoint[i*3]= q1.getX();
                tablePoint[i*3+1]= q1.getY();
                tablePoint[i*3+2]= q1.getZ();

                p.setPValue(p.getPValue()+deltaT);
            }
        }
        qDebug()<< "THE END";
        i++;
        qDebug()<< "i"<<i;
    }
    compteur = i;
    return tablePoint;
}

//implémentation de la fonction factorielle
int Discretisation::recursiveFact(int value)
{
    if(value == 0) return 1;
    else return value*recursiveFact(value-1);
}

//implémentation du calcul du coefficient binomial
int Discretisation::binomialCoeff(int end, int start)
{
    return (recursiveFact(end))/((recursiveFact(end-1)*recursiveFact(start)));
}

//nb de pts de la discrétisation pour le segment
float Discretisation::getP()
{
    return p.getPValue();
}

//nb de pts de la discrétisation pour bézier
int Discretisation::getCount(){
    return compteur;
}


