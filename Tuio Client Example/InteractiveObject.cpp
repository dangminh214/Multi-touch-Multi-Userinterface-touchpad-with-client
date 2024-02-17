#include "InteractiveObject.h"

InteractiveObject::InteractiveObject()
{
    bottom_left_corner = new Point(-0.25, -0.25);
    bottom_right_corner = new Point(0.25, -0.25);
    top_right_corner = new Point(0.25, 0.25);  
    top_left_corner = new Point(-0.25, 0.25);

    height = 0.5; 
    width = 0.5;
}

bool InteractiveObject::isBeingTouchedBy(Point* point)
{
    if ((this->getSmallestX() <= point->getX() && point->getX() <= this->getBiggestX())
        && (this->getSmallestY() <= point->getY() && point->getY() <= this->getBiggestY()))
    {
        std::cout <<"INSIDE" << std::endl;
        return true;
    }
    else {
        std::cout <<"NOT INSIDE" << std::endl;
    }
    return false;
}

void InteractiveObject::translate(float trans_X, float trans_Y)
{
    this->bottom_left_corner->setX(this->bottom_left_corner->getX() + trans_X);
    this->bottom_left_corner->setY(this->bottom_left_corner->getY() + trans_Y);

    this->bottom_right_corner->setX(this->bottom_right_corner->getX() + trans_X);
    this->bottom_right_corner->setY(this->bottom_right_corner->getY() + trans_Y);

    this->top_right_corner->setX(this->top_right_corner->getX() + trans_X);
    this->top_right_corner->setY(this->top_right_corner->getY() + trans_Y);

    this->top_left_corner->setX(this->top_left_corner->getX() + trans_X);
    this->top_left_corner->setY(this->top_left_corner->getY() + trans_Y);
}

Point* InteractiveObject::calculatePointAfterRotate(float angleDelta, Point* point)
{
    float radianAngle = angleDelta * (M_PI / 180.0f);

    float cosAngle = cos(radianAngle);
    float sinAngle = sin(radianAngle);

    float res_x = cosAngle * point->getX() + (-sinAngle) * point->getY();
    float res_y = sinAngle * point->getX() + cosAngle * point->getY();

    Point* res = new Point(res_x, res_y);

    return res;
}

void InteractiveObject::rotate(float angleDelta)
{
    this->translate(this->getCenterPoint()->getX(), this->getCenterPoint()->getY());
    this->bottom_left_corner = this->calculatePointAfterRotate(angleDelta, bottom_left_corner);
    this->bottom_right_corner = this->calculatePointAfterRotate(angleDelta, bottom_right_corner); 

    this->top_right_corner = this->calculatePointAfterRotate(angleDelta, top_right_corner); 
    this->top_left_corner = this->calculatePointAfterRotate(angleDelta, top_left_corner);
    this->translate(-this->getCenterPoint()->getX(), -this->getCenterPoint()->getY());
}

/*
void InteractiveObject::scale(float scalar)
{
    std::cout << "Scalar: " << scalar << std::endl;
    std::cout << "before: " << this->width << ", " << this->height << std::endl;
    this->height = this->height * scalar;
    this->width = this->width * scalar;
    std::cout << "after: " << this->width << ", " << this->height << std::endl;

    float center_x = this->getCenterPoint()->getX(); 
    float center_y = this->getCenterPoint()->getY();

    this->bottom_right_corner->setX(this->bottom_left_corner->getX() + width);
    this->top_left_corner->setY(this->bottom_left_corner->getY() + height);
    this->top_right_corner->setY(this->top_left_corner->getY());
    this->top_right_corner->setX(this->bottom_right_corner->getX());   
}
*/

void InteractiveObject::scaleFromCenter(float scalar)
{
    std::cout << "Scalar: " << scalar << std::endl;
    std::cout << "before: " << this->width << ", " << this->height << std::endl;
    this->height = this->height * scalar;
    this->width = this->width * scalar;
    std::cout << "after: " << this->width << ", " << this->height << std::endl;

    this->bottom_left_corner->setX(this->getCenterPoint()->getX() - this->width / 2);
    this->bottom_left_corner->setY(this->getCenterPoint()->getY() - this->height / 2);

    this->bottom_right_corner->setX(this->getCenterPoint()->getX() + this->width / 2);
    this->bottom_right_corner->setY(this->getCenterPoint()->getY() - this->height / 2);

    this->top_left_corner->setX(this->getCenterPoint()->getX() - this->width / 2);
    this->top_left_corner->setY(this->getCenterPoint()->getY() + this->height / 2);

    this->top_right_corner->setX(this->getCenterPoint()->getX() + this->width / 2);
    this->top_right_corner->setY(this->getCenterPoint()->getY() + this->height / 2);
}

Point* InteractiveObject::getTopLeftCorner()
{
    return top_left_corner;
}

Point* InteractiveObject::getTopRightCorner()
{
    return top_right_corner;
}

Point* InteractiveObject::getBottomLeftCorner()
{
    return bottom_left_corner;
}

Point* InteractiveObject::getBottomRightCorner()
{
    return bottom_right_corner;
}

float InteractiveObject::getHeight()
{
    return this->height;
}

float InteractiveObject::getWidth()
{
    return this->width;
}

float InteractiveObject::getBiggestX()
{
    float xValues[] = {top_left_corner->getX(), top_right_corner->getX(), bottom_left_corner->getX(), bottom_right_corner->getX()};
    float max = xValues[0]; 
    for (unsigned int i = 1; i < 4; i++) {
        if (max < xValues[i]) {
            max = xValues[i];
        }
    }
    return max;
}

float InteractiveObject::getBiggestY()
{
    float yValues[] = { top_left_corner->getY(), top_right_corner->getY(), bottom_left_corner->getY(), bottom_right_corner->getY() };
    float max = yValues[0];
    for (unsigned int i = 1; i < 4; i++) {
        if (max < yValues[i]) {
            max = yValues[i];
        }
    }
    return max;
}

float InteractiveObject::getSmallestX()
{
    float xValues[] = { top_left_corner->getX(), top_right_corner->getX(), bottom_left_corner->getX(), bottom_right_corner->getX() };
    float min = xValues[0];
    for (unsigned int i = 1; i < 4; i++) {
        if (min > xValues[i]) {
            min = xValues[i];
        }
    }
    return min;
}

float InteractiveObject::getSmallestY()
{
    float yValues[] = { top_left_corner->getY(), top_right_corner->getY(), bottom_left_corner->getY(), bottom_right_corner->getY() };
    float min = yValues[0];
    for (unsigned int i = 1; i < 4; i++) {
        if (min > yValues[i]) {
            min = yValues[i];
        }
    }
    return min;
}

void InteractiveObject::outputInfo()
{
    std::cout << "bottom_left : " 
        << this->bottom_left_corner->getX() << " ," 
        << this->bottom_left_corner->getY() << std::endl;
    std::cout << "bottom_right : "
        << this->bottom_right_corner->getX() << " ,"
        << this->bottom_right_corner->getY() << std::endl;
    std::cout << "top_left : "
        << this->top_left_corner->getX() << " ,"
        << this->top_left_corner->getY() << std::endl;
    std::cout << "top_right : "
        << this->top_right_corner->getX() << " ,"
        << this->top_right_corner->getY() << std::endl;
}

Point* InteractiveObject::getCenterPoint()
{
    float center_x = (this->getBiggestX() + this->getSmallestX()) / 2;
    float center_y = (this->getBiggestY() + this->getSmallestY()) / 2;
    Point* center = new Point(center_x, center_y);
    return center;
}










