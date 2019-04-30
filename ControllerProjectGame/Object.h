#pragma once
#include "stdafx.h"

class Object
{
public:
	Object();

	SDL_Surface * image;
	SDL_Texture * texture;
	SDL_Rect * rect;
	SDL_Point * center;
	void MakeImage(SDL_Renderer * renderer, int pos_x, int pos_y, const char * image_path);
	void Render(SDL_Renderer * renderer, double rotation);

};

