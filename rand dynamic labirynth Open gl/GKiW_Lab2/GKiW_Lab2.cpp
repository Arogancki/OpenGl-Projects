// proba gry.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GeometrySoldiers.h"

int main(int argc, char* argv[])
{
	srand(time(NULL));
	cout << "Geometry Soldiers\n";
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(400, 300);
	glutCreateWindow("Geometry Soldiers");
	glutFullScreen();
	engine = createIrrKlangDevice(); // deklaracja w Mapa, inicjalizacja silnika dziwiekowego
	engine->setDefault3DSoundMinDistance(10); // ustawiane na podstawie prob i bledow

											  // Ladowanie tekstur
	stick = new CTexture("bitmap\\desk.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	stick->Load();
	stickH = new CTexture("bitmap\\deskH.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	stickH->Load();
	mtitle = new CTexture("bitmap\\mtitle.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	mtitle->Load();
	mtlo = new CTexture("bitmap\\mtlo.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	mtlo->Load();
	koniec = new CTexture("bitmap\\koniec.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	koniec->Load();
	pomoc = new CTexture("bitmap\\pomoc.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	pomoc->Load();
	green = new CTexture("bitmap\\green.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	green->Load();
	stronapomocy = new CTexture("bitmap\\kartapomocy.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	stronapomocy->Load();
	sojusznicy = new CTexture("bitmap\\sojusz.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	sojusznicy->Load();
	samotnik = new CTexture("bitmap\\samotnik.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	samotnik->Load();
	wybierz = new CTexture("bitmap\\wybor.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	wybierz->Load();
	bar = new CTexture("bitmap\\bar.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	bar->Load();
	guns = new CTexture("bitmap\\gun.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	guns->Load();
	miner = new CTexture("bitmap\\miner.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	miner->Load();
	loadning = new CTexture("bitmap\\ladowanie.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	loadning->Load();
	info = new CTexture("bitmap\\me.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	info->Load();
	lvup = new CTexture("bitmap\\lvup.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	lvup->Load();
	how = new CTexture("bitmap\\info.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	how->Load();
	stick = new CTexture("bitmap\\desk.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	stick->Load();
	stickH = new CTexture("bitmap\\deskH.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	stickH->Load();
	dead = new CTexture("bitmap\\dead.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	dead->Load();
	blue = new CTexture("bitmap\\blue.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	blue->Load();
	red = new CTexture("bitmap\\red.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	red->Load();
	grey = new CTexture("bitmap\\grey.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	grey->Load();
	pauzat = new CTexture("bitmap\\pauza.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	pauzat->Load();
	gotowy = new CTexture("bitmap\\gotowy.bmp", GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	gotowy->Load();

	Menu();
}

