/*THIEL Samantha, PAREL Gabriel, M1 Informatique - TP1 Modélisation de surfaces 3D (05/03/2021)*/

#include <cmath>
#include "segment.h"
#include "myopenglwidget.h"

Segment::Segment()
{
    pointList = new Point[2];
}

Segment::Segment(int step, Point start, Point end): step(step), start(start), end(end)
{
    pointList = new Point[2]; //liste de points
    this->setStart(start);
    this->setEnd(end);
}

Segment::~Segment()
{
	delete [] pointList;
	pointList = nullptr;
}

Segment::Segment(const Segment & s)
{
	pointList = new Point[2];

	for (unsigned i=0; i<2; ++i)
        pointList[i] = s.pointList[i];

}

void Segment::setStart(const Point & p)
{
	pointList[0] = p;
}

void Segment::setEnd(const Point & p)
{
	pointList[1] = p;
}

/**
 * @brief Segment::setN : spécifier l'un des 2 points "de contrôle"
 * @param r
 * @param p
 */
void Segment::setN(unsigned r, const Point & p)
{
	if (r>1)
		r=1;
	pointList[r] = p;
}

/**
 * @brief Segment::getN
 * @param r
 * @return un des points "de contrôle" du segment, le 1er ou le 2e
 */
Point Segment::getN(unsigned r) const
{
	if (r>1)
		r=1;

	return pointList[r];
}

Point Segment::getStart() const
{
	return getN(0);
}


Point Segment::getEnd() const
{
	return getN(1);
}

int Segment::getStep() const
{
    return step;
}

Point Segment::getPointList(int index){
    return pointList[index];
}

float Segment::length() const
{
	float res=0.0f;

	for (unsigned i=0; i<3; ++i)
		res += pow((pointList[1]).getN(i) - (pointList[0]).getN(i), 2.0f);

	return sqrt(res);
}

Segment& Segment::operator= (const Segment &s)
{
	for (unsigned i=0; i<2; ++i)
		pointList[i] = s.pointList[i];

    return *this;
}

std::ostream& operator<<(std::ostream& out, const Segment& s)
{
	return out << s.pointList[0] << " -- " << s.pointList[1];
}

