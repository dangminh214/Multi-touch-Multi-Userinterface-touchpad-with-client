#pragma once
#define _USE_MATH_DEFINES
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include "Point.h"

class InteractiveObject
{
public: 

	InteractiveObject();

	bool isBeingTouchedBy(Point* point);

	void translate(float trans_X, float trans_Y);

	void rotate(float angleDelta);

	//void scale(float scalar);

	void scaleFromCenter(float scalar);

	Point* getTopLeftCorner(); 
	Point* getTopRightCorner(); 
	Point* getBottomLeftCorner(); 
	Point* getBottomRightCorner();

	float getHeight(); 
	float getWidth();
	
	float getBiggestX(); 
	float getBiggestY();

	float getSmallestX(); 
	float getSmallestY(); 

	void outputInfo();

	Point* getCenterPoint();

private: 
	Point* calculatePointAfterRotate(float angleDelta, Point* point);

	Point* top_left_corner;
	Point* top_right_corner;
	Point* bottom_left_corner; 
	Point* bottom_right_corner;

	float height; 
	float width; 
};

