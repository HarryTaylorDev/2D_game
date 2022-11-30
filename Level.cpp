#include "Level.h"
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <regex>
#include <stdlib.h> 
#include <time.h> 

Level::Level()
{
	//load the texture atlas and generate the tile for the menu level
	this->LoadTextureAtlas();
	this->generateMenuLevel();
}

Level::Level(char* levelName, int exitX, int exitY)
{
	//load the texture atlas and generate the tile
	this->LoadTextureAtlas();
	this->generateLevel(levelName, exitX, exitY);
}

void Level::LoadTextureAtlas()
{

	// Texture loading object
	nv::Image img;

	// Return true on success
	if (img.loadImageFromFile("worldTiles.png"))
	{
		glGenTextures(1, &this->texture);
		glBindTexture(GL_TEXTURE_2D, this->texture);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
		sizeX = img.getWidth();
		sizeY = img.getHeight();
	} else
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);
}

std::vector<std::vector<int>> Level::buildLevel(char* levelName)
{
	//load the room coordinates from csv

	const std::regex comma(",");
	// Open source file.
	std::ifstream file(levelName);

	// Here we will store the result
	std::vector<std::vector<std::string>> roomCoords;

	// We want to read all lines of the file
	std::string line{};
	while (file && getline(file, line)) {
		// Tokenize the line && store result in vector. Use range constructor of std::vector
		std::vector<std::string> row{ std::sregex_token_iterator(line.begin(),line.end(),comma,-1), std::sregex_token_iterator() };
		roomCoords.push_back(row);
	}

	//generate the 2d vector of tileIDs defaulting to -1
	std::vector<std::vector<int>> tileIDs;
	int size = 100;
	for (int i = 0; i < size; i++) {
		tileIDs.push_back({});
		for (int j = 0; j < size; j++)
		{
			tileIDs[i].push_back(-1);
		}
	}
	
	//add the start and end room coordinates
	roomCoords.push_back({ "3", "98", "11", "90" });
	roomCoords.push_back({"90", "11", "98", "3"});

	//place floor tiles in to tileId array
	for (std::vector<std::string> room : roomCoords) {
		int x = std::stoi(room[0]);
		int y = std::stoi(room[1]);
		int xB = std::stoi(room[2]);
		int yB = std::stoi(room[3]);

		x--;
		yB--;

		for (int i = yB; i < y; i++) {
			for (int j = x; j < xB; j++) {
				tileIDs[i][j] = 6;
			}
		}
	}

	int array[5] = { -1, 1, 4, 49, 52 };
	
	int* test;
	int* test2;

	//place corner wall tiles
	for (int y = 1; y < size-1;y++){
		for (int x = 1; x < size-1; x++) {
			test = std::find(std::begin(array), std::end(array), tileIDs[y + 1][x]);
			test2 = std::find(std::begin(array), std::end(array), tileIDs[y][x + 1]);
			if ((tileIDs[y][x] == -1 && test != std::end(array) && test2 != std::end(array) && tileIDs[y + 1][x + 1] == 6)
				|| (tileIDs[y][x] == -1 && tileIDs[y - 1][x] == 6 && tileIDs[y][x - 1] == 6 && tileIDs[y - 1][x - 1] == 6)) {
				tileIDs[y][x] = 1;
			}

			test = std::find(std::begin(array), std::end(array), tileIDs[y][x - 1]);
			test2 = std::find(std::begin(array), std::end(array), tileIDs[y + 1][x]);
			if ((tileIDs[y][x] == -1 && test != std::end(array) && test2 != std::end(array) && tileIDs[y + 1][x - 1] == 6)
				|| (tileIDs[y][x] == -1 && tileIDs[y - 1][x] == 6 && tileIDs[y][x + 1] == 6 && tileIDs[y - 1][x + 1] == 6)) {
				tileIDs[y][x] = 4;
			}

			test = std::find(std::begin(array), std::end(array), tileIDs[y - 1][x]);
			test2 = std::find(std::begin(array), std::end(array), tileIDs[y][x + 1]);
			if ((tileIDs[y][x] == -1 && test != std::end(array) && test2 != std::end(array) && tileIDs[y - 1][x + 1] == 6) \
				|| (tileIDs[y][x] == -1 && tileIDs[y][x - 1] == 6 && tileIDs[y + 1][x] == 6 && tileIDs[y + 1][x - 1] == 6)) {
				tileIDs[y][x] = 49;
			}

			test = std::find(std::begin(array), std::end(array), tileIDs[y - 1][x]);
			test2 = std::find(std::begin(array), std::end(array), tileIDs[y][x - 1]);
			if ((tileIDs[y][x] == -1 && test != std::end(array) && test2 != std::end(array) && tileIDs[y - 1][x - 1] == 6) \
				|| (tileIDs[y][x] == -1 && tileIDs[y + 1][x] == 6 && tileIDs[y][x + 1] == 6 && tileIDs[y + 1][x + 1] == 6)) {
				tileIDs[y][x] = 52;
			}
		}
	}

	//place walls
	for (int x =1;x<size-1;x++) {
		for (int y = 1; y < size-1; y++) {
			if ((tileIDs[y][x] == -1 && tileIDs[y + 1][x] == 6) || (tileIDs[y][x] == -1 && tileIDs[y - 1][x] == 6)) {
				tileIDs[y][x] = 3;
			}
			if ((tileIDs[y][x] == -1 && tileIDs[y][x + 1] == 6) || (tileIDs[y][x] == -1 && tileIDs[y][x - 1] == 6)) {
				tileIDs[y][x] = 33;
			}
		}
	}

	//seed random number generator
	srand(time(0));

	//change some of the floor and walls to cracked versions
	for (int x = 1; x < size-1; x++) {
		for (int y = 1; y < size-1; y++) {
			int randNum = (rand() % 1000) + 1;
			if (tileIDs[y][x] == 6) {
				if (randNum < 50) {
					tileIDs[y][x] = 23;
				}
				else if (randNum < 100) {
					tileIDs[y][x] = 7;
				}
			}
			if (tileIDs[y][x] == 3) {
				if (randNum < 80) {
					tileIDs[y][x] = 37;
				}
				else if (randNum < 160) {
					tileIDs[y][x] = 38;
				}
			}
			if (tileIDs[y][x] == 33) {
				if (randNum < 80) {
					tileIDs[y][x] = 5;
				}
				else if (randNum < 160) {
					tileIDs[y][x] = 36;
				}
			}
		}
	}

	return tileIDs;
}

void Level::generateLevel(char* levelName, int exitX, int exitY)
{
	//generate the tiles from the tileId vector
	std::vector<std::vector<int>> worldTiles = buildLevel(levelName);

	for (int y = 0; y < worldTiles.size(); y++) {
		tiles.push_back({});
		for (int x = 0; x < worldTiles[y].size(); x++) {			
			tiles[y].push_back(new GameTile(x, worldTiles.size() - (y + 1), sizeX, sizeY, worldTiles[y][x]));			
		}
	}

	//create the level exit
	levelExit = TriggerBox(exitX, exitY, 1, 1, GameTile(exitX, exitY, sizeX, sizeY, 119));
}


void Level::generateMenuLevel()
{
	//generate the menu level tiles
	std::vector<std::vector<int>> worldTiles = {
		{33,6,6,6,33},
		{33,6,6,6,33},
		{33,6,6,6,33},
		{33,6,6,6,33},
		{33,6,6,6,33},
		{33,6,6,6,33},
		{33,6,6,6,33},
		{33,6,6,6,33},
		{33,6,6,6,33},
		{33,6,6,6,33},
		{33,6,6,6,33}
	};

	for (int y = 0; y < worldTiles.size(); y++) {
		tiles.push_back({});
		for (int x = 0; x < worldTiles[y].size(); x++) {
			int test = worldTiles[y][x];
			tiles[y].push_back(new GameTile(x, worldTiles.size() - (y + 1), sizeX, sizeY, worldTiles[y][x]));
		}
	}
}

void Level::setDebug(bool _debug) {
	//set the debug bool
	debug = _debug;
}


void Level::drawLevel()
{

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, texture);

	//draw all the tiles in the array
	for (int y = 0; y < tiles.size(); y++) {
		for (int x = 0; x < tiles[0].size(); x++) {
			//dont draw the tile if its ID is -1
			if (tiles[y][x]->tileID != -1) {
				tiles[y][x]->draw();
			}
			//draw the debug tile outline
			if (debug) {
				glLineWidth(3);
				glBegin(GL_LINE_LOOP);
				glColor3f(0.0, 0.0, 0.0); glVertex2f(tiles[y][x]->Xpos, tiles[y][x]->Ypos);
				glColor3f(0.0, 0.0, 0.0); glVertex2f(tiles[y][x]->Xpos, tiles[y][x]->Ypos + 1);
				glColor3f(0.0, 0.0, 0.0); glVertex2f(tiles[y][x]->Xpos + 1, tiles[y][x]->Ypos + 1);
				glColor3f(0.0, 0.0, 0.0); glVertex2f(tiles[y][x]->Xpos + 1, tiles[y][x]->Ypos);
				glEnd();
				glColor3f(1.0, 1.0, 1.0);
			}
		}
	}
	
	//draw the level exit
	levelExit.drawTrigger();
}


void Level::drawLevel(float posX, float posY)
{
	//truncate position coordinates
	int x = (int)posX;
	int y = tiles.size() - ((int)posY + 1);

	int temp = tiles[0].size() - 11;

	//calculate the min and max tiles that can be seen on screen
	int xMin = (x > 11) ? (x - 10) : (0);
	int xMax = (x < temp) ? (x + 10) : (tiles[0].size() - 1);

	int yMin = (y > 8) ? (y - 7) : (0);
	int yMax = (y < (tiles.size() - 9)) ? (y + 7) : (tiles.size() - 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, texture);

	for (int y = yMin; y < yMax+1; y++) {
		for (int x = xMin; x < xMax+1; x++) {
			//draw tiles to screen
			if (tiles[y][x]->tileID != -1) {
				tiles[y][x]->draw();
			}
			//draw the debug tile outline
			if (debug) {
				glLineWidth(5);
				glBegin(GL_LINE_LOOP);
				glColor3f(0.0, 0.0, 0.0); glVertex2f(tiles[y][x]->Xpos, tiles[y][x]->Ypos);
				glColor3f(0.0, 0.0, 0.0); glVertex2f(tiles[y][x]->Xpos, tiles[y][x]->Ypos + 1);
				glColor3f(0.0, 0.0, 0.0); glVertex2f(tiles[y][x]->Xpos + 1, tiles[y][x]->Ypos + 1);
				glColor3f(0.0, 0.0, 0.0); glVertex2f(tiles[y][x]->Xpos + 1, tiles[y][x]->Ypos);
				glEnd();
				glColor3f(1.0, 1.0, 1.0);
			}
		}
	}

	//draw the level exit tile
	levelExit.drawTrigger();
}



