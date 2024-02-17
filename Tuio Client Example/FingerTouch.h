#pragma once
#include "InteractiveObject.h"
#include <vector>
#include "Point.h"
class FingerTouch
{
public: 
	FingerTouch(int id, float x, float y);
	int getID();

	void updateCoords(Point* new_coord);
	void setCoords(float p_x, float p_y);

	std::vector<Point*> getPath();

	Point* getCoord();

	bool isHolding;
	bool isMoving;
private: 
	int id;
	std::vector<Point*> path;
	Point* coord;
};

