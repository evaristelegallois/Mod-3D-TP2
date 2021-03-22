/*THIEL Samantha, PAREL Gabriel, M1 Informatique - TP2 Modélisation de surfaces 3D (22/03/2021)*/

#include "parametre.h"

Parametre::Parametre()
{
}

Parametre::Parametre(float p) : m_par(p)
{
}

float Parametre::getPValue(){
    return m_par;
}
