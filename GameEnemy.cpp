#include "GameEnemy.h"

GameEnemy::GameEnemy()
{
}

GameEnemy::GameEnemy(float _Xpos, float _Ypos, float _size, char* img, Level& level) {
	spriteSize = _size;
	//sets the sprite position based on the world position
	spritePosX = _Xpos - spriteSize / 2.8;
	spritePosY = _Ypos - spriteSize / 3.85;
	textureX = 1.0;
	textureY = 10.0;
	
	vertical = true;
	horizontal = true;
	deltaTime = 0.0f;
	loadPNG(img);
	Xvelocity = 0.0;
	Yvelocity = 0.0;
	acceleration = 4.5;
	MAX_Velocity = 4.8;
	worldPosX = _Xpos;
	worldPosY = _Ypos;
	worldPosXPast = 0.0;
	worldPosYPast = 0.0;
	worldSizeX = spriteSize / 3.5;
	worldSizeY = spriteSize / 4;

	//generates a list of waypoints to the level exit
	newPath(level);
}

void GameEnemy::updateMovment(GameCharacter& otherCharacter, Level& level, double deltaTime)
{
	bool up, down, left, right;
	//checks to make sure there are more waypoints to vist
	if (coordQueue.size() > 0) {
		//checks to see if the enemy was in the same position as last frame
		if (worldPosX == worldPosXPast && worldPosY == worldPosYPast) {
			stuckTimer += deltaTime;
		}
		//update the past coordinates
		worldPosXPast = worldPosX;
		worldPosYPast = worldPosY;

		//checks to see if the enemy has been in the same position for a second or more and is not already stuck
		if (stuckTimer > 1 && stuck==false) {
			stuck = true;
		}
		if (stuck){
			//tries to align the enemy to the tile it is currently on
			getUnStuck(otherCharacter,level,deltaTime);
			stuckTimer = 0.0;
		}
		else {		
			//sets the current way point to the next one in the queue
			wayPoint current = coordQueue.back();
			//checks to see if the enemy is at the current way point
			if ((int)worldPosX == current.x && (int)worldPosY == current.y) {
				//removes the last waypoint and update current waypoint to the next one
				coordQueue.pop_back();
				current = coordQueue.back();
			}

			//updates the x axis inputs
			if ((int)worldPosX != current.x) {
				if (worldPosX < current.x) {
					left = false;
					right = true;
				}
				else {
					left = true;
					right = false;
				}
			}
			else {
				left = false;
				right = false;
			}

			//updates the y axis inputs
			if ((int)worldPosY != current.y) {
				if (worldPosY < current.y) {
					down = false;
					up = true;
				}
				else {
					down = true;
					up = false;
				}
			}
			else {
				down = false;
				up = false;
			}

			//updates the position of the enemy based on the calculated positions
			GameCharacter::updateMovment(otherCharacter, level, up, down, left, right, deltaTime);
			//updates the sprite position
			updatePosition();
		}
	}
	else {
		//updates the position of the enemy as if it were not moving
		GameCharacter::updateMovment(otherCharacter, level, false, false, false, false, deltaTime);
		//updates the sprite position
		updatePosition();
	}
}

void GameEnemy::updatePosition()
{
	//sets the sprite position based on the world position
	spritePosX = worldPosX - spriteSize / 2.8;
	spritePosY = worldPosY - spriteSize / 3.85;
}

void GameEnemy::getUnStuck(GameCharacter& otherCharacter, Level& level, double deltaTime)
{
	//checks if the enemy has reached the bottom left of the tile
	if ((worldPosX==(int)worldPosX || worldPosX < ((int)worldPosX)+0.1) && (worldPosY == (int)worldPosY || worldPosY < ((int)worldPosY) + 0.1)) {
		//sets the enemy to no longer stuck
		stuck = false;
	}
	else {
		//caulates inputs
		bool up = false;
		bool down = false;
		bool right = false;
		bool left = false;
		if (!(worldPosX == (int)worldPosX || worldPosX < ((int)worldPosX) + 0.1)) {
			left = true;
		}
		if (!(worldPosY == (int)worldPosY || worldPosY < ((int)worldPosY) + 0.1)) {
			down = true;
		}

		//updates the position of the enemy based on the calculated positions
		GameCharacter::updateMovment(otherCharacter, level, up, down, left, right, deltaTime);
		//updates the sprite position
		updatePosition();
	}
}

void GameEnemy::reset(float _posX, float _posY,Level& level)
{
	//sets the enemys values to the default
	GameCharacter::reset(_posX, _posY);
	updatePosition();
	//gets a new path to the end of the level
	newPath(level);

}

void GameEnemy::newPath(Level& level)
{
	//generates a new list of coordinates to the level's exit
	stuckTimer = 0.0;
	stuck = false;
	exitX = level.levelExit.posX;
	exitY = level.levelExit.posY;
	pathFinder = PathFinder(level);
	wayPoint start = { worldPosX, worldPosY };
	wayPoint end = { exitX, exitY };
	coordQueue = pathFinder.getPath(start, end);
}

