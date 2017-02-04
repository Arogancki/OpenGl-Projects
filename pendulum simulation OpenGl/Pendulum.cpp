#include "stdafx.h"
#include "Pendulum.h"

using std::cout;

Pendulum::Pendulum(double _lenght,double _angle,double _muting,double _weight,double _timeUnit)
{
	file = "";
	lenght = _lenght;
	angle.push_back((_angle * M_PI) / 180.0);

	std::vector<double> temp;
	temp.push_back(0);
	temp.push_back((_angle * M_PI) / 180.0);
	integrators.push_back(temp);
	temp.clear();

	time = 0.0;
	muting = _muting;
	weight = _weight;
	timeUnit = _timeUnit;
}

Pendulum::Pendulum(std::string name)
{
	file = name;
	std::ifstream myfile(file);
	std::string line = "";
	std::string::size_type s_l;
	
	std::vector<double> temp;
	integrators.push_back(temp);
	temp.clear();

	if (myfile.is_open())
	{
		while (std::getline(myfile, line))
		{
			integrators[0].push_back(std::stod(line, &s_l));
		}
	}
	myfile.close();
	lenght = 0;
}

void Pendulum::Update()
{
	if (time > simTime)
		glutLeaveMainLoop();

	time += timeUnit;

	if (file.empty())
	{

		angle[0] = angle[0] + (integrators[0][0] * timeUnit);
		integrators[0][0] = integrators[0][0] + integrators[0][1];

		double b1 = integrators[0][0] * (muting / weight);

		double b2 = sin(angle[0]) * (gravity / lenght);

		integrators[0][1] = -b1+-b2;



	}
	else
	{
		if (integrators[0][lenght])
			angle.push_back(integrators[0][lenght++]);
		else
			time = simTime;
	}
	
}

Pendulum::~Pendulum()
{
	if (angle[0])
		angle.clear();
	if (integrators[0][0])
		integrators[0].clear();
	if (integrators[1][0])
		integrators[1].clear();
}

void Pendulum::Draw()
{
	Update();
	Print();
	//wahadlo
	glPushMatrix();
	glRotatef(angle[angle.size() - 1] * 57.2957795, 0.0f, 0.0f, 1.0f);
	// nic
	glPushMatrix();
	glTranslatef(0, -2.5, 0);
	glColor3f(0.2f, 0.1f, 0.0f);
	glScalef(0.2, 5, 0.1);
	glutSolidCube(1);
	glPopMatrix();

	// zaczep1
	glPushMatrix();
	glColor3f(0.1f, 0.1f, 0.1f);
	glScalef(0.2, 0.2, 0.2);
	glutSolidSphere(1.2, 32, 32);
	glPopMatrix();

	// brelok
	glPushMatrix();
	glTranslatef(0, -5, 0);
	glColor3f(0.6f, 0.0f, 0.0f);
	glScalef(0.2, 0.2, 0.2);
	glutSolidSphere(2.5, 32, 32);
	glPopMatrix();
	glPopMatrix();
}

void Pendulum::Print()
{
	cout << "Time:\t"<<time<<"\t- Angle:\t" << angle[angle.size()-1] * 57.2957795 << "\n";
}
