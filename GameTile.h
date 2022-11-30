#pragma once
#include "Image_Loading/nvImage.h"

class GameTile
{
private:
	
public:
	float Xpos;
	float Ypos;
	double texX1;
	double texX2;
	double texY1;
	double texY2;
	float size;
	double texSizeX;
	double texSizeY;
	int tileID;
	int collidableTiles[13] = {-1,1,2,3,4,5,17,33,36,37,38,49,52};
	bool collidable;

	GameTile();
	//initialises the attributes and works out the texture coords based on the tileID
	GameTile(float _Xpos, float _Ypos,double _texSizeX, double _texSizeY, int _tileID);
	//draws the tile
	void draw();
};

