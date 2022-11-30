#pragma once
#include "Image_Loading/nvImage.h"
#include <windows.h>
class Arrow
{
public:
	float posX;
	float posY;
	float turn;
	float size;
	GLuint texture;

	Arrow();
	Arrow(float _posX, float _posY, float _turn);

	//loads the sprite for the arrow
	void loadIMG();

	//draws the arrow to the screen
	void draw();

	//test function, updates the turn angle by set amount
	void updateTurn();

	//sets the turn angle to the angle between two points
	void updateTurn(float aX, float aY, float bX, float bY);
};

