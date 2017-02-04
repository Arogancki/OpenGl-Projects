#pragma once
#include "stdafx.h"

int rozmiar = 100; // rozmiar mapy
bool blokada = 0; // blokada  przy poruszaniu na koniec
int czas = 30000; // czas gry =30 sek
int points = 0;
int mouseX = 0;
int mouseY = 0;
float mouseSensitivity = 1.655f;
int ilosc_celow = 20; // ilosc kwadratow do szczeclania
int czas_ostatniego_strzelenia = 0;
int odstep_pomiedzy_strzalami = 300;

// wektor 3d
struct vector3d
{
	float x, y, z;
};
//  kamery
struct SCameraState
{
	vector3d pos; // pozycja kamery
	vector3d dir; // kierunek patrzenia (wektor jednostkowy)
	float speed; // mno¿nik zmian pozycji - "szybkoœæ" ruchów kamery
	float velRX = 0.0f;
	float velRY = 0.0f;
};

class ammo
{
public:
	vector3d pos;
	vector3d dir;
	float szybkosc = 0.3;
	ammo() { pos.y = -1; }
	~ammo() {}
	void rysuj();
	bool czy_kolizja();
};
ammo bullet;

class target
{
public:
	vector3d pos;
	int score;
	int start;
	target(int);
	~target() {}
	void rysuj();
};
vector <target*> cell;
target::target(int i)
{
	pos.x = (rand() % (rozmiar-3))+2;
	pos.z = (rand() % (rozmiar-3))+2;
	pos.y = (rand() % ((rozmiar/5))-4)+4;
	if (i>3 || i<1)
		i = (rand() % 3) + 1;
	score = i;

	switch (score)
	{
	case 1:
		start = glutGet(GLUT_ELAPSED_TIME)+4000;
		break;
	case 2:
		start = glutGet(GLUT_ELAPSED_TIME)+3000;
		break;
	case 3:
		start = glutGet(GLUT_ELAPSED_TIME)+2000;
		break;
	}
}

void target::rysuj()
{
	glPushMatrix();
	switch(score)
	{
	case 1:
		glColor3f(1,0,0);
		break;
	case 2:
		glColor3f(0, 1, 0);
		break;
	case 3:
		glColor3f(0, 0, 1);
		break;
	}
	glTranslatef(pos.x, pos.y, pos.z);
	glScalef(2, 2, 2);
	glutSolidCube(1);
	glPopMatrix();
	if (glutGet(GLUT_ELAPSED_TIME) > start)
	{
		switch (score)
		{
		case 1:
			start = glutGet(GLUT_ELAPSED_TIME) + 4000;
			break;
		case 2:
			start = glutGet(GLUT_ELAPSED_TIME) + 3000;
			break;
		case 3:
			start = glutGet(GLUT_ELAPSED_TIME) + 2000;
			break;
		}
		pos.x = (rand() % (rozmiar - 3) )+ 2;
		pos.z = (rand() % (rozmiar - 3) )+ 2;
		pos.y = (rand() % ((rozmiar / 5) )- 3) + 3;
	}
}

bool czy_kolizja_sciana(int, int);

bool keystate[256]; // zmienne okreslajace wcisnietosc klawiszy
SCameraState player;

void drawText(int x, int y, std::string st, float q, float w, float e)
{
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);  // Tell opengl that we are doing project matrix work
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();  // Clear the matrix
	glOrtho(-128.0, 128.0, -128.0, 128.0, 0.0, 30.0);  // Setup an Ortho view
	glMatrixMode(GL_MODELVIEW);  // Tell opengl that we are doing model matrix work. (drawing)
	glLoadIdentity(); // Clear the model matrix
	glPushMatrix();
	glColor3f(q, w, e);
	glLoadIdentity();
	glRasterPos2i(x, y); // location to start printing text
	for (int i = 0; i < st.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, st[i]);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);  // Tell opengl that we are doing project matrix work
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void OnTimer(int id) {
	// chodzenie - kolizje
	if (blokada)
		return;

		if (keystate['I'] || keystate['i'])
			mouseY -= 5;
		if (keystate['k'] || keystate['K'])
			mouseY += 5;
		if (keystate['L'] || keystate['l'])
			mouseX += 5;
		if (keystate['j'] || keystate['J'])
			mouseX -= 5;

	if (keystate['w'] || keystate['W']) 
	{
		float x= player.pos.x + (player.dir.x * player.speed);
		float z = player.pos.z + (player.dir.z * player.speed);
		if (!czy_kolizja_sciana(x,z))
		{
			player.pos.x = x;
			player.pos.z = z;
		}
	}
	if (keystate['s'] || keystate['S']) {
		float x = player.pos.x - (player.dir.x * player.speed);
		float z = player.pos.z - (player.dir.z * player.speed);
		if (!czy_kolizja_sciana(x, z))
		{
			player.pos.x = x;
			player.pos.z = z;
		}
	}
	if (keystate['d'] || keystate['D']) {
			float x = player.pos.x + (-player.dir.z*player.speed);
			float z = player.pos.z + (player.dir.x * player.speed);
			if (!czy_kolizja_sciana(x, z))
			{
				player.pos.x = x;
				player.pos.z = z;
			}
	}
	if (keystate['a'] || keystate['A']) {
		float x = player.pos.x + (player.dir.z*player.speed);
		float z = player.pos.z + (-player.dir.x * player.speed);
		if (!czy_kolizja_sciana(x, z))
		{
			player.pos.x = x;
			player.pos.z = z;
		}
	}

	if (keystate[' '] && czas_ostatniego_strzelenia<glutGet(GLUT_ELAPSED_TIME)) {
		bullet.pos = player.pos;
		bullet.dir = player.dir;
		bullet.pos.y -= 0.2;
		czas_ostatniego_strzelenia = glutGet(GLUT_ELAPSED_TIME) + odstep_pomiedzy_strzalami;
	}
	
glutTimerFunc(18, OnTimer, 0);
}

void OnReshape(int width, int height) {
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glViewport(0, 0, width, height);
gluPerspective(60.0f, (float)width / height, .01f, 400.0f);
}

// Obs³uga naszego w³asnego zdarzenia klawisza
void OnKeyDown(unsigned char key, int x, int y) {

if (key == 27)  // ESC - wyjœcie do menu
	glutLeaveMainLoop();

}

// KLAWIATURA- Obs³uga wcisniecia puszczenia klawisza.
void OnKeyPress(unsigned char key, int x, int y) {

	if (!keystate[key]) {
		OnKeyDown(key, x, y); // Emulacja zdarzenia zwi¹zanego z pojedynczym wciœniêciem klawisza
	}
	keystate[key] = true;
}

// Obs³uga zdarzenia puszczenia klawisza.
void OnKeyUp(unsigned char key, int x, int y) {
keystate[key] = false;
if (key<224)
keystate[key + 32] = false;
if (key>31)
keystate[key - 32] = false;// ODPRONE NA PUSZCZENIE clk
}

void OnMouseMove(int x, int y) {
	mouseX = x;
	mouseY = y;
}

void OnMouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && czas_ostatniego_strzelenia<glutGet(GLUT_ELAPSED_TIME) && !blokada)
	{
		bullet.pos = player.pos;
		bullet.dir = player.dir;
		bullet.pos.y -= 0.2;
		czas_ostatniego_strzelenia = glutGet(GLUT_ELAPSED_TIME)+odstep_pomiedzy_strzalami;
	}
}

// sciany
class wall
{
public:
	vector3d pos;
	vector3d size;
	vector3d color;
	wall(float, float, float, float, float, float, float, float , float);
	~wall() {}
	void rysuj();
};
vector <wall*> sciany;
wall::wall(float px, float py, float pz, float sx, float sy, float sz, float red, float green, float blue)
{
	pos.x = px;
	pos.y = py;
	pos.z = pz;

	size.x = sx;
	size.y = sy;
	size.z = sz;

	color.x = red;
	color.y=green;
	color.z = blue;
}

void wall::rysuj()
{
	glPushMatrix();
	glColor3f(color.x, color.y, color.z);
	glTranslatef(pos.x, pos.y, pos.z);
	glScalef(size.x, size.y, size.z);
	glutSolidCube(1);
	glPopMatrix();
}

bool czy_kolizja_sciana(int x, int z) 
{
	if (x<=0 || z<=0 || x>=rozmiar || z>=rozmiar) // sciany pokoju
		return 1;
	return 0;
}

void ammo::rysuj()
{
	if (czy_kolizja())
		return;
	glPushMatrix();
	glColor3f(0,0,0);
	glTranslatef(pos.x, pos.y, pos.z);
	glutSolidSphere(0.1, 12, 12);
	glPopMatrix();
	pos.x += (dir.x * szybkosc);
	pos.y += (dir.y * szybkosc);
	pos.z += (dir.z * szybkosc);
}

bool ammo::czy_kolizja()
{
	for (int i = 0; i < cell.size(); i++) // kolumny
	{
		if (abs(cell[i]->pos.x - pos.x) < 1 && abs(cell[i]->pos.z - pos.z) < 1 && abs(cell[i]->pos.y - pos.y) < 1)
		{
			pos.y = -10000;
			dir.y = -1;
			points += cell[i]->score * 10;
			cell[i]->start = 0;
			return 1;
		}
	}
	return 0;
}

float px=-128; // animacja na koncu
void OnRender()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		player.pos.x, player.pos.y, player.pos.z, // Pozycja kamery
		player.pos.x + player.dir.x, player.pos.y + player.dir.y, player.pos.z + player.dir.z, // Punkt na ktory patrzy kamera (pozycja + kierunek)
		0.0f, 1.0f, 0.0f // Wektor wyznaczajacy pion
		);

	for (int i = 0; i < sciany.size(); i++)
		sciany[i]->rysuj();
	for (int i = 0; i < cell.size(); i++)
		cell[i]->rysuj();
	bullet.rysuj();

	drawText(-2, -5, "()", 0, 0, 0); // celownik

	if (glutGet(GLUT_ELAPSED_TIME)>czas)
	{
		blokada = 1;
		drawText(px, (px+28)*-1, "Koniec czasu ", 0, 0, 0);
		drawText(px, (px+20)*-1, "Punkty = " + to_string(points), 0, 0, 0);
		if (glutGet(GLUT_ELAPSED_TIME) > czas+5000)
			glutLeaveMainLoop();
		if (px<0)
			px+=0.1;
	}
	else
	{
		drawText(px, (px+28)*-1, "Pozostalo " + to_string((czas-glutGet(GLUT_ELAPSED_TIME))/1000), 0, 0, 0);
		drawText(px, (px + 20)*-1, "Punkty = " + to_string(points), 0, 0, 0);
	}

	if (!blokada)
	{
		player.velRY = -mouseSensitivity * (glutGet(GLUT_WINDOW_WIDTH) / 2 - mouseX);
		player.velRX = mouseSensitivity * (glutGet(GLUT_WINDOW_HEIGHT) / 2 - mouseY);
		glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);

		float T = acos(player.dir.y);
		float G = atan2(player.dir.z, player.dir.x);
		T -= player.velRX * .03f;
		G += player.velRY * .03f;
		player.dir.x = sin(T) * cos(G);
		player.dir.y = cos(T);
		player.dir.z = sin(T) * sin(G);
	}

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}
