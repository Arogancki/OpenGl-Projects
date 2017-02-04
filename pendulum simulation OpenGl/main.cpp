#include "stdafx.h"
#include "main.h"

int main(int argc, char* argv[])
{
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Pendulum");

	Start();
}