#pragma once
#include "stdafx.h"

//swiatlo nie dziala na podlodze/ scianach pokoju
//dlatego ze wierzcholki znajduja sie w kontach pokoju. 
//musialo by byc wiecej wierzcholkow na podloge

int rozmiar = 100; // rozmiar mapy
int ilosc_kolumn = 150; // ilosc kolumn
bool blokada = 0; // blokada poruszania sie na koniec
int czas = 30000; // czas gry =30 sek
int points = 0;
int ilosc_gwiazdek = 100; // ilosc gwaizdek
int skalaPokoju = 8;

// swiatlo - cechy materialow
float Material[4]; // uniwersalna tablica cech materialu
float Specular[4] = {0,0,0,1}; // tablica na SPECULAR // odblask

// cechy swiatla
float l_amb[] = { .5f, .5f, .5f, 1.0f };
float l_dif[] = { 0.7f, 0.7f, 0.7f, 1.0f };
float l_spe[] = { .705f, .705f,.705f, 1.0f };
float l_pos[] = {rozmiar/2, .5f, rozmiar / 2, 1 }; // na bierzaco w miejscu gracza


// wektor 3d
struct vector3d
{
float x, y, z, a;
};
//  kamery
struct SCameraState
{
vector3d pos; // pozycja kamery
vector3d dir; // kierunek patrzenia (wektor jednostkowy)
float speed; // mno¿nik zmian pozycji - "szybkoœæ" ruchów kamery
};

void kolizja_gwiazdka(int, int);
bool czy_kolizja_sciana(int, int);

bool keystate[256]; // zmienne okreslajace wcisnietosc klawiszy
vector <vector3d> gwiazdki;
GLuint gwiazda_bitmap;
// Funkcja odczytuj¹ca bitmapê i tworz¹ca na jej podstawie teksturê z zadanym rodzajem filtracji
GLuint LoadTexture(char * file, int magFilter, int minFilter) {

	// Odczytanie bitmapy
	Bitmap *tex = new Bitmap();
	if (!tex->loadBMP(file)) {
		printf("ERROR: Cannot read texture file \"%s\".\n", file);
		return -1;
	}

	// Utworzenie nowego id wolnej tekstury
	GLuint texId;
	glGenTextures(1, &texId);

	// "Bindowanie" tekstury o nowoutworzonym id
	glBindTexture(GL_TEXTURE_2D, texId);

	// Okreœlenie parametrów filtracji dla tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter); // Filtracja, gdy tekstura jest powiêkszana
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter); // Filtracja, gdy tekstura jest pomniejszana

																	  // Wys³anie tekstury do pamiêci karty graficznej zale¿nie od tego, czy chcemy korzystaæ z mipmap czy nie
	if (minFilter == GL_LINEAR_MIPMAP_LINEAR || minFilter == GL_LINEAR_MIPMAP_NEAREST) {
		// Automatyczne zbudowanie mipmap i wys³anie tekstury do pamiêci karty graficznej
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tex->width, tex->height, GL_RGB, GL_UNSIGNED_BYTE, tex->data);
	}
	else {
		// Wys³anie tekstury do pamiêci karty graficznej 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->width, tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex->data);
	}

	// Zwolnienie pamiêci, usuniêcie bitmapy z pamiêci - bitmapa jest ju¿ w pamiêci karty graficznej
	delete tex;

	// Zwrócenie id tekstury
	return texId;
}

SCameraState player;

void drawText(int x, int y, std::string st, float q, float w, float e)
{
	glPushMatrix();
	glDisable(GL_LIGHTING); // Wylaczenie oswietlenia
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
	glEnable(GL_LIGHTING); // Wlaczenie oswietlenia
	glPopMatrix();
}

void OnTimer(int id) {
	// chodzenie - kolizje
	if (blokada)
		return;

	if (keystate['w'] || keystate['W']) 
	{
		float x= player.pos.x + (player.dir.x * player.speed);
		float z = player.pos.z + (player.dir.z * player.speed);
		if (!czy_kolizja_sciana(x,z))
		{
			kolizja_gwiazdka(x, z);
			player.pos.x = x;
			player.pos.z = z;
		}
	}
	if (keystate['s'] || keystate['S']) {
		float x = player.pos.x - (player.dir.x * player.speed);
		float z = player.pos.z - (player.dir.z * player.speed);
		if (!czy_kolizja_sciana(x, z))
		{
			kolizja_gwiazdka(x, z);
			player.pos.x = x;
			player.pos.z = z;
		}
	}
	if (keystate['d'] || keystate['D']) {
			float x = player.pos.x + (-player.dir.z*player.speed);
			float z = player.pos.z + (player.dir.x * player.speed);
			if (!czy_kolizja_sciana(x, z))
			{
				kolizja_gwiazdka(x, z);
				player.pos.x = x;
				player.pos.z = z;
			}
	}
	if (keystate['a'] || keystate['A']) {
		float x = player.pos.x + (player.dir.z*player.speed);
		float z = player.pos.z + (-player.dir.x * player.speed);
		if (!czy_kolizja_sciana(x, z))
		{
			kolizja_gwiazdka(x, z);
			player.pos.x = x;
			player.pos.z = z;
		}
	}

	// kamera
	float sensitivity = 0.08;
	if (keystate['e']) // w prawo (e)
	{
		float fi = atan2((player.dir.x), (player.dir.z)) - sensitivity;
		float teta = asin(player.dir.y);
		player.dir.x = cos(teta)*sin(fi);
		player.dir.y = sin(teta);
		player.dir.z = cos(teta)*cos(fi);
	}

	if (keystate['q']) // w lewo (q)
	{
		float fi = atan2((player.dir.x), (player.dir.z)) + sensitivity;
		float teta = asin(player.dir.y);
		player.dir.x = cos(teta)*sin(fi);
		player.dir.y = sin(teta);
		player.dir.z = cos(teta)*cos(fi);
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
	//glColor3f(color.x, color.y, color.z);
	// cechy materialu
	Material[0] = color.x;
	Material[1] = color.y;
	Material[2] = color.z;
	Material[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, Material);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Material);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Specular);
	glMaterialf(GL_FRONT, GL_SHININESS, .1f);

	glTranslatef(pos.x, pos.y, pos.z);
	glScalef(size.x, size.y, size.z);
	glutSolidCube(1);
	glPopMatrix();
}

bool czy_kolizja_sciana(int x, int z) 
{
	if (x<=0 || z<=0 || x>=rozmiar || z>=rozmiar) // sciany pokoju
		return 1;
	for (int i = 6; i < sciany.size(); i++) // kolumny
	{
		if (abs(sciany[i]->pos.x - x)<1.15 && abs(sciany[i]->pos.z - z)<1.15) // odleglosc x z miedzy scianami jest mniejsza niz 1.15
				return 1;
	}
	return 0;
}

void kolizja_gwiazdka(int x, int z)
{
	for (int i = 0; i < gwiazdki.size(); i++)
	{
		if (abs(gwiazdki[i].x - x)<1.2 && abs(gwiazdki[i].z - z)<1.2) // jesli odleglosc miedzy x i z sa mniejsze niz 1.2
		{
			// zebranie gwaizdki
			points += 100;
			gwiazdki.erase(gwiazdki.begin() + i);
			return;
		}
	}
}

float px = -128;  // animacja na koncu
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

	//ustawienie swiatla w miesjcu gracza
	l_pos[0] = player.pos.x;
	l_pos[1] = player.pos.y;
	l_pos[2] = player.pos.z;
	l_pos[3] = 1.0f;
	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_AMBIENT, l_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l_dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, l_spe);
	glLightfv(GL_LIGHT0, GL_POSITION, l_pos);
	glPopMatrix();
	// tlumioenie swiatla
	glPushMatrix();
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1); // ustawienie odleglosci tlumnia - wieksza=ciemniej
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);
	glPopMatrix();

	glPushMatrix();
	for (int i = 0; i < sciany.size(); i++)
		sciany[i]->rysuj();
	glPopMatrix();

	// RYSOWANIE OTEKSTUROWANYCH QUADOW

	// W³¹czamy teksturowanie
	glEnable(GL_TEXTURE_2D);

	// Ustawienie sposobu teksturowania - GL_MODULATE sprawia, ¿e œwiat³o ma wp³yw na teksturê; GL_DECAL i GL_REPLACE rysuj¹ teksturê tak jak jest
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	float rot = glutGet(GLUT_ELAPSED_TIME) / 50;
	for (int i = 0; i<gwiazdki.size(); i++)
	{
		glBindTexture(GL_TEXTURE_2D, gwiazda_bitmap);
		glPushMatrix();
		glTranslatef(gwiazdki[i].x, gwiazdki[i].y, gwiazdki[i].z);
		glRotatef(rot, 0.0f, 1.0f, 0.0f);


		glBegin(GL_QUADS);

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);

		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);


		glTexCoord2f(0.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);

		glTexCoord2f(0.0f, 1.0f);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, 0.0f);

		glTexCoord2f(1.0f, 1.0f);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);

		glTexCoord2f(1.0f, 0.0f);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);

		glEnd();

		glPopMatrix();
	}
	glDisable(GL_TEXTURE_2D);

	if (glutGet(GLUT_ELAPSED_TIME)>czas)
	{
		blokada = 1;
		drawText(px, (px + 28)*-1, "Koniec czasu ", 1,1,1);
		drawText(px, (px + 20)*-1, "Punkty = " + to_string(points), 1,1,1);
		if (glutGet(GLUT_ELAPSED_TIME) > czas + 5000)
			glutLeaveMainLoop();
		if (px<0)
			px += 0.1;
	}
	else
	{
		drawText(px, (px + 28)*-1, "Pozostalo " + to_string((czas - glutGet(GLUT_ELAPSED_TIME)) / 1000), 1,1,1);
		drawText(px, (px + 20)*-1, "Punkty = " + to_string(points), 1,1,1);
	}

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}
