#pragma once
#include "stdafx.h"
#include <vector>
#include <fstream>
#include <string>

#ifndef simTime
#define simTime 10
#endif

#ifndef gravity
	#define gravity 10
#endif

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

class Pendulum
{
public:
	Pendulum(double _lenght, double _angle, double _muting, double _weight, double timeUnit);
	Pendulum(std::string name);
	void Draw();
	~Pendulum();
private:
	void Print();
	void Update();
	double time;
	double lenght;
	std::vector<std::vector<double>> integrators;
	std::vector<double> angle;
	double muting;
	double weight;
	double timeUnit;
	std::string file;
};
