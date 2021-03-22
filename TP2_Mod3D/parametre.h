/*THIEL Samantha, PAREL Gabriel, M1 Informatique - TP2 Modélisation de surfaces 3D (22/03/2021)*/

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
    void setPValue(float p);
};

#endif // PARAMETRE_H
