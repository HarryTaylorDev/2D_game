#include "TextBox.h"

TextBox::TextBox()
{
}

TextBox::TextBox(float _posX, float _posY, float _sizeX, float _sizeY, char* name)
{
	// Texture loading object
	nv::Image img;

	// Return true on success
	if (img.loadImageFromFile(name))
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

	else {
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);
	}
	posX = _posX;
	posY = _posY;
	sizeX = _sizeX;
	sizeY = _sizeY;
}

TextBox::TextBox(float _posX, float _posY, float _sizeX, float _sizeY, char* name, float timer)
{
	// Texture loading object
	nv::Image img;

	// Return true on success
	if (img.loadImageFromFile(name))
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

	else {
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);
	}
	posX = _posX;
	posY = _posY;
	sizeX = _sizeX;
	sizeY = _sizeY;
	//set the timer and dead status
	this->timer = timer;
	dead = false;
}

void TextBox::drawText()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, texture);

	//draw the textbox
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(posX, posY);
		glTexCoord2f(0, 1); glVertex2f(posX, posY + sizeY);
		glTexCoord2f(1, 1); glVertex2f(posX + sizeX, posY + sizeY);
		glTexCoord2f(1, 0); glVertex2f(posX + sizeX, posY);
	glEnd();
}

bool TextBox::drawText(float deltaTime)
{
	glEnable(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//draw the text box
	drawText();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	//update the timer
	timer -= deltaTime;
	//set the text box to dead if it has no time left
	if (timer < 0) {
		dead = true;
		return true;
	}
	return false;
}
