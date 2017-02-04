// proba gry.cpp : Defines the entry point for the console application.
//

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

	// sciany boczne
	sciany.push_back(new wall(0, rozmiar/2, rozmiar/2, 0.01, rozmiar, rozmiar, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01));
	sciany.push_back(new wall(rozmiar, rozmiar/2, rozmiar/2, 0.01, rozmiar, rozmiar, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01));
	// sufit i podloga
	sciany.push_back(new wall(rozmiar/2, 0, rozmiar/2, rozmiar, 0.01, rozmiar, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01));
	sciany.push_back(new wall(rozmiar/2, rozmiar/10, rozmiar/2, rozmiar, 0.01, rozmiar, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01));
	// przednia tylnia
	sciany.push_back(new wall(rozmiar/2, rozmiar/2, 0, rozmiar, rozmiar, 0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01));
	sciany.push_back(new wall(rozmiar/2, rozmiar/2, rozmiar, rozmiar, rozmiar, 0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01));

	// losowe miejsca kolumn
	for (; ilosc_kolumn > 0;ilosc_kolumn--)
	{
		// zeby sie nie nakladaly:
		int blad;
		int x;
		int z;
		do {
			blad = 0;
			x = rand() % rozmiar;
			z = rand() % rozmiar;
			for (int i = 0; i < sciany.size(); i++)
			{
				if ((sciany[i]->pos.x == x && sciany[i]->pos.z == z) || rozmiar / 2 == x || rozmiar / 2 == z)
					blad = 1;
			}
		} while (blad == 1);
		sciany.push_back(new wall(x, rozmiar / 2, z, 1, rozmiar, 1, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01, ((float)(rand() % 100))*0.01));
	}
	
	// tekstura gwiazdek
	gwiazda_bitmap = LoadTexture("Resources\\checkboard.bmp", GL_NEAREST, GL_NEAREST);

	for (int i = 0; i<ilosc_gwiazdek; i++)
	{
		vector3d temp;
		gwiazdki.push_back(temp);
		int blad;
		int x;
		int z;
		do {
			blad = 0;
			x = rand() % rozmiar;
			z = rand() % rozmiar;
		for (int i = 0; i < sciany.size(); i++) // gwiazka nie stworzy sie na kolumnie ani gwiazdce
		{
			if ((sciany[i]->pos.x == x && sciany[i]->pos.z == z) || rozmiar / 2 == x || rozmiar / 2 == z)
				blad = 1;
		}
		for (int i = 0; i < gwiazdki.size(); i++) // gwiazka nie stworzy sie na kolumnie ani gwiazdce
		{
			if (gwiazdki[i].x == x && gwiazdki[i].z == z)
				blad = 1;
		}

		} while (blad == 1);
		gwiazdki[i].x = x;
		gwiazdki[i].y = 1;
		gwiazdki[i].z = z;
	}

	// Inicjalizacja kamery:
	player.pos.x = rozmiar/2;
	player.pos.y = .5f;
	player.pos.z = rozmiar / 2;
	player.dir.x = 0.0f;
	player.dir.y = 0.0f;
	player.dir.z = -1.0f;

	player.speed = .2;

	// ustawienie swiatla
	// Ustawiamy komponent ambient naszej sceny - wartosc niezalezna od swiatla (warto zresetowac)
	float gl_amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gl_amb);
	glEnable(GL_LIGHTING); // Wlaczenie oswietlenia
	glShadeModel(GL_SMOOTH); // Wybor techniki cieniowania
	glEnable(GL_LIGHT0); // Wlaczenie 0-go zrodla swiatla
	glEnable(GL_NORMALIZE);

	glutMainLoop();

}

