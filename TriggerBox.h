#pragma once

#include "Image_Loading/nvImage.h"
#include <windows.h>
#include "GameTile.h"

class TriggerBox
{

public:
	float posX;
	float posY;
	float sizeX;
	float sizeY;
	GameTile exitTile;

	TriggerBox();
	//creates the bounding box and sets the tile
;	TriggerBox(float _posX, float _posY, float _sizeX, float _sizeY, GameTile tile);
	//draws the tile
	void drawTrigger();

};

