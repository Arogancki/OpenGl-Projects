#pragma once
#include "stdafx.h"

// sterowanie wasd serowanie qe obracanie escape wyjscie
// Labirynt tworzony dynamicznie, randomowo, bez cykli, 
// Wiem ze tekstura na quadzie znajdzki sie obrucila ale nie chce mi sie tego poprawiac
// jesli przeszkadza zmien teksture
// tektury w formacie bmp 24 kolory, najlepiej 128x128
// Wszytkie zmienne ponizsze stanowia interfejs ustawien - zmieniane do woli
// ustawione na preferowane wartosci
// displaylisty pozwalaja na zapis lokalizajcji wierzcholkow w karcie graficznej nie pamieci
// dzieki czemu przy dobrej karcie (bron banie Jezusie zintegrowanej (TFUUU), dziala szybciej niz 
// tradycyjne wysweitlanie. 
// kolizja rozwiazana ze znajdzka na odleglosci bezwzglednej miedzy graczem a znajdzka
// kolizja ze sciana rozwiazana na podstawie wejscia w pole sciany
// swiatlo nie dziala na podlodze
// dlatego ze wierzcholki znajduja sie w kontach pokoju. 
// musialo by byc wiecej wierzcholkow na podloge
// mozna by bylo zrobic w 2x forach po kawalku, 
// ale po co , jest gitesnie - i tak jest zbyt wiele wierzcholkow rysowanych co petle
// trzeba by bylo zastosowac ograniczenie rysowanych elementow do wylacznie widzianych
// brak sufitu ukryty krotkim swiatlem
// Labirynt specjalnie sie zaciemnia czym dalej jestesmy od znajdzek - zeby bylo trudniej

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
};

float rozmiarZnajdzki = 1.0; // wielkosc kwadratu do znalezienia
int skalaPokoju = 8; // skala mapy 1:skala
int rozmiar = 15 * skalaPokoju; // rozmiar mapy kwadratowy
bool blokada = 0; // blokada poruszania sie na koniec, po czasie/wygranej
int czas = 120000; // czas gry x sek
int points = 0; // aktualna ilosc zebranych punktow na poczatku gry
int pointsMax = 3; // ilosc obiektow do zebrania
float kolor = 0.2;// kolor scian i napisow(przeciwny), aktualizowany zaleznie od odleglosci najblizszej znajdzki
int startX=0, startY=0; // warotsci przykrywane po stworznieu mapy(main), startowe dla gracza
bool keystate[256]; // zmienne okreslajace wcisnietosc klawiszy
vector <vector3d> znajdzki; // wektor itemow do podniesienia
GLuint znajdzki_bitmap; // bitmapa znajdziek
GLuint wall_bitmap; // bitmapy na sciane
GLuint znajdzkiLista; // display lista dla znajdziek
GLuint ScianyLista; // display lista dla znajdziek
vector <int> doskasowania; // wektor podniesionych itemow i kasowanych w nastepnej ramce
int indexNajblizszejZnajdzki = 0; // index w tablicy najblizszej gwiazdki
int nextframe = 30; // za ile milisekund zostanie wygenerowana minimalnie nastepna ramka
int frame_start = nextframe*-1; // nie zmieniac!!, do przechowywania czasu ostatniej ramki
int wysietlamy = 0; // zmienna pamietajaca o wyswietlaniu ze podniesiono gwiazdke przez sekunde
float szybkosc = 0.8; // szybkosc poruszania sie

// swiatlo - cechy materialow
float Material[4]; // uniwersalna tablica cech materialu
float Specular[4] = {0,0,0,1}; // tablica na SPECULAR // odblask

// cechy swiatla
float l_amb[] = { .5f, .5f, .5f, 1.0f };
float l_dif[] = { 0.7f, 0.7f, 0.7f, 1.0f };
float l_spe[] = { .805f, .805f, .805f, 0.0f };
float l_pos[] = {rozmiar/2, .5f, rozmiar / 2, 1 }; // na bierzaco w miejscu gracza, aktualizacja po januszowatemu w main loopie
float l_pos2[] = { rozmiar / 2, .5f, rozmiar / 2, 1 }; // w miejscu najblizszej znajdzki
float tlumienie[3] = { 0.2,0.4,0.2 };

void kolizja_znajdzki(int, int); // naglowek
bool czy_kolizja_sciana(int, int); // naglowek

//laduje textury
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

// zmienna gracza
SCameraState player;

// funkcja piszaca tekst na ekranie
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

// timer
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
			kolizja_znajdzki(x, z);
			player.pos.x = x;
			player.pos.z = z;
		}
	}
	if (keystate['s'] || keystate['S']) {
		float x = player.pos.x - (player.dir.x * player.speed);
		float z = player.pos.z - (player.dir.z * player.speed);
		if (!czy_kolizja_sciana(x, z))
		{
			kolizja_znajdzki(x, z);
			player.pos.x = x;
			player.pos.z = z;
		}
	}
	if (keystate['d'] || keystate['D']) {
			float x = player.pos.x + (-player.dir.z*player.speed);
			float z = player.pos.z + (player.dir.x * player.speed);
			if (!czy_kolizja_sciana(x, z))
			{
				kolizja_znajdzki(x, z);
				player.pos.x = x;
				player.pos.z = z;
			}
	}
	if (keystate['a'] || keystate['A']) {
		float x = player.pos.x + (player.dir.z*player.speed);
		float z = player.pos.z + (-player.dir.x * player.speed);
		if (!czy_kolizja_sciana(x, z))
		{
			kolizja_znajdzki(x, z);
			player.pos.x = x;
			player.pos.z = z;
		}
	}

	// kamera
	float sensitivity = 0.28;
	if (keystate['e'] || keystate['E']) // w prawo (e)
	{
		float fi = atan2((player.dir.x), (player.dir.z)) - sensitivity;
		float teta = asin(player.dir.y);
		player.dir.x = cos(teta)*sin(fi);
		player.dir.y = sin(teta);
		player.dir.z = cos(teta)*cos(fi);
	}

	if (keystate['q'] || keystate['Q']) // w lewo (q)
	{
		float fi = atan2((player.dir.x), (player.dir.z)) + sensitivity;
		float teta = asin(player.dir.y);
		player.dir.x = cos(teta)*sin(fi);
		player.dir.y = sin(teta);
		player.dir.z = cos(teta)*cos(fi);
	}

glutTimerFunc(17, OnTimer, 0);
}

//zmiana wielkosci okna
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

// KLAWIATURA- Obs³uga zdarzenia puszczenia klawisza.
void OnKeyUp(unsigned char key, int x, int y) {
keystate[key] = false;
if (key<224)
keystate[key + 32] = false;
if (key>31)
keystate[key - 32] = false;// ODPRONE NA PUSZCZENIE clk
}

// klasa sciany
class wall
{
public:
	void createlist();
	vector3d pos;
	vector3d size;
	vector3d color;
	wall(float, float, float, float, float, float, float, float , float);
	~wall() {}
	void editcolor(float, float, float);
	void rysuj();
	void rysuj2();
	GLuint dislist[3];
};

// wektor zawierajacy wszytkie sciany
vector <wall*> sciany;
wall::wall(float px, float py, float pz, float sx, float sy, float sz, float red, float green, float blue)
{
	dislist[0] = -1;
	dislist[1] = -1;
	dislist[2] = -1;
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
//stworznie display listy (w klasie)
void wall::createlist()
{
	float adu[4], spe[4];
	adu[0] = 0.5;
	adu[1] = 0.5;
	adu[2] = 0.5;
	adu[3] = 1.0f;
	spe[0] = 0.0f;
	spe[1] = 0.0f;
	spe[2] = 0.0f;
	spe[3] = 1.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, adu);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, adu);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spe);
	glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);

	dislist[0] = glGenLists(1);
	glNewList(dislist[0], GL_COMPILE);
	glPushMatrix();
	

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 1.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5, 0.5, 0);

	glTexCoord2f(0.0f, 0.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5, -0.5, 0);

	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5, -0.5, 0);

	glTexCoord2f(1.0f, 1.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5, 0.5, 0);

	glEnd();

	glPopMatrix();
	glEndList();

	dislist[1] = glGenLists(1);
	glNewList(dislist[1], GL_COMPILE);
	for (float j = (this->size.x / -2.0) + 0.5; j < this->size.x / 2.0; j++)
	{
		for (float k = ((this->size.y) / -2.0) ; k <= (this->size.y / 2.0)+0.5; k++)
		{
			glPushMatrix();
			glTranslatef(j, k, 0);
			glBindTexture(GL_TEXTURE_2D, wall_bitmap);
			glCallList(dislist[0]);
			glPopMatrix();
		}
	}
	glEndList();

	dislist[2] = glGenLists(1);
	glNewList(dislist[2], GL_COMPILE);
	for (int i = 0; i < 4; i++)
	{
		glPushMatrix();
		glRotatef(90 * i, 0.0, 1.0, 0.0);
		//jesli zle obrucone tu blad
		glTranslatef(0, 0, skalaPokoju / 2);
		glCallList(dislist[1]);
		glPopMatrix();
	}
	glEndList();
	
}
//edytuje kolor sciany (tylko dla podlogi)
void wall::editcolor(float x, float y, float z)
{
	color.x = x;
	color.y = y;
	color.z = z;
}
//rysowanie nie oteksturowanej sciany - podlogi
void wall::rysuj()
{
	glPushMatrix();
	glColor3f(color.x, color.y, color.z);
	glTranslatef(pos.x, pos.y, pos.z);
	glScalef(size.x, size.y, size.z);
	glutSolidCube(1);
	glPopMatrix();
}
// rysowanie oteksturowanych scian
void wall::rysuj2()
{
	glPushMatrix();
	glTranslatef(pos.x,pos.y,pos.z);
	glCallList(dislist[2]);
	
	glPopMatrix();
}
// tworzy displayliste znajdziek
void StworzListeZnajdzki()
{
	znajdzkiLista = glGenLists(1);
	glNewList(znajdzkiLista, GL_COMPILE);
	glPushMatrix();

	glBegin(GL_QUADS);

	glTexCoord2f(0.0f, 1.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-rozmiarZnajdzki / 2, rozmiarZnajdzki / 2, rozmiarZnajdzki / 2);

	glTexCoord2f(0.0f, 0.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-rozmiarZnajdzki / 2, -rozmiarZnajdzki / 2, rozmiarZnajdzki / 2);

	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(rozmiarZnajdzki / 2, -rozmiarZnajdzki / 2, rozmiarZnajdzki / 2);

	glTexCoord2f(1.0f, 1.0f);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(rozmiarZnajdzki / 2, rozmiarZnajdzki / 2, rozmiarZnajdzki / 2);

	glTexCoord2f(0.0f, 0.0f);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-rozmiarZnajdzki / 2, -rozmiarZnajdzki / 2, -rozmiarZnajdzki / 2);

	glTexCoord2f(0.0f, 1.0f);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-rozmiarZnajdzki / 2, rozmiarZnajdzki / 2, -rozmiarZnajdzki / 2);

	glTexCoord2f(1.0f, 1.0f);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(rozmiarZnajdzki / 2, rozmiarZnajdzki / 2, -rozmiarZnajdzki / 2);

	glTexCoord2f(1.0f, 0.0f);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(rozmiarZnajdzki / 2, -rozmiarZnajdzki / 2, -rozmiarZnajdzki / 2);

	glEnd();

	glPopMatrix();
	glEndList();
}
// tworzy globalna dispalayliste scian
void StworzListeSciany()
{
	for (int i = 1; i<sciany.size(); i++)
		sciany[i]->createlist();
	ScianyLista = glGenLists(1);
	glNewList(ScianyLista, GL_COMPILE);
	for (int i = 1; i < sciany.size(); i++)
		sciany[i]->rysuj2();
	glEndList();
}
// sprawdza kolizje ze sciana
bool czy_kolizja_sciana(int x, int z) 
{
	if (x<=0 || z<=0 || x>=rozmiar || z>=rozmiar) // sciany pokoju
		return 1;
	for (int i = 1; i < sciany.size(); i++) // sciany labiryntu
	{
		/* warunki
		1|2|3
		4|K|5    k -kolizja, gracz moze znales sie tylko w konkatenacjach pol
		6|7|8
		*/
		bool W2 = z >= sciany[i]->pos.z + (skalaPokoju / 2.0);
		bool W4 = x < sciany[i]->pos.x - (skalaPokoju / 2.0);
		bool W5 = x >= sciany[i]->pos.x + (skalaPokoju / 2.0);
		bool W7 = z < sciany[i]->pos.z - (skalaPokoju / 2.0);
		bool W1 = W2 && W4;
		bool W3 = W2 && W5;
		bool W6 = W4 && W7;
		bool W8 = W5 && W7;
		if (!W1 && !W2 && !W3 && !W4 && !W5 && !W6 && !W7 && !W8)
			return 1;
	}
	return 0;
}
// kolizja znajdzki
void kolizja_znajdzki(int x, int z)
{
	for (int i = 0; i < znajdzki.size(); i++)
	{
		if (sqrt(pow(x - znajdzki[i].x, 2) + pow(z - znajdzki[i].z, 2))<(((float)rozmiarZnajdzki)*sqrt(2))) // jesli odleglosc miedzy x i z sa mniejsze 
		{
			// zebranie gwaizdki
			points += 1;
			doskasowania.push_back(i);
			return;
		}
	}
}
// rysowanie znajdzki
void rysujZnajdzke(int i, float rot)
{
	glPushMatrix();
	glTranslatef(znajdzki[i].x, znajdzki[i].y, znajdzki[i].z);
	glRotatef(rot, 0.0f, 1.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, znajdzki_bitmap);
	glCallList(znajdzkiLista);
	glPopMatrix();
}
// petla glowna
void OnRender()
{
	// generowanie klatek ograniczone
	if (frame_start + nextframe - glutGet(GLUT_ELAPSED_TIME) > 0 )
		Sleep(frame_start + nextframe - glutGet(GLUT_ELAPSED_TIME));
	// zapamitanie kiedy zrobiono ostania klatke
	frame_start = glutGet(GLUT_ELAPSED_TIME);

	// zamiana aktualnego bufora, nastepny do przygotowania *(chyba to to)
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
	if (znajdzki.size())
	{ // ustawienie swiatla w miejscu najblizszej znajdzki
		l_pos2[0] = znajdzki[indexNajblizszejZnajdzki].x;
		l_pos2[1] = znajdzki[indexNajblizszejZnajdzki].y;
		l_pos2[2] = znajdzki[indexNajblizszejZnajdzki].z;
		l_pos[3] = 1.0f;
	}
	else
		glDisable(GL_LIGHT0 + 1); // Wylaczenie swiatla znajdzki jesli juz ich nie ma
	// parametry swiatla
	glLightfv(GL_LIGHT0, GL_POSITION, l_pos);
	glLightfv(GL_LIGHT0+1, GL_POSITION, l_pos2);
	for (int i = 0; i < 2; i++)
	{
		glPushMatrix();
		glLightfv(GL_LIGHT0+i, GL_AMBIENT, l_amb);
		glLightfv(GL_LIGHT0+i, GL_DIFFUSE, l_dif);
		glLightfv(GL_LIGHT0+i, GL_SPECULAR, l_spe);
		glPopMatrix();
		// tlumioenie swiatla
		glPushMatrix();
		glLightf(GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, tlumienie[0]);
		glLightf(GL_LIGHT0+i, GL_LINEAR_ATTENUATION, tlumienie[1]); // ustawienie odleglosci tlumnia 
		glLightf(GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, tlumienie[2]);
		glPopMatrix();
	}

	// czy sa znajdzki podniesione - do usuniecia
	if (!doskasowania.empty())
	{
		for (int i = 0; i < doskasowania.size(); i++)
			znajdzki.erase(znajdzki.begin() + doskasowania[i]);
		doskasowania.clear();
		// zapis ze teraz podniesiono gwiazde
		wysietlamy = glutGet(GLUT_ELAPSED_TIME) + 2000;
	}
	// jesli w ostatniej sekundzie podnioslem gwiade
	if (wysietlamy > glutGet(GLUT_ELAPSED_TIME))
	{
		// wysietlenie napisu ze mamy znajdzke
		float k1 = ((float)(((glutGet(GLUT_ELAPSED_TIME) % 43) + glutGet(GLUT_ELAPSED_TIME)) % 100)) / 100;
		float k2 = ((float)(((glutGet(GLUT_ELAPSED_TIME) % 17) + glutGet(GLUT_ELAPSED_TIME)) % 100)) / 100;
		float k3 = ((float)(((glutGet(GLUT_ELAPSED_TIME) % 13) + glutGet(GLUT_ELAPSED_TIME)) % 100)) / 100;
		drawText(0, 0, "!! ZNALEZIONO !!", k1, k2, k3);
	}
	
	// znalezienie najblizszej znajdzki i ustalenie koloru do aktualizacji koloru podlogi - czym blizej znajdzki tym jasniej
	if (znajdzki.size()) {
		float odlegloscMin = rozmiar*sqrt(2); // maks mapy (przekatna kwadratu)
		float odlegloscAkt;
		for (int i = 0; i < znajdzki.size(); i++)
		{
			odlegloscAkt = sqrt(pow(player.pos.x - znajdzki[i].x, 2) + pow(player.pos.z - znajdzki[i].z, 2));
			if (odlegloscAkt < odlegloscMin) // podmiana jesli blizsza
			{
				indexNajblizszejZnajdzki = i; // zapis indeksu
				odlegloscMin = odlegloscAkt; // zapis odleglosci
			}
		}
		kolor = (((rozmiar*skalaPokoju*0.1) - (odlegloscMin*1.5)) / (rozmiar*skalaPokoju*0.1))*0.1; // dobrane metoda prob i bledow
		// wypis wskazuwki odleglosci
		drawText(88, 118, "Najblizsza za", round((kolor*-1) + 1), round((kolor*-1) + 1), round((kolor*-1) + 1));
		drawText(88, 110, to_string((int)odlegloscMin), round((kolor*-1) + 1), round((kolor*-1) + 1), round((kolor*-1) + 1));
		
	}
	// rysowanie podlogi
	glPushMatrix();
	glDisable(GL_LIGHTING);
	for (int i = 0; i < 1; i++)
	{
		sciany[i]->editcolor(kolor, kolor, kolor);
		sciany[i]->rysuj();
	}
	glEnable(GL_LIGHTING);
	glPopMatrix();

	// RYSOWANIE OTEKSTUROWANYCH QUADOW- W³¹czamy teksturowanie
	glEnable(GL_TEXTURE_2D);
	// Ustawienie sposobu teksturowania - GL_MODULATE sprawia, ¿e œwiat³o ma wp³yw na teksturê; GL_DECAL i GL_REPLACE rysuj¹ teksturê tak jak jest
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	// rysowanie znajdzek - tylko najblizejsz, po co tracic zasoby na inne
	float rot = glutGet(GLUT_ELAPSED_TIME) /8; // obracanie sie kulki
	glPushMatrix();
	glTranslatef(0, (sin(glutGet(GLUT_ELAPSED_TIME)*0.002)*0.2)-(rozmiarZnajdzki/4), 0);
	rysujZnajdzke(indexNajblizszejZnajdzki, rot - 90); // boki
	rysujZnajdzke(indexNajblizszejZnajdzki, rot); // drugie 2 boki (prostopadle)
	glPushMatrix();
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	rysujZnajdzke(indexNajblizszejZnajdzki, rot); // sufit i podloga
	glPopMatrix();
	glPopMatrix();
	//rysowanie scian
	glPushMatrix();
	glCallList(ScianyLista);
	glPopMatrix();
	// koniec teksturowania - wylaczamy
	glDisable(GL_TEXTURE_2D);

	// czy zebrano wszytko 
	if (points == pointsMax)
	{
		czas = glutGet(GLUT_ELAPSED_TIME) - 1; // jesli wszystko zebrane pozostaly czas rowna sie aktualny - koniec czasu i gry
		points = -1; // tak dla beki i latwiejszych sprawdzan
	}

	// czy skonczyl sie czas
	if (glutGet(GLUT_ELAPSED_TIME)>czas)
	{
		blokada = 1; // blokada ruchu po czasie, realizowana w timerze, tam gdzie poruszanie
		// stworzenie randomowych kolorow przy wysietlaniu napisu konca
		float k1=((float)(((glutGet(GLUT_ELAPSED_TIME) % 43) + glutGet(GLUT_ELAPSED_TIME)) % 100)) / 100;
		float k2 = ((float)(((glutGet(GLUT_ELAPSED_TIME) % 17) + glutGet(GLUT_ELAPSED_TIME)) % 100)) / 100;
		float k3 = ((float)(((glutGet(GLUT_ELAPSED_TIME) % 13) + glutGet(GLUT_ELAPSED_TIME)) % 100)) / 100;
		if (glutGet(GLUT_ELAPSED_TIME) > czas + 1000)
		{
			if (points != -1) // jesli nie wygrano
			{
				drawText(0, 0, "Koniec czasu ", k1, k2, k3);
				drawText(0, 0 + 8, "Zabraklo = " + to_string(pointsMax - points), k1, k2, k3);
			}
			else // zebrano itemy
			{
				drawText(0, 0 + 8, "!! Udalo sie !!", k1, k2, k3);
			}
			if (glutGet(GLUT_ELAPSED_TIME) > czas + 6000)
				glutLeaveMainLoop(); // koniec
		}
		else
		{
			for (int i=0;i<10;i++) // wariujace napisy koniec
				drawText(0 + (128 - (rand() % 256)), 0 + (128 - (rand() % 256)), "KONIEC", k1,k2,k3);
		}
	}
	else // gra trwa - wypisanie statusu gry
	{
		// round aby zawsze bylo 0 lub 1 i przeciwnie do koloru sufitu lub dolu
		drawText(-128, 118, "Pozostalo " + to_string((czas - glutGet(GLUT_ELAPSED_TIME)) / 1000), round((kolor*-1) + 1), round((kolor*-1) + 1), round((kolor*-1) + 1));
		drawText(-128, 110, "Brakuje = " + to_string(pointsMax-points), round((kolor*-1)) + 1, round((kolor*-1) + 1), round((kolor*-1) + 1));
	}

	// zamiana buforow i inne niskopoziomowe pierdoly gluta, nikogo to nie interesuje :)
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}