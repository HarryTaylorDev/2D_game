#include "GameTile.h"

GameTile::GameTile()
{
}

GameTile::GameTile(float _Xpos, float _Ypos, double _texSizeX, double _texSizeY, int _tileID)
{
	Xpos = _Xpos;
	Ypos = _Ypos;
	size = 1;
	//sets the size of one tile in the x and then y
	texSizeX = 16.0 / _texSizeX;
	texSizeY = 16.0 / _texSizeY;
	tileID = _tileID;
	//calculates the texture coordinates from the ID
	int countX = tileID;
	int countY = 16;
	while (countX > 16) {
		countX -=16;
		countY--;
	}
	texX1 = texSizeX * (countX - 1);
	texX2 = texSizeX * countX;
	texY1 = texSizeY * (countY - 1);
	texY2 = texSizeY * countY;

	//sets collidable to true if the tileID is in the list of tiles that are collidable
	collidable = std::find(std::begin(collidableTiles), std::end(collidableTiles), tileID) != std::end(collidableTiles);
}

void GameTile::draw()
{
	//draw the tile
	glBegin(GL_QUADS);
		glTexCoord2f(texX1, texY1); glVertex2f(Xpos, Ypos);
		glTexCoord2f(texX1, texY2); glVertex2f(Xpos, Ypos + size);
		glTexCoord2f(texX2, texY2); glVertex2f(Xpos + size, Ypos + size);
		glTexCoord2f(texX2, texY1); glVertex2f(Xpos + size, Ypos);
	glEnd();
}
