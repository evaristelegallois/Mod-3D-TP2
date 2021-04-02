/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#ifndef PARAMETRE_H
#define PARAMETRE_H

/**
 * @brief The Parametre class : paramètre entre 0 et 1 pour avoir la position d'un point sur un élément géométrique
 */
class Parametre
{
private:
    float m_par;

public:
    Parametre();
    Parametre(float p);

    float getPValue();
    void setPValue(float p);
};

#endif // PARAMETRE_H
