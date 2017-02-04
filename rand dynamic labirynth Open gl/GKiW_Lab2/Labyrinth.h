#pragma once
#include <vector>
#include <iostream>
#include <sstream>

typedef struct field
{
	int type;
};

class Labyrinth
{
public:
	Labyrinth(int, int);
	~Labyrinth();
	std::string print();
	int getPoint(bool, int);
	int* Labyrinth::setItemLocation(int);
	int* GiveWallLocation();

private:
	field** fields;
	std::vector<int> shortestWayToFinish;
	int width;
	int height;

	void build_labyrinth(int,int,int, int); // labyrinth initial, take last build field indexes
	std::vector<int> findFarestRoom(int,int); 
	std::vector<int> findFarestRoomRec(int,int,int,int); 
	int is(int,int,int);
	int isNeighborsNot(int,int,int,int,int);
};

