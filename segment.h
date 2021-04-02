/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

/* R. Raffin
 * M1 Informatique, Aix-Marseille Université
 * définition d'une classe Segment
 * Comme pour Point, il faudrait mieux utiliser un std::Vector pour stocker les 2 Poins du segment, cela évite de manipuler des pointeurs (et d'en faire allocation et libération.
 * Un Segment n'a par contre que 2 élements Point, le tbaleau est donc de taille fixe
 * Pour aller plus loin, il faut pouvoir évaluer un point sur le segment en fonction d'un paramètre.
 * Ce paramètre doit appartenir à l'intervalle [0,1] et il faudrait donc stocker/vérifier cet intervalle.
 */

#ifndef SEGMENT_H
#define SEGMENT_H

#include "point.h"
#include "parametre.h"

/**
 * @brief The Segment class : création d'un segment
 */
class Segment
{
private:
    int step;
    Point * pointList;
    Point start;
    Point end;

public:
    Segment();
    Segment(int step, Point start, Point end);
    ~Segment();

    Segment(const Segment&);

    Segment& operator= (const Segment &);


    void setStart(const Point&);
    void setEnd(const Point&);
    void setN(unsigned, const Point&);  //spécifier l'un des 2 points "de contrôle"

    Point getN(unsigned) const;  //récupérer un des points "de contrôle" du segment, le 1er ou le 2e
    Point getStart() const;
    Point getEnd() const;
    int getStep() const;

    Point getPointList(int index);

    float length() const;

    friend std::ostream& operator<<(std::ostream&, const Segment&);

};

#endif // SEGMENT_H
