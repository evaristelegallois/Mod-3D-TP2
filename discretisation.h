/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#ifndef DISCRETISATION_H
#define DISCRETISATION_H

#include "segment.h"
#include "parametre.h"
#include "courbeparametrique.h"

#include<QVector3D>

/**
 * @brief The Discretisation class : discrétisation d'éléments géométriques
 */
class Discretisation
{
public:
    //constructeurs pour segments et courbes/surfaces de Bézier
    Discretisation(Segment segment, Parametre p);
    Discretisation(CourbeParametrique courbe, Parametre p, bool isSurface = true);

    //calcul des coordonnées sur le segment + discrétisation
    Point getValueFromSegment(Parametre p);
    float * segmentToTable();

    //permet de calculer les points des courbes/surfaces de Bézier
    Point getValueFromBezierCurve(Parametre p);
    Point getValueFromBezierSurface(Parametre t, Parametre s);

    //fonctions de discrétisation
    float * bezierToTable(); //par avancée de front (courbe)
    float * altBezierToTable(); //uniforme (surface)

    //fonctions de calcul des polynômes de Bernstein
    //factorielle et coefficient binomial
    float bernsteinPolynomial(Parametre t, int i);
    int recursiveFact(int value);
    int binomialCoeff(int end, int start);

    //permet de récupérer les paramètres
    float getP();
    int getCount();
    //vrai si surface de Bézier, faux sinon
    bool isBezierSurface();

    //permet de récuperer un point sur la surface de Bézier
    Point getPoint(Parametre t, Parametre s);

private:
    Segment segment;
    CourbeParametrique courbe;
    Parametre p;

    //points de la courbe
    float * tablePoint = nullptr;
    //nb de points (dépend de la discrétisation)
    int compteur;
    bool isSurface;

};

#endif // DISCRETISATION_H
