#include "FingerTouch.h"

FingerTouch::FingerTouch(int id, float x, float y)
{
    this->id = id;
    this->coord = new Point(x, y);

    this->isHolding = true;
    this->isMoving = false;

    this->path.push_back(this->coord);
}

int FingerTouch::getID()
{
    return this->id;
}

void FingerTouch::updateCoords(Point* new_coord)
{
    this->path.push_back(new_coord);
    this->setCoords(new_coord->getX(), new_coord->getY());
    this->isHolding = false;
    this->isMoving = true;
}

void FingerTouch::setCoords(float p_x, float p_y)
{
    this->coord->setX(p_x); 
    this->coord->setY(p_y);
}

std::vector<Point*> FingerTouch::getPath()
{
    return this->path;
}

Point* FingerTouch::getCoord()
{
    return this->coord;
}
