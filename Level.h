#pragma once
#include <vector>
#include "Image_Loading/nvImage.h"
#include <windows.h>
#include "GameTile.h"
#include "TriggerBox.h"

class Level
{
public:
	
	//2d array of tiles
	std::vector<std::vector<GameTile*>> tiles;

	GLuint texture;
	double sizeX;
	double sizeY;
	TriggerBox levelExit;
	bool debug = false;

	//generate a default menu level
	Level();	
	//loads the texture atlas for the level, loads the level from a csv
	//fills the tile array with the correct tiles
	Level(char* levelName, int exitX, int exitY);
	//loads the texture atlas for the level
	void LoadTextureAtlas();
	//loads the coordinates for each room from a file and generates 
	//a 2d array of tile IDs
	std::vector<std::vector<int>> buildLevel(char* levelName);
	//creates the tile objects from the 2d array of tile IDs
	void generateLevel(char* levelName, int exitX, int exitY);
	//creates the tiles for the menu screen level
	void generateMenuLevel();
	//binds the texture atlas for the level, then draws each of the 
	//tiles and the level exit
	void drawLevel();
	//binds the texture atlas for the level, then draws only the tiles 
	//that are visable to the player, and the level exit
	void drawLevel(float posX, float posY);
	//sets the debug attribute
	void setDebug(bool _debug);
	
};

