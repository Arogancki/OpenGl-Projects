// proba gry.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GRA.h"
#include "Labyrinth.h"

int main(int argc, char* argv[])
{
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(400, 300);
	glutCreateWindow("Gra Zutowy Labirynt");
	glutFullScreen(); // robi full screen

	glutSetCursor(GLUT_CURSOR_NONE); // ukrycie kursora

	// zmiana rozmiaru okna
	glutDisplayFunc(OnRender); 
	glutReshapeFunc(OnReshape);

	// Timer
	glutTimerFunc(17, OnTimer, 0); // wywoluje sie co 17 ms
	glEnable(GL_DEPTH_TEST); // test glebokosci

	// klawiatura
	glutKeyboardFunc(OnKeyPress);
	glutKeyboardUpFunc(OnKeyUp);

	// generacja dynamiczna, randomowa labiryntu labiryntu
	Labyrinth Map(rozmiar/ skalaPokoju, rozmiar/ skalaPokoju);
	// wrzucenie przedmiotow do labirytntu
	int *lokalizacje = Map.setItemLocation(pointsMax);
	vector3d temp = { 0,0,0 };
	for (int i = 0; i < pointsMax; i++)
	{
		temp = { (float)lokalizacje[i] * skalaPokoju,rozmiarZnajdzki,(float)lokalizacje[i+1] * skalaPokoju };
		znajdzki.push_back(temp);
	}
	//clear
	delete[] lokalizacje;

	//podloga i sciagniecie lokalizacji scian z labiryntu
	sciany.push_back(new wall(rozmiar / 2, 0, rozmiar / 2, rozmiar, 0.01, rozmiar, 1, 1, 1));
	lokalizacje = Map.GiveWallLocation();
	for (int i=0; lokalizacje[i]!=-1; i+=2)
	{
		sciany.push_back(new wall(lokalizacje[i]*skalaPokoju, rozmiarZnajdzki, lokalizacje[i+1]*skalaPokoju, skalaPokoju, 3, skalaPokoju, 0, 0, 0));
	}
	delete[] lokalizacje;

	//ustawienie startu z labirntu
	startX = Map.getPoint(0, 5)*skalaPokoju;
	startY = Map.getPoint(1, 5)*skalaPokoju;
	
	// Inicjalizacja kamery:
	player.pos.x = startX;
	player.pos.y = rozmiarZnajdzki;
	player.pos.z = startY;
	player.dir.x = 0.0f;
	player.dir.y = 0.0f;
	player.dir.z = -1.0f;

	player.speed = szybkosc;

	// ladowanie bitmap
	znajdzki_bitmap = LoadTexture("Resources\\checkboard.bmp", GL_NEAREST, GL_NEAREST);
	wall_bitmap = LoadTexture("Resources\\floor.bmp", GL_NEAREST, GL_NEAREST);

	// stworznie displaylist
	StworzListeZnajdzki();
	StworzListeSciany();

	// ustawienie swiatla
	// Ustawiamy komponent ambient naszej sceny - wartosc niezalezna od swiatla (warto zresetowac)
	float gl_amb[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gl_amb);
	glEnable(GL_LIGHTING); // Wlaczenie oswietlenia
	glShadeModel(GL_SMOOTH); // Wybor techniki cieniowania
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHT0); // Wlaczenie 0-go zrodla swiatla - gracaza
	glEnable(GL_LIGHT0+1); // Wlaczenie 1-go zrodla swiatla - najblizszej znajdzki

	glutMainLoop();
	// cleary
	if (!sciany.empty())
		sciany.clear();
	if (!znajdzki.empty())
		znajdzki.clear();
}

