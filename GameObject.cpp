#include "GameObject.h"
#include "Image_Loading/nvImage.h"
#include <windows.h>



GameObject::GameObject()
{
	Xpos = 0.0;
	Ypos = 0.0;
	size = 1.0;
	loadPNG("test.png");
	texoffset = 0;
}

GameObject::GameObject(int _texX, int _texY, char* _filename)
{
	//Xpos = _Xpos;
	//Ypos = _Ypos;
	texX = _texX;
	texY = _texY;
	loadPNG(_filename);
	texoffset = 0;
}


void GameObject::draw(float Xpos, float Ypos, float size) {
	float sizex = 0.03703703703;
	float sizey = 0.1;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex2f(Xpos, Ypos);
		glTexCoord2f(0, 1); glVertex2f(Xpos, Ypos + size);
		glTexCoord2f(1, 1); glVertex2f(Xpos + size, Ypos + size);
		glTexCoord2f(1, 0); glVertex2f(Xpos + size, Ypos);
	glEnd();
}


void GameObject::loadPNG(char* name)
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

	else
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);
}

void GameObject::updateTexture(float _texoffset) {
	texoffset += _texoffset;
}