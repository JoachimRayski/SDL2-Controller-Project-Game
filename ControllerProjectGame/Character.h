#pragma once
#include "Object.h"

class Character :
	public Object
{
public:
	Character(SDL_Renderer * renderer, int pos_x, int pos_y, const char * image_path);
	~Character();

	bool is_destroyed;
	bool is_virus;
	void Move(double speedX, double speedY, int windowWidth, int windowHeight);
	void DetectCollision(SDL_Rect * target_rect, SerialPort * arduino);

private:
	double currentSpeedX;
	double currentSpeedY;
};