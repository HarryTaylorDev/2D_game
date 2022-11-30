#include "GameCharacter.h"
#include <windows.h>
#include <iostream>
#include "Level.h"

GameCharacter::GameCharacter()
{

}

GameCharacter::GameCharacter(float _Xpos, float _Ypos, float _size, char* img)
{
	//defualt position of the character
	spritePosX = 8.1f;
	spritePosY = 4.0f;
	//defualt position for the sprite sheet
	textureX = 1.0;
	textureY = 10.0;
	spriteSize = _size;
	vertical = true;
	horizontal = true;
	deltaTime = 0.0f;
	//load sprite sheet
	loadPNG(img);
	Xvelocity = 0.0;
	Yvelocity = 0.0;
	acceleration = 3.2;
	MAX_Velocity = 5.0;
	worldPosX = _Xpos;
	worldPosY = _Ypos;
	worldSizeX = spriteSize / 3.5;
	worldSizeY = spriteSize / 4;

	//debug off by defalut
	debug = false;
}

void GameCharacter::draw(double deltaTime)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
	//draws the character faceing left if velocity is negative on x axis
	if (Xvelocity < -0.01) {
		glTexCoord2f(textureSizeX * textureX, textureSizeY * (textureY - 1));			glVertex2f(spritePosX, spritePosY);
		glTexCoord2f(textureSizeX * textureX, textureSizeY * textureY);					glVertex2f(spritePosX, spritePosY + spriteSize);
		glTexCoord2f(textureSizeX * (textureX - 1), textureSizeY * textureY);			glVertex2f(spritePosX + spriteSize, spritePosY + spriteSize);
		glTexCoord2f(textureSizeX * (textureX - 1), textureSizeY * (textureY - 1));		glVertex2f(spritePosX + spriteSize, spritePosY);
	}
	//draws the character faceing right if velocity is positive on x axis
	else {
		glTexCoord2f(textureSizeX * (textureX - 1), textureSizeY * (textureY - 1));		glVertex2f(spritePosX, spritePosY);
		glTexCoord2f(textureSizeX * (textureX - 1), textureSizeY * textureY);			glVertex2f(spritePosX, spritePosY + spriteSize);
		glTexCoord2f(textureSizeX * textureX, textureSizeY * textureY);					glVertex2f(spritePosX + spriteSize, spritePosY + spriteSize);
		glTexCoord2f(textureSizeX * textureX, textureSizeY * (textureY - 1));			glVertex2f(spritePosX + spriteSize, spritePosY);
	}
	glEnd();

	//draw the debug boxes around the character
	//green for the sprite area and pink for the bounding box
	if (debug) {
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);

		glLineWidth(3);
		glBegin(GL_LINE_LOOP);
		glColor3f(0.0, 1.0, 0.0); glVertex2f(spritePosX, spritePosY);
		glColor3f(0.0, 1.0, 0.0); glVertex2f(spritePosX, spritePosY + spriteSize);
		glColor3f(0.0, 1.0, 0.0); glVertex2f(spritePosX + spriteSize, spritePosY + spriteSize);
		glColor3f(0.0, 1.0, 0.0); glVertex2f(spritePosX + spriteSize, spritePosY);
		glEnd();
		glColor3f(1.0, 1.0, 1.0);

		float tempX = spritePosX + spriteSize / 2.8;
		float tempY = spritePosY + spriteSize / 3.85;
		float tempSizeX = spriteSize / 3.5;
		float tempSizeY = spriteSize / 4;

		glLineWidth(3);
		glBegin(GL_LINE_LOOP);
		glColor3f(1.0, 0.0, 1.0); glVertex2f(tempX, tempY);
		glColor3f(1.0, 0.0, 1.0); glVertex2f(tempX, tempY + tempSizeY);
		glColor3f(1.0, 0.0, 1.0); glVertex2f(tempX + tempSizeX, tempY + tempSizeY);
		glColor3f(1.0, 0.0, 1.0); glVertex2f(tempX + tempSizeX, tempY);
		glEnd();
		glColor3f(1.0, 1.0, 1.0);

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	//updates the sprite for the character
	updateTexture(deltaTime);
}

void GameCharacter::loadPNG(char* name)
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
		textureSizeX = 48.0f / img.getWidth();
		textureSizeY = 48.0f / img.getHeight();

	}

	else
		MessageBox(NULL, "Failed to load texture", "End of the world", MB_OK | MB_ICONINFORMATION);
}

void GameCharacter::updateTexture(float _deltaTime)
{
	//updates the deltaTime of the character
	deltaTime += _deltaTime;
	//checks if the character is idle, true if it is idle
	if ((Xvelocity > -0.05 && Xvelocity < 0.05) && (Yvelocity > -0.05 && Yvelocity < 0.05)) {
		//for each 0.1 seconds updates the sprite
		while (deltaTime > 0.100f) {
			deltaTime -= 0.100f;
			//moves to the next frame of the idle animation
			textureX = textureX + 1;
			//sets it back to the first frame
			if (textureX > 8) {
				textureX = 1;
			}
		}
		//sets the frame for if the charater is walking up or down
		if (vertical) {
			textureY = 10;
		}
		else {
			textureY = 9;
		}
	}
	else {
		//for each 0.1 seconds updates the sprite
		while (deltaTime > 0.100f) {
			deltaTime -= 0.100f;
			//moves to the next frame of the idle animation
			textureX = textureX + 1;
			//sets it back to the first frame
			if (textureX > 6) {
				textureX = 1;
			}
		}
		//needed if a frame took less than 0.1 seconds and last frame 
		//was an idle frame so the x tex coord is greater than 6
		if (textureX > 6) {
			textureX = 1;
		}

		//sets the frame for if the charater is walking up or down
		if (vertical) {
			textureY = 8;
		}
		else {
			textureY = 7;
		}
	}
}

//Function updates the X velocity depending on if the a or d keys are pressed
float GameCharacter::calcXVelocity(bool left, bool right, float _deltaTime)
{
	//temp velocity value
	float tempVelocity = Xvelocity;
	if (left) {
		//updates the temp velocity by the acceleration
		if (tempVelocity < 0)
		{
			tempVelocity -= acceleration * _deltaTime;
		}
		//updates the temp velocity by the acceleration times a deceleration factor
		else {
			tempVelocity -= 3.5f * acceleration * _deltaTime;
		}

		//limits velocity to the maximum value
		if (tempVelocity < -MAX_Velocity) {
			tempVelocity = -MAX_Velocity;
		}
	}
	if (right) {
		//updates the temp velocity by the acceleration
		if (tempVelocity > 0)
		{
			tempVelocity += acceleration * _deltaTime;
		}
		//updates the temp velocity by the acceleration times a deceleration factor
		else {
			tempVelocity += 3.5f * acceleration * _deltaTime;
		}

		//limits velocity to the maximum value
		if (tempVelocity > MAX_Velocity) {
			tempVelocity = MAX_Velocity;
		}
	}
	//decelerates the charcter if no keys are being pressed 
	if (!left && !right) {
		if (tempVelocity < 0.0) {
			tempVelocity += 3.5f * acceleration * _deltaTime;
			if (tempVelocity > 0.0) {
				tempVelocity = 0.0;
			}
		}
		else if (tempVelocity > 0.0) {
			tempVelocity -= 3.5f * acceleration * _deltaTime;
			if (tempVelocity < 0.0) {
				tempVelocity = 0.0;
			}
		}
	}
	return tempVelocity;
}

//Function updates the Y velocity depending on if the w or s keys are pressed
float GameCharacter::calcYVelocity(bool down, bool up, float _deltaTime)
{
	//temp velocity value
	float tempVelocity = Yvelocity;
	if (down) {
		//updates the temp velocity by the acceleration
		if (Yvelocity < 0)
		{
			tempVelocity -= acceleration * _deltaTime;
		}
		//updates the temp velocity by the acceleration times a deceleration factor
		else {
			tempVelocity -= 3.5f * acceleration * _deltaTime;
		}

		//limits velocity to the maximum value
		if (tempVelocity < -MAX_Velocity) {
			tempVelocity = -MAX_Velocity;
		}
	}
	if (up) {
		//updates the temp velocity by the acceleration
		if (tempVelocity > 0)
		{
			tempVelocity += acceleration * _deltaTime;
		}
		//updates the temp velocity by the acceleration times a deceleration factor
		else {
			tempVelocity += 3.5f * acceleration * _deltaTime;
		}

		//limits velocity to the maximum value
		if (tempVelocity > MAX_Velocity) {
			tempVelocity = MAX_Velocity;
		}
	}
	//decelerates the charcter if no keys are being pressed 
	if (!up && !down) {
		if (tempVelocity < 0.0) {
			tempVelocity += 3.5f * acceleration * _deltaTime;
			if (tempVelocity > 0.0) {
				tempVelocity = 0.0;
			}
		}
		else if (tempVelocity > 0.0) {
			tempVelocity -= 3.5f * acceleration * _deltaTime;
			if (tempVelocity < 0.0) {
				tempVelocity = 0.0;
			}
		}
	}


	return tempVelocity;
}

void GameCharacter::updateXvelocity(float velocity)
{
	//updates the x velocity
	Xvelocity = velocity;
}

void GameCharacter::updateYvelocity(float velocity)
{
	//updates the y velocity
	Yvelocity = velocity;
	if (Yvelocity > 0.01) {
		vertical = false;
	}
	else {
		vertical = true;
	}
}

void GameCharacter::updateMovment(GameCharacter & otherCharacter,Level& level, bool up, bool down, bool left, bool right, double deltaTime)
{
	//calculates how far the character would move in X 
	float xMov = calcXVelocity(left, right, deltaTime);
	//saves distance
	float uvx = xMov;
	//updates the distance for the frame time
	xMov *= deltaTime;

	//calculates how far the character would move in X 
	float yMov = calcYVelocity(down, up, deltaTime);
	//saves distance
	float uvy = yMov;
	//updates the distance for the frame time
	yMov *= deltaTime;

	//sets the collide tests values to their defualt
	bool xCollide = false;
	bool yCollide = false;
	
	//checks for a collision with another character
	if (colliding(otherCharacter, xMov, 0) || colliding(otherCharacter, 0, yMov)) {

		//sets the current x velocity to the opposite with a base of 2.2, excluding 0
		if (Xvelocity > 2.2f || Xvelocity < -2.2f) {
			updateXvelocity(-Xvelocity);
		}
		else if (Xvelocity != 0.0) {
			if (Xvelocity > 0) {
				updateXvelocity(-2.2f);
			}
			else {
				updateXvelocity(2.2f);
			}
		}

		//sets the current x velocity to the opposite with a base of 2.2, excluding 0
		if (Yvelocity > 2.2f || Yvelocity < -2.2f) {
			updateYvelocity(-Yvelocity);
		}
		else if (Yvelocity!=0.0){
			if (Yvelocity > 0) {
				updateYvelocity(-2.2f);
			}
			else {
				updateYvelocity(2.2f);
			}
		}

		//re-calculates how far the character would move in X and Y 
		xMov = calcXVelocity(false, false, deltaTime);
		uvx = xMov;
		xMov *= deltaTime;
		yMov = calcYVelocity(false, false, deltaTime);
		uvy = yMov;
		yMov *= deltaTime;
	}

	//checks for collision in X
	if (colliding(level,xMov,0)) {
		xCollide = true;
	}

	//checks for collision in Y
	if (colliding(level,0,yMov)) {
		yCollide = true;
	}

	//If no collision update both x and y position and update velocities
	if (!xCollide && !yCollide) {
		worldPosY += yMov;
		updateYvelocity(uvy);
		worldPosX += xMov;
		updateXvelocity(uvx);
	}
	//collision in at least one axis
	else {
		//decelerate velocity and calulate how far would be moved
		uvx = calcXVelocity(false, false, deltaTime);
		uvy = calcYVelocity(false, false, deltaTime);

		//if collision in both axis skip ahead
		if (!(xCollide && yCollide)) {
			if (xCollide) {
				//move characters position in y
				yMov = uvy * deltaTime;
				worldPosY += yMov;
			}
			if (yCollide) {
				//move characters position in x
				xMov = uvx * deltaTime;
				worldPosX += xMov;
			}
		}

		//set characters velocity
		updateXvelocity(uvx);
		updateYvelocity(uvy);
	}
}

void GameCharacter::reset(float _Xpos, float _Ypos)
{
	//sets the characters position
	worldPosX = _Xpos;
	worldPosY = _Ypos;
	//sets the velocity to 0 in both axes
	Xvelocity = 0.0;
	Yvelocity = 0.0;
}

float GameCharacter::distance(Level& level)
{
	float dx = worldPosX - level.levelExit.posX;
	float dy = worldPosY - level.levelExit.posY;
	//returns the square of the distance as i only need to know if is bigger or 
	//smaller than another distance which will also be squared
	return dx*dx + dy*dy;
}

bool GameCharacter::colliding(TriggerBox& triggerBox, float xMov, float yMov)
{
	//Does an AABB test to check if chracter is colliding with a trigger box
	if (triggerBox.posX						< worldPosX + xMov + worldSizeX &&
		triggerBox.posX + triggerBox.sizeX	> worldPosX + xMov &&
		triggerBox.posY						< worldPosY + yMov + worldSizeY &&
		triggerBox.posY + triggerBox.sizeY	> worldPosY + yMov)
	{
		return true;
	}
	return false;
}

bool GameCharacter::colliding(Level& level, float xMov, float yMov)
{
	//temporary updated position
	float posX = worldPosX + xMov;
	float posY = worldPosY + yMov;

	//truncates the positions
	int x = (int)posX;
	int y = level.tiles.size() - ((int)posY + 1);

	//finds the coordinate of the first tile on the left and the right of the characters bounding box
	int xMin = (x > 0) ? (x - 1) : (0);
	int xMax = ((x +worldSizeX+ 1) < level.tiles[0].size() - 1) ? (x +worldSizeX+ 1) : (level.tiles[0].size() - 1);

	//finds the coordinate of the first tile on the top and bottom of the characters bounding box
	int yMin = (y > 0) ? (y - 1) : (0);
	int yMax = ((y + worldSizeX + 1) < level.tiles.size() - 1) ? (y + worldSizeX + 1) : (level.tiles.size() - 1);

	//Does an AABB test to check if chracter is colliding with a each of the tiles in the range
	for (int i = xMin; i < xMax + 1; i++) {
		for (int j = yMin; j < yMax + 1; j++) {
			if (level.tiles[j][i]->collidable) {
				if (level.tiles[j][i]->Xpos		< posX +worldSizeX&&
					level.tiles[j][i]->Xpos + 1	> posX &&
					level.tiles[j][i]->Ypos		< posY + worldSizeX &&
					level.tiles[j][i]->Ypos + 1	> posY)
				{
					//exits as soon as a colliding tile has been found
					return true;
				}
			}
		}
	}

	return false;
}

bool GameCharacter::colliding(GameCharacter& otherCharacter, float xMov, float yMov)
{
	//Does an AABB test to check if chracter is colliding with another character
	if (otherCharacter.worldPosX								< worldPosX + xMov + worldSizeX &&
		otherCharacter.worldPosX + otherCharacter.worldSizeX	> worldPosX + xMov &&
		otherCharacter.worldPosY								< worldPosY + yMov + worldSizeY &&
		otherCharacter.worldPosY + otherCharacter.worldSizeY	> worldPosY + yMov)
	{
		return true;
	}
	return false;
}

