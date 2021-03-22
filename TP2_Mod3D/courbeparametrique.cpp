/*THIEL Samantha, PAREL Gabriel, M1 Informatique - TP2 Modélisation de surfaces 3D (22/03/2021)*/

#include "courbeparametrique.h"
#include <cmath>

CourbeParametrique::CourbeParametrique(){}

CourbeParametrique::CourbeParametrique(Parametre t, int n): t(t), n(n)
{
    //t entre 0 et 1 !!!!!
}


/**
 * @brief Discretisation::courbeBezier : discrétisation d'une coure de Bézier par avancée de front
 */
void CourbeParametrique::discretizeBezierCurve(){
    float q0 = bezierCurve(0);
    float qp = 0;
    float deltat = 0.1;
    float seuilBas = 0.01;
    float seuilHaut = 0.5;

    /*float angle = acos()

    while (t<1){
        if (abs(angle)) return 0;

    }*/
}

float CourbeParametrique::bezierCurve(float pointList[]){
    float curve = 0;
    for (int i=0; i<n; i++){
        curve += binomialCoeff(n, i)*pow(((float)1-t.getPValue()),n-i)
                *pow((float)t.getPValue(),i)*pointList[i];
    }
    return curve;
}

float CourbeParametrique::derivedBezierCurve(float pointList[]){
    float curve = 0;
    for (int i=0; i<n-1; i++){
        curve += binomialCoeff(n-1, i)*pow(((float)1-t.getPValue()),n-1-i)
                *pow((float)t.getPValue(),i)*(pointList[i+1]-pointList[i]);
    }
    return n*curve;
}

//implémentation de la fonction factorielle
int CourbeParametrique::recursiveFact(int value)
{
    if(value == 0) return 1;
    else return value*recursiveFact(value-1);
}

//implémentation du calcul du coefficient binomial
int CourbeParametrique::binomialCoeff(int end, int start)
{
    return (recursiveFact(end))/((recursiveFact(end-1)*recursiveFact(start)));
}
