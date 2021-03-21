/*THIEL Samantha, PAREL Gabriel, M1 Informatique - TP1 Modélisation de surfaces 3D (05/03/2021)*/

#ifndef PARAMETRE_H
#define PARAMETRE_H


class Parametre
{
private:
    float m_par;

public:
    Parametre();
    Parametre(float p);

    float getPValue();
};

#endif // PARAMETRE_H
