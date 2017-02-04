#pragma once
#include "stdafx.h"
#include <vector>
#include <fstream>
#include <string>

#ifndef simTime
#define simTime 5
#endif

#ifndef gravity
#define gravity 10
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class PendulumOnPendulum
{
public:
	PendulumOnPendulum(double _lenght, double _lenght2, double _angle, double _angle2, 
		double _muting, double _muting2, double _weight, double _weight2, double timeUnit);
	PendulumOnPendulum(std::string file,double l1,double l2,double m1,double m2);
	void Draw();
	~PendulumOnPendulum();
private:
	std::string file;
	void Print();
	void Update();
	double time;
	double lenght;
	double lenght2;
	std::vector<std::vector<double>> integrators;
	std::vector<double> angle;
	std::vector<double> angle2;
	double muting;
	double muting2;
	double weight;
	double weight2;
	double timeUnit;
};
