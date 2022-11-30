#include "TriggerBox.h"

TriggerBox::TriggerBox()
{
}

TriggerBox::TriggerBox(float _posX, float _posY, float _sizeX, float _sizeY, GameTile tile)
{
	//initialises attributes
	posX = _posX;
	posY = _posY;
	sizeX = _sizeX;
	sizeY = _sizeY;
	exitTile = tile;
}

void TriggerBox::drawTrigger()
{
	//draws the tile
	exitTile.draw();
}

