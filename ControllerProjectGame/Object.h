#pragma once
#include "stdafx.h"

class Object
{
public:
	Object(SDL_Renderer * renderer, SDL_Rect arg_rect, const char* image_path);
	~Object();

	void Render(SDL_Renderer * renderer, int rotation);
	void Move(int speedX, int speedY, int windowWidth, int windowHeight);

private:
	SDL_Surface * image;
	SDL_Texture * texture;
	SDL_Rect * rect;
	SDL_Point * center;
};

