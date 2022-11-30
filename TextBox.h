#pragma once
#include "Image_Loading/nvImage.h"
#include <windows.h>
#include <string>

class TextBox
{
public:
	float posX;
	float posY;
	float sizeX;
	float sizeY;
	GLuint texture;
	//time left alive
	float timer;
	//is there no time left, true if there is, flase other wise
	bool dead;

	TextBox();
	//creates a textbox with no timer for its life
	TextBox(float posX,float posY, float sizeX,float sizeY,char* name);
	//creates a textbox with a timer for its life
	TextBox(float posX, float posY, float sizeX, float sizeY, char* name,float _timer);
	//draws the textbox ignoring the timer
	void drawText();
	//draws the textbox ignoring the timer
	bool drawText(float deltaTime);
};

