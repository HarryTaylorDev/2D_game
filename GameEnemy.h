#pragma once
#include "GameCharacter.h"
#include "PathFinder.h"
#include <queue>


class GameEnemy:public GameCharacter
{
public:
	//pathfinding attributes
	std::vector<wayPoint> coordQueue;
	PathFinder pathFinder;
	double stuckTimer;
	bool stuck;
	float worldPosXPast;
	float worldPosYPast;
	int exitX;
	int exitY;

	GameEnemy();
	GameEnemy(float _Xpos, float _Ypos, float _size, char* img, Level& level);
	//updates the coordinates based on current velocity and input based on the pathfinder
	void updateMovment(GameCharacter& otherCharacter, Level& level, double deltaTime);
	//sets the sprite position based on the world coordinates
	void updatePosition();
	//try to get the character unstuck / back on path for the way points
	void getUnStuck(GameCharacter& otherCharacter, Level& level, double deltaTime);
	//reset attributes and get new path to the new level exit
	void reset(float _posX, float _posY, Level& level);
	//get new path to level exit
	void newPath(Level& level);
	
};

