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
