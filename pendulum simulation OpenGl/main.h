#pragma once
#include "stdafx.h"
#include "Pendulum.h"
#include "PendulumOnPendulum.h"
#include <thread>

#define frame 10

int timeInterval = (1000.0/frame);

Pendulum *first2;
PendulumOnPendulum *first;

void OnReshape(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, width, height);
	gluPerspective(60.0f, (float)width / height, .01f, 400.0f);
}

void OnRender();

void Start()
{
	first2=new Pendulum(3, 90, 1, 10,0.01);
	//first2=new Pendulum("P.txt");

	//first = new PendulumOnPendulum(1, 2, 80, 150, 2, 1, 1, 2, 0.01);
	first = new PendulumOnPendulum("PoP.txt",1,2,1,2);

	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnReshape);

	glEnable(GL_DEPTH_TEST); // test glebokosci
	glutMainLoop();
}

double SecfromStart = 0;
void OnRender()
{
	SecfromStart= glutGet(GLUT_ELAPSED_TIME);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(
		0,0,0, // Pozycja kamery
		0,0,1, // Punkt na ktory patrzy kamera (pozycja + kierunek)
		0.0f, 1.0f, 0.0f // Wektor wyznaczajacy pion
		);

	glPushMatrix();

		glTranslatef(0, 0, 10);
		//tlo
		glPushMatrix();
			glTranslatef(0, 0, 1);
			glColor3f(0.8f, 0.8f, 0.8f);
			glScalef(20, 20, 0.1);
			glutSolidCube(1);
		glPopMatrix();

		//first->Draw();
		first2->Draw();

	glPopMatrix();


		//Sleep(SecfromStart + timeInterval - (glutGet(GLUT_ELAPSED_TIME)));
	int x = SecfromStart + timeInterval - glutGet(GLUT_ELAPSED_TIME);
	std::this_thread::sleep_for(std::chrono::milliseconds(x));
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}
