#include "stdafx.h"
#include "Labyrinth.h"
#include <conio.h>
#include <ctime>
#include <complex>

Labyrinth::Labyrinth(int _width,int _height)
{
	width = _width;
	height=_height;
	if (width < 4)
		width = 4;
	if (height < 4)
		height = 4;
	if (width > 80)
		width = 80;
	if (height > 80)
		height = 80;
	fields = new field*[width];
	for (int i = 0; i < width; ++i)
	{
		fields[i] = new field[height];
		for (int j = 0; j < height; j++)
		{
				fields[i][j].type = 0;
		}
	}
	srand(time(NULL));
	int x=4 +(rand() % (width-8));
	int y =4+ (rand() % (height-8));
	build_labyrinth(-1,-1,x,y);
	fields[x][y].type = 5; // start/meta
	/*shortestWayToFinish= findFarestRoom(x, y);
	std::vector<int> neighborsX = { 1, 0, -1,  0 };
	std::vector<int> neighborsY = { 0, 1,  0, -1 };
	for (int i=shortestWayToFinish.size()-1;i>=0;i--)
	{
		x = x + neighborsX[shortestWayToFinish[i]];
		y = y + neighborsY[shortestWayToFinish[i]];
	}
	fields[x][y].type = 5; // meta
	*/
}

int* Labyrinth::setItemLocation(int n)
{
	int *locations;
	locations= new int[2*n];
	for (int i=0;i<n;i++)
	{
		start:
		int x = 1+(rand() % (width-1));
		int y = 1+(rand() % (height-1));
		if (fields[x][y].type == 1)
		{
			fields[x][y].type = 2;
			locations[i] = x;
			locations[i + 1] = y;
		}
		else
			goto start;
	}
	return(locations);
}

int* Labyrinth::GiveWallLocation()
{
	int counter = 0;
	for (int x = 0; x<width; x++)
	{
		for (int y = 0; y < height; y++)
			if (fields[x][y].type == 0)
				// policzenie ile jest scian
				counter++;
	}
	int *locations;
	locations = new int[(counter*2)+1];
	counter = 0;
	for (int x = 0; x<width; x++)
	{
		for (int y = 0; y<height; y++)
			if (fields[x][y].type == 0)
			{
				locations[counter] = x;
				locations[counter+1] = y;
				counter += 2;
			}
	}
	locations[counter] = -1;
	return(locations);
}

int Labyrinth::getPoint(bool axis, int type) // 0 dla x 1 dla y
{
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < height; j++)
			if (fields[i][j].type == type)
				if (!axis)
					return(i);
				else
					return(j);
	return(0);
}
void Labyrinth::build_labyrinth(int parentX,int parentY,int x,int y)
{
	//std::cout << this->print();
	//getchar();
	fields[x][y].type = 1;
	std::vector<int> neighborsX={ 1, 0, -1,  0 };
	std::vector<int> neighborsY={ 0, 1,  0, -1 };
	while (neighborsX.size()>0)
	{
		int _rand = rand() % neighborsX.size();
		if (x + neighborsX[_rand] == parentX && y + neighborsY[_rand] == parentY)
			;
		else
			if (is(x + neighborsX[_rand], y+ neighborsY[_rand],0)==1 && isNeighborsNot(x,y,x + neighborsX[_rand], y + neighborsY[_rand], 1)==1)
				build_labyrinth(x,y,x + neighborsX[_rand], y+ neighborsY[_rand]);
		neighborsX.erase(neighborsX.begin() + _rand);
		neighborsY.erase(neighborsY.begin() + _rand);
	}
	
}

std::vector<int> Labyrinth::findFarestRoom(int x, int y)
{
	std::vector<int> _return = findFarestRoomRec(-1, -1, x, y);
	_return.erase(_return.begin());
	return (_return);
}

std::vector<int> Labyrinth::findFarestRoomRec(int parentX, int parentY,int x, int y)
{
	std::vector<int> neighborsX = { 1, 0, -1,  0 };
	std::vector<int> neighborsY = { 0, 1,  0, -1 };
	std::vector<int> neighbors[4]={{},{},{},{}};
	for (int i = 0; i < 4;i++)
	{
		if (x + neighborsX[i] == parentX && y + neighborsY[i] == parentY) // no repetition
			;
		else
			if (is(x + neighborsX[i], y + neighborsY[i], 1) == 1)
			{
				std::vector<int> temp = findFarestRoomRec(x, y, x + neighborsX[i], y + neighborsY[i]);
				neighbors[i].insert(neighbors[i].end(), temp.begin(), temp.end());
			}
	}
	// find farest point for finish line
	int maxIndex = rand()%4;
	for (int i = 0; i < 4; i++)
		if (neighbors[maxIndex].size() < neighbors[i].size())
			maxIndex = i;
	neighbors[maxIndex].push_back(maxIndex);
	return neighbors[maxIndex];
}

int Labyrinth::is(int x,int y, int n)
{
	if (x >= 1 && x < width-1 && y >= 1 && y < height-1)
	{
		if (fields[x][y].type == n)
		{
				return 1; // equal n
		}
		return 0; // equal not n
	}
	return -1; // unavailable
}

int Labyrinth::isNeighborsNot(int parentX, int parentY,int x, int y, int n)
{
	std::vector<int> neighborsX = { 1, 0, -1,  0 };
	std::vector<int> neighborsY = { 0, 1,  0, -1 };
	while (neighborsX.size()>0)
	{
		int _rand = rand() % neighborsX.size();
		if (x+neighborsX[_rand] == parentX && y+neighborsY[_rand] == parentY)
			;
		else
			if (is(x + neighborsX[_rand], y + neighborsY[_rand], n) == 1)
				return 0;
		neighborsX.erase(neighborsX.begin() + _rand);
		neighborsY.erase(neighborsY.begin() + _rand);
	}
	return 1;
}

std::string Labyrinth::print()
{
	char wall = 'X';
	char meta = 'O';
	char room = ' ';
	char item = 'I';

	std::stringstream _return;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			switch(fields[j][i].type)
			{
			case 0:
				_return << wall;
				break;
			case 1:
				_return << room;
				break;
			case 2:
				_return << item;
				break;
			case 5:
				_return << meta;
				break;
			}
		}
		_return << "\n";
	}
	_return << '\n';
	system("cls");
	return _return.str();
}

Labyrinth::~Labyrinth()
{
	for (int i = 0; i < width; i++)
	{
		if (fields[i]) delete[] fields[i];
	}
	if (fields)delete[] fields;
	shortestWayToFinish.clear();
}