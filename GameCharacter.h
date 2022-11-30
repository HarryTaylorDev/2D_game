#pragma once
#include "Image_Loading/nvImage.h"
#include "Level.h"
#include "TriggerBox.h"
class GameCharacter
{
private:

public:
	float deltaTime;
	bool debug;

	//sprite/texture attributes
	float spritePosX;
	float spritePosY;
	float spriteSize;

	float textureSizeX;
	float textureSizeY;
	int textureX;
	int textureY;

	GLuint texture;
	bool vertical;
	bool horizontal;

	//position/collider attributes
	float worldPosX;
	float worldPosY;
	float worldSizeX;
	float worldSizeY;

	//movement attributes
	float Xvelocity;
	float Yvelocity;
	float acceleration;
	float MAX_Velocity;

	GameCharacter();
	//set up defualt values and initializes position size and the sprite sheet
	GameCharacter(float _Xpos, float _Ypos, float _size, char* img);
	//draw the character
	void draw(double deltaTime);
	//load the characters sprite sheet
	void loadPNG(char* name);
	//animates the characters sprite based on their velocity
	void updateTexture(float deltaTime);
	//calculate how far the character would move based on currect velocity and keys pressed in x
	float calcXVelocity(bool left, bool right, float _deltaTime);
	//calculate how far the character would move based on currect velocity and keys pressed in y
	float calcYVelocity(bool down, bool up, float _deltaTime);
	//set the characters x velocity
	void updateXvelocity(float velocity);
	//set the characters y velocity
	void updateYvelocity(float velocity);
	//check for a collision between character and trigger area
	bool colliding(TriggerBox& triggerBox, float xMov, float yMov);
	//check for a collision between character and the levels walls, slows down on collision
	bool colliding(Level& level,float xMov, float yMov);
	//check for a collision between character and another character, will bounce off of the other character
	bool colliding(GameCharacter& otherCharacter, float xMov, float yMov);
	//update the characters position, depending on if there was a collision and keys being pressed
	void updateMovment(GameCharacter& otherCharacter, Level& level, bool up, bool down, bool left, bool right, double deltaTime);
	//resets the characters attributes to default
	void reset(float _Xpos, float _Ypos);
	//calculate the distance between the character and the level exit
	float distance(Level& level);
};

