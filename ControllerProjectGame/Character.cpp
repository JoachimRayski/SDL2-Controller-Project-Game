#include "Character.h"



Character::Character(SDL_Renderer * renderer, int pos_x, int pos_y, const char * image_path)
{
	MakeImage(renderer, pos_x, pos_y, image_path);
	is_destroyed = false;
	currentSpeedX = 0;
	currentSpeedY = 0;
}


Character::~Character()
{
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image);
}

void Character::Move(double speedX, double speedY, int windowWidth, int windowHeight)
{
	if (!is_destroyed)
	{
		//Correct horizontal position
		if (rect->x > windowWidth - rect->w / 2)
			rect->x = -rect->w / 2;
		else if (rect->x < -rect->w / 2)
			rect->x = windowWidth - rect->w / 2;
		//Correct vertical position
		if (rect->y > windowHeight - rect->h / 2)
			rect->y = -rect->h / 2;
		else if (rect->y < -rect->h / 2)
			rect->y = windowHeight - rect->h / 2;

		//Smooth slow down
		if (currentSpeedX > 0.1)
			currentSpeedX -= 0.1;
		else if (currentSpeedX < -0.1)
			currentSpeedX += 0.1;
		else
			currentSpeedX = 0;

		if (currentSpeedY > 0.1)
			currentSpeedY -= 0.1;
		else if (currentSpeedY < -0.1)
			currentSpeedY += 0.1;
		else
			currentSpeedY = 0;

		//Move using the speed arguments
		if (speedX != 0)
			currentSpeedX = speedX;
		if (speedY != 0)
			currentSpeedY = speedY;

		//Apply movement
		rect->x += (int)currentSpeedX;
		rect->y += (int)currentSpeedY;
	}
}

void Character::DetectCollision(SDL_Rect * target_rect, SerialPort * arduino)
{
	SDL_bool collision = SDL_HasIntersection(rect, target_rect);
	if (collision)
	{
		is_destroyed = true;
		if (!is_virus && arduino->isConnected())
		{
			char message = 'M';
			arduino->writeSerialPort(&message, 1);
		}
	}
}