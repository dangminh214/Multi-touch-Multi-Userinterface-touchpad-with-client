#include <iostream>
#include <process.h>

#include "TuioClient.h"
#include "TuioListener.h"

#include <GL/glut.h>

#include <vector>

#include "MyClient.h"

using namespace TUIO;

double width = 752;
double height = 480;

TUIO::TuioClient* tuioClient;

MyClient* myClient = new MyClient();

void draw()
{
	char id[5];

	glClear(GL_COLOR_BUFFER_BIT); 

	myClient->draw(myClient->interactiveObject);
	
	glLineWidth(5.0);

	std::list<TuioCursor*> cursorList;

	// get the actual cursor list from the tuioClient
	tuioClient->lockCursorList();
	cursorList = tuioClient->getTuioCursors();

	for (std::list<TuioCursor*>::iterator cursorListIter = cursorList.begin(); cursorListIter != cursorList.end(); ++cursorListIter)
	{
		TuioCursor* tuioCursor = (*cursorListIter);

		if (tuioCursor->getCursorID() < 0 || tuioCursor == nullptr) {
			std::cout << "Fehler" << std::endl;
		}
		else {
			std::list<TuioPoint> path = tuioCursor->getPath();
			if (!path.empty()) {
				glBegin(GL_LINE_STRIP);
				glColor3f(0.0, 1.0, 0.0);	//Color declare: green

				double x = 0, y = 0;

				for (std::list<TUIO::TuioPoint>::iterator pathIter = path.begin(); pathIter != path.end(); ++pathIter)
				{
					TUIO::TuioPoint point = *pathIter;
					x = (point.getX() - 0.5) * 2.0;
					y = (-point.getY() + 0.5) * 2.0;
					glVertex2d(x, y);
				}
				glEnd();

				//Draw the ID
				glColor3f(0.0, 1.0, 1.0);
				glRasterPos2f(x, y);
				if (tuioCursor->getCursorID() < 0) {
					std::cout << "Fehler" << std::endl;
				}
				else {
					sprintf(id, "%d", tuioCursor->getSessionID());
				}

				glPushMatrix();
				glScalef(2.0, 2.0, 1.0); // Scale up the text size
				for (size_t i = 0; i < strlen(id); i++) {
					glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, id[i]);
				}
				glPopMatrix();

				glEnd();
			}
			else {
				std::cout << "Idle" << std::endl;
			}
		}
	}
	tuioClient->unlockCursorList();

	glutSwapBuffers();
}

void tuioThread(void*)
{
	tuioClient = new TUIO::TuioClient();
	tuioClient->addTuioListener(myClient);
	tuioClient->connect(true);
}

void idle(void)
{
	// this might be needed on some systems, otherwise the draw() function will only be called once
	glutPostRedisplay();
}

void glInit()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

int main(int argc, char** argv)
{
	std::cout << "NEW CLIENT STARTS !!! " << std::endl;
	// create a second thread for the TUIO listener
	HANDLE hThread_TUIO;
	unsigned threadID;

	//hThread = (HANDLE)_beginthreadex( NULL, 0, &tuioThread, NULL, 0, &threadID );
	hThread_TUIO = (HANDLE)_beginthread(tuioThread, 0, NULL);

	
	// GLUT Window Initialization (just an example):
	glutInit(&argc, argv);
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("My TUIO Client (using GLUT)");

	// openGL init
	glInit();

	// Register callbacks:
	glutDisplayFunc(draw);
	glutIdleFunc(idle);
	glutMainLoop();
	std::cout << ".............................................." << std::endl;

	return 0;
}