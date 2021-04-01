/*THIEL Samantha, PAREL Gabriel, M1 Informatique - Projet Modélisation de surfaces 3D (22/03/2021)*/

#include "point.h"

Point::Point()
{
	coords = new float[3];
}

Point::~Point()
{
	delete [] coords;
	coords = nullptr;
}

Point::Point(const Point& p)
{
	coords = new float[3];
	for (unsigned i=0; i<3; ++i)
		coords[i] = p.coords[i];
}

float Point::getX() const
{
	return getN(0);
}

float Point::getY() const
{
	return getN(1);
}

float Point::getZ() const
{
	return getN(2);
}

void Point::get(float * t) const
{
	for (unsigned i=0; i<3; ++i)
		t[i] = coords[i];
}

float Point::getN(unsigned r) const
{
	if (r>2)
		r=2;
	return coords[r] ;
}

void Point::setN(unsigned r, const float & v)
{
	if (r>2)
		r=2;
	coords[r] = v;
}

void Point::set(const float * t)
{
	for (unsigned i=0; i<3; ++i)
		coords[i] = t[i];
}

void Point::setX(const float& v)
{
	setN(0, v);
}
void Point::setY(const float& v)
{
	setN(1, v);
}
void Point::setZ(const float& v)
{
	setN(2, v);
}

/**
 * @brief Point::operator = : surcharge opérateur affectation
 * @param p point
 * @return le point avec la nouvelle valeur affectée
 */
Point& Point::operator= (const Point &p)
{
	for (unsigned i=0; i<3; ++i)
		coords[i] = p.coords[i];

    return *this;
}

/**
 * @brief Point::operator + : surcharge opérateur somme
 * @param p point
 * @return le point qu'on a sommé avec un autre point
 */
float* operator+ (const Point &p1, const Point &p2)
{
    float* somme = new float[3];
    for (unsigned i=0; i<3; ++i)
        somme[i] += p1.coords[i] + p2.coords[i];

    return somme;
}

Point Point::translate(float x , float y , float z)
{
    Point point;
    point.setX(this->getX()+x);
    point.setY(this->getY()+y);
    point.setZ(this->getZ()+z);
    return point;
}

/**
 * @brief Point::operator * : surcharge opérateur scalaire
 * @param p point
 * @return le scalaire entre un point et un autre
 */
float* operator* (const Point &p1, const Point &p2)
{
    float* scalaire = new float[3];
    for (unsigned i=0; i<3; ++i)
        scalaire[i] *= p1.coords[i]*p2.coords[i];

    return scalaire;
}

/**
 * @brief operator << : surcharge de l'opérateur d'affichage dans le terminal
 * @param out : flux de sortie
 * @param p le point à afficher
 * @return l'affichage d'un point sous forme textuelle
 */
std::ostream& operator<<(std::ostream& out, const Point& p)
{
	return out << "[ " << p.coords[0] << " " << p.coords[1] << " " << p.coords[2] << " ]"; // can access private member Y::data
}
