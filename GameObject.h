#pragma once
#include "Image_Loading/nvImage.h"

class GameObject
{
public:
	float Xpos;
	float Ypos;
	int texX;
	int texY;
	float size;
	float texoffset;
	GLuint texture;

	GameObject();
	GameObject(int texX, int textY, char* _filename);
	void draw(float _Xpos, float _Ypos, float _size);
	void loadPNG(char* name);
	void updateTexture(float _texoffset);
};

