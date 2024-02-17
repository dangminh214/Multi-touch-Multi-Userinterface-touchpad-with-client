#pragma once
#include <iostream>
class Point
{
public: 
	Point();
	Point(const Point& otherPoint) : x(otherPoint.x), y(otherPoint.y) {};
	Point(float x, float y);
	float getX();
	float getY();
	void setX(float p_x); 
	void setY(float p_y);

	void outputPointInfo();

private: 
	float x, y;
};

