#pragma once
#include "TuioListener.h"
#include "InteractiveObject.h"
#include "FingerTouch.h"
#include <vector>
#include "$1/GeometricRecognizer.h"

class MyClient : public TUIO::TuioListener 
{
public: 
	void MyClient::addTuioObject(TUIO::TuioObject* tobj) {};
	void MyClient::updateTuioObject(TUIO::TuioObject* tobj) {};
	void MyClient::removeTuioObject(TUIO::TuioObject* tobj) {};

	void MyClient::addTuioCursor(TUIO::TuioCursor* tcur) override;
	void MyClient::updateTuioCursor(TUIO::TuioCursor* tcur) override;
	void MyClient::removeTuioCursor(TUIO::TuioCursor* tcur) override;

	void  MyClient::refresh(TUIO::TuioTime frameTime) {};

	void MyClient::recognizeTranslation(FingerTouch* fingerTouch);

	void MyClient::recognizeRotation(FingerTouch* fingerTouch); 

	void MyClient::recognizeScaling(FingerTouch* fingerTouch);

	InteractiveObject* interactiveObject = new InteractiveObject();

	void draw(InteractiveObject* interactiveObject);


private: 
	std::vector<FingerTouch*> touches;

	bool isRecordingMotion = false;	

	bool turnsClockwise(Point* before, Point* after);

	bool hasAtLeastOneHoldingPoint();

	float distance(Point* a, Point* b);

	Point* getHoldingPoint();

	DollarRecognizer::GeometricRecognizer* geometric
		= new DollarRecognizer::GeometricRecognizer();
};