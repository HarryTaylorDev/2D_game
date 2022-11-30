#include "Arrow.h"
#include <cmath>

Arrow::Arrow()
{
}

Arrow::Arrow(float _posX, float _posY, float _turn)
{
	posX = _posX;
	posY = _posY;
	turn = _turn;
	//sets the default size
	size = 2;
	//loads the sprite
	this->loadIMG();
}

void Arrow::loadIMG()
{
	// Texture loading object
	nv::Image img;

	// Return true on success
	if (img.loadImageFromFile("arrow.png"))
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
	}

	else
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);
}

void Arrow::draw()
{
	//used to draw the img around the origin
	float tempSize = size / 2;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	//translates the arrow to the top right corner
	glTranslatef(tempSize+15.5, tempSize+8, 0.0);
	//rotates the arrow in the z axis by the turn angle
	glRotatef(turn, 0, 0, 1);
	
	//draws the img around the origin
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(-tempSize, -tempSize);
		glTexCoord2f(0, 1); glVertex2f(-tempSize, tempSize);
		glTexCoord2f(1, 1); glVertex2f(tempSize, tempSize);
		glTexCoord2f(1, 0); glVertex2f(tempSize, -tempSize);
	glEnd();
}

void Arrow::updateTurn()
{
	//test method to update the turn
	turn += 1;
	if (turn > 360) {
		turn = 0;
	}
}

void Arrow::updateTurn(float aX, float aY, float bX, float bY)
{
	//works out the difference between the points
	float deltaX = bX - aX;
	float deltaY = bY - aY;
	float pi = 3.1415926;

	//works out the angle
	float angle = 180*atan(deltaY / deltaX)/pi;

	//corrects for each quadrant
	if (deltaX > 0 && deltaY > 0){
		turn = angle;
	}

	if (deltaX > 0 && deltaY < 0){
		turn = angle + 360;
	}

	if (deltaX < 0 && deltaY < 0){
		turn = angle + 180;
	}

	if (deltaX < 0 && deltaY > 0){
		turn = angle + 180;
	}
}
