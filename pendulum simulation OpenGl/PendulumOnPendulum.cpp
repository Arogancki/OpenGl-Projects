#include "stdafx.h"
#include "PendulumOnPendulum.h"

using std::cout;

PendulumOnPendulum::PendulumOnPendulum(double _lenght, double _lenght2, double _angle, double _angle2,
	double _muting, double _muting2, double _weight, double _weight2, double _timeUnit)
{
	lenght = _lenght;
	lenght2 = _lenght2;

	angle.push_back((_angle * M_PI) / 180.0);
	angle2.push_back((_angle2 * M_PI) / 180.0);

	time = 0.0;
	muting = _muting;
	muting2 = _muting2;
	weight2 = _weight2;
	weight = _weight;
	timeUnit = _timeUnit;
}

void PendulumOnPendulum::Update()
{
	if (time > simTime)
		glutLeaveMainLoop();

	time += timeUnit;

	if (file.empty())
	{
		double b1 = integrators[0][angle.size() - 1] * (muting / weight);

		double b2 = sin(integrators[1][angle.size() - 1]) * (gravity / lenght);

		integrators[0].push_back(((-b1 + -b2)*time) + integrators[0][0]);

		integrators[1].push_back((integrators[0][angle.size() - 1] * time) + integrators[1][0]);

		angle.push_back(integrators[1][angle.size() - 1]);
	}
	else
	{
		if (integrators[0][muting] && integrators[1][muting])
		{
			angle.push_back(integrators[0][muting]);
			angle2.push_back(integrators[1][muting++]);
		}
		else
			time = simTime;
	}

}

PendulumOnPendulum::PendulumOnPendulum(std::string name,double l1, double l2, double m1, double m2)
{
	file = name;
	std::ifstream myfile(file);
	std::string line = "";
	std::string::size_type s_l;

	std::vector<double> temp;
	integrators.push_back(temp);
	integrators.push_back(temp);
	temp.clear();

	if (myfile.is_open())
	{
		while (std::getline(myfile, line))
		{
			integrators[0].push_back(std::stod(line, &s_l));
			if (std::getline(myfile, line))
			{
				integrators[1].push_back(std::stod(line, &s_l));
			}
			else
				integrators[1].push_back(0);
		}
	}
	myfile.close();
	muting = 0;
	lenght = l1;
	lenght2 = l2;
	weight = m1;
	weight2 = m2;
}

PendulumOnPendulum::~PendulumOnPendulum()
{
	if (angle[0])
		angle.clear(); 
	if (angle2[0])
		angle2.clear();
	if (integrators[0][0])
		integrators[0].clear();
	if (integrators[1][0])
		integrators[1].clear();
}

void PendulumOnPendulum::Draw()
{
	Update();
	Print();

	//wahadlo
		glRotatef(angle[angle.size() - 1] * 57.2957795, 0.0f, 0.0f, 1.0f);
		
	// zaczep
		glPushMatrix();
			glColor3f(0.1f, 0.1f, 0.1f);
			glScalef(0.2, 0.2, 0.2);
			glutSolidSphere(1.2, 32, 32);
		glPopMatrix();
		//lina
		glTranslatef(0, -2.5 * (lenght / (lenght + lenght2)), 0);

		glPushMatrix();
			glColor3f(0.2f, 0.1f, 0.0f);
			glScalef(0.2, 5 * (lenght / (lenght + lenght2)), 0.1);
			glutSolidCube(1);
		glPopMatrix();
	
	// brelok
		glTranslatef(0, -2.5 * (lenght / (lenght + lenght2)), 0);

		glPushMatrix();
			glColor3f(0.6f, 0.0f, 0.0f);
			glScalef(0.2, 0.2, 0.2);
			glutSolidSphere(4* (weight / (weight + weight2)), 32, 32);
		glPopMatrix();

		// druga

		glRotatef((angle2[angle2.size() - 1]- angle[angle.size() - 1]) * 57.2957795, 0.0f, 0.0f, 1.0f);
			
			//nowa nic
		glTranslatef(0, -2.5*(lenght2 / (lenght + lenght2)), 0);
			glPushMatrix();
				glColor3f(0.2f, 0.0f, 0.2f);
				glScalef(0.2, 5* (lenght2 / (lenght + lenght2)), 0.1);
				glutSolidCube(1);
			glPopMatrix();
			//nowy brylok
			glTranslatef(0, -2.5*(lenght2 / (lenght + lenght2)), 0);
			glPushMatrix();
				glColor3f(0.0f, 0.6f, 0.0f);
				glScalef(0.2, 0.2, 0.2);
				glutSolidSphere(4* (weight2 / (weight + weight2)), 32, 32);
			glPopMatrix();
			
	
}

void PendulumOnPendulum::Print()
{
	cout << "Time:\t" << time << "\t- Angle1:\t" << angle[angle.size() - 1] * 57.2957795 << "\t- Angle2:\t" << angle2[angle2.size() - 1] * 57.2957795 << "\n";
}
