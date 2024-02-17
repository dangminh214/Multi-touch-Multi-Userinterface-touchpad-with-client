#include "MyClient.h"

void MyClient::addTuioCursor(TUIO::TuioCursor* tcur)
{
		float scaled_x = (tcur->getX()-0.5) * 2;
		float scaled_y = (-tcur->getY() + 0.5) * 2;
		std::cout << "new finger detected: (id=" << tcur->getSessionID() << ", coordinates=" << scaled_x << "," << scaled_y << ")\n";

		FingerTouch* fingerTouch = new FingerTouch(tcur->getSessionID(), scaled_x, scaled_y);
		touches.push_back(fingerTouch);

		//Click inside the object or one finger is holding the object
		if (this->interactiveObject->isBeingTouchedBy(fingerTouch->getCoord()) || this->interactiveObject->isBeingTouchedBy(this->getHoldingPoint())) 
		{
			this->isRecordingMotion = true;
		}
		else 
		{
			this->isRecordingMotion = false;
		}
}

void MyClient::updateTuioCursor(TUIO::TuioCursor* tcur)
{
	float scaled_x = (tcur->getX() - 0.5) * 2;
	float scaled_y = (-tcur->getY() + 0.5) * 2;

	std::cout << "finger updated: (id=" << tcur->getSessionID() << ", coordinates=" << scaled_x << "," << scaled_y << ")\n";
	std::cout << "Number of actived cursor: " << this->touches.size() << std::endl;
	for (auto* finger : touches) {
		std::cout << "ID: " << finger->getID() << " | ";
	}
	for (auto* finger : touches) {
		if (finger->getID() == tcur->getSessionID()) {
			Point* newCoord = new Point(scaled_x, scaled_y);
			finger->updateCoords(newCoord);
			this->recognizeTranslation(finger);
			if (this->hasAtLeastOneHoldingPoint() 
				&& this->interactiveObject->isBeingTouchedBy(this->getHoldingPoint())
				&& !this->interactiveObject->isBeingTouchedBy(finger->getCoord())
				) 
			{
				this->recognizeRotation(finger);
			}

			if (this->interactiveObject->isBeingTouchedBy(this->getHoldingPoint())
				&& this->interactiveObject->isBeingTouchedBy(finger->getCoord())) 
			{
				this->recognizeScaling(finger);
			}
			this->interactiveObject->outputInfo();
			break;
		}
	}
}

void MyClient::removeTuioCursor(TUIO::TuioCursor* tcur)
{
	float scaled_x = (tcur->getX() - 0.5) * 2;
	float scaled_y = (-tcur->getY() + 0.5) * 2;

	std::cout << "finger removed: (id=" << tcur->getSessionID() << ", coordinates=" << scaled_x << "," << scaled_y << ")\n";

	for (int i = 0; i < touches.size(); i++) {
		if (touches.at(i)->getID() == tcur->getSessionID()) {
			touches.erase(touches.begin() + i);
			return;
		}
	}
	this->isRecordingMotion = false;
}
void MyClient::recognizeTranslation(FingerTouch* fingerTouch)
{
	if (isRecordingMotion && fingerTouch->getPath().size() >=3) 
	{
		int lastPos = fingerTouch->getPath().size() - 1;
		Point* latestPos = fingerTouch->getPath().at(lastPos-1);
		if (this->interactiveObject->isBeingTouchedBy(latestPos))
		{
			float trans_x = fingerTouch->getCoord()->getX() - latestPos->getX();
			float trans_y = fingerTouch->getCoord()->getY() - latestPos->getY();
			Point* transVector = new Point(trans_x, trans_y);

			this->interactiveObject->translate(trans_x, trans_y);
			std::cout << "Translate: " << transVector->getX() << ", " << transVector->getY() << std::endl;
		}	
	}
}

bool MyClient::hasAtLeastOneHoldingPoint()
{
	if (touches.size() >= 2) {
		for (auto& touch : touches) {
			if (touch->isHolding && touch->getID() >= 0 && touch->getPath().size() > 0) {
				return true;
			}
		}
	}
	
	return false;
}

Point* MyClient::getHoldingPoint()
{
	for (auto& touch : touches) {
		if (touch->isHolding && touch->getID() >= 0 && touch->getPath().size() > 0) {
			return touch->getCoord();
		}
	}
}

void MyClient::recognizeRotation(FingerTouch* fingerTouch)
{
	if (isRecordingMotion && fingerTouch->getPath().size() > 1) {
			int lastPos = fingerTouch->getPath().size() - 1;
			Point* movingFingerLastPos = fingerTouch->getPath().at(lastPos-1);
			Point* movingFingerCurrentPos = fingerTouch->getCoord();

			// Angle, which created by two vector a(x1,y1) and b(x2,y2)			
			//->VectorA (holdingPoint, movingFingerLastPos)
			//->VectorB (holdingPoint, movingFingerCurrentPos)
			//cos(alpha) = (a.b)/(abs(a).abs(b)) => alpha = arc cos(alpha);
			
			Point* VectorA = new Point(
				movingFingerLastPos->getX() - this->getHoldingPoint()->getX(),
				movingFingerLastPos->getY() - this->getHoldingPoint()->getY()
			);
			Point* VectorB = new Point(
				movingFingerCurrentPos->getX() - this->getHoldingPoint()->getX(),
				movingFingerCurrentPos->getY() - this->getHoldingPoint()->getY()
			);
			float opOne = VectorA->getX() * VectorB->getX() + VectorA->getY() * VectorB->getY();
			float opTwoLeft = sqrt(pow(VectorA->getX(), 2) + pow(VectorA->getY(), 2));
			float opTwoRight = sqrt(pow(VectorB->getX(), 2) + pow(VectorB->getY(), 2));

			float opTwo = opTwoLeft * opTwoRight;
			float cos = opOne / opTwo;

			float angle = acos(cos);

			if (turnsClockwise(movingFingerLastPos, movingFingerCurrentPos)) angle = 180 - angle;

			//idk sometimes the result of the angle is NOT A NUMBER aka NAN
			if (!std::isnan(angle)) {
				this->interactiveObject->rotate(angle * 10.0);
			}
			
			std::cout << "rotate: " << angle << std::endl;
	}
}


bool MyClient::turnsClockwise(Point* before, Point* after)
{
	if (before->getX() >= 0 && after->getX() >= 0) {
		if (after->getY() < before->getY()) {
			return true;
		}
	}
	if (before->getX() <= 0 && after->getX() <= 0) {
		if (after->getY() > before->getY()) {
			return true;
		}
	}
	return false;
}

void MyClient::recognizeScaling(FingerTouch* fingerTouch)
{
	if (isRecordingMotion && fingerTouch->getPath().size() > 1) {
		int lastPos = fingerTouch->getPath().size() - 1;
		Point* movingFingerLastPos = fingerTouch->getPath().at(lastPos - 1); //touchBefore
		for (auto& currentTouch : this->touches) {
			if (fingerTouch->getID() == currentTouch->getID()) continue;

				float distanceBefore = this->distance(currentTouch->getCoord(), movingFingerLastPos);
				float distanceNow = this->distance(currentTouch->getCoord(), fingerTouch->getCoord());

				float scalar = (distanceNow / distanceBefore);

				if (!std::isnan(scalar) && scalar!= 0) {
					this->interactiveObject->scaleFromCenter(scalar);
					std::cout << "Scaling: "; 
					break;
				}	
		}
	}
}

void MyClient::draw(InteractiveObject* interactiveObject)
{
	glPushMatrix();
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	
	glVertex2f(this->interactiveObject->getBottomLeftCorner()->getX(), this->interactiveObject->getBottomLeftCorner()->getY());
	glVertex2f(this->interactiveObject->getBottomRightCorner()->getX(), this->interactiveObject->getBottomRightCorner()->getY());
	glVertex2f(this->interactiveObject->getTopRightCorner()->getX(), this->interactiveObject->getTopRightCorner()->getY());
	glVertex2f(this->interactiveObject->getTopLeftCorner()->getX(), this->interactiveObject->getTopLeftCorner()->getY());
	
	glEnd();
	glPopMatrix();
}

float MyClient::distance(Point* a, Point* b)
{
	float op1 = pow((b->getX() - a->getX()), 2);
	float op2 = pow((b->getY() - a->getY()), 2);
	float dist = sqrt(op1 + op2);
	return dist;
}

