

#include "stdafx.h"
#include "GRA.h"

int main(int argc, char* argv[])
{
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(400, 300);
	glutCreateWindow("Gra");
	glutFullScreen();

	// 
	glutDisplayFunc(OnRender);
	glutReshapeFunc(OnReshape);

	// Timer
	glutTimerFunc(17, OnTimer, 0); // wywoluje sie co 17 ms
	glEnable(GL_DEPTH_TEST); // test glebokosci

	// klawiatura
	glutKeyboardFunc(OnKeyPress);
	glutKeyboardUpFunc(OnKeyUp);
	// myszka
	glutPassiveMotionFunc(OnMouseMove);
	glutMotionFunc(OnMouseMove);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutMouseFunc(OnMouseButton);

	// sciany boczne
	sciany.push_back(new wall(0, rozmiar/2, rozmiar/2, 0.01, rozmiar, rozmiar, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01));
	sciany.push_back(new wall(rozmiar, rozmiar/2, rozmiar/2, 0.01, rozmiar, rozmiar, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01));
	// sufit i podloga
	sciany.push_back(new wall(rozmiar/2, 0, rozmiar/2, rozmiar, 0.01, rozmiar, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01));
	sciany.push_back(new wall(rozmiar/2, rozmiar/5, rozmiar/2, rozmiar, 0.01, rozmiar, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01));
	// przednia tylnia
	sciany.push_back(new wall(rozmiar/2, rozmiar/2, 0, rozmiar, rozmiar, 0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01));
	sciany.push_back(new wall(rozmiar/2, rozmiar/2, rozmiar, rozmiar, rozmiar, 0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01));
		
	for (int i = 0; i < ilosc_celow; i++)
		for (int j = 0; j < 3; j++)
			cell.push_back(new target(j));

	player.speed = .3;

	player.pos.x = rozmiar / 2;
	player.pos.y = 1.0f;
	player.pos.z = rozmiar / 2;

	player.dir.x = 0.0f;
	player.dir.y = 0.0f;
	player.dir.z = -1.0f;

	glutMainLoop();

}