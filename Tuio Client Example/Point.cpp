#include "Point.h"

Point::Point()
{
	this->x = 0; 
	this->y = 0;
}


Point::Point(float x, float y)
{
	this->x = x; 
	this->y = y;
}

float Point::getX()
{
	return this->x;
}

float Point::getY()
{
	return this->y;
}

void Point::setX(float p_x)
{
	this->x = p_x;
}

void Point::setY(float p_y)
{
	this->y = p_y;
}

void Point::outputPointInfo()
{
	std::cout << "Point( " << this->x << ", "<< this->y << " )" << std::endl;
}
