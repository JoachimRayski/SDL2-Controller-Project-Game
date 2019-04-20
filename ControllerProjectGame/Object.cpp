#include "Object.h"

Object::Object(SDL_Renderer * renderer, SDL_Rect  arg_rect, const char * image_path)
{
	image = SDL_LoadBMP(image_path);
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 255, 0));
	texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	rect = new SDL_Rect(arg_rect);
	center = new SDL_Point{ rect->h / 2, rect->w / 2 };
}

Object::~Object()
{
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image);
}

void Object::Render(SDL_Renderer * renderer, int rotation)
{
	SDL_RenderCopyEx(renderer, texture, NULL, rect, rotation, center, SDL_FLIP_NONE);
}

void Object::Move(int speedX, int speedY, int windowWidth, int windowHeight)
{
	//Correct horizontal position
	if (rect->x > windowWidth - rect->w / 2)
		rect->x = -rect->w/2;
	else if (rect->x < -rect->w/2)
		rect->x = windowWidth - rect->w / 2;
	//Correct vertical position
	if (rect->y > windowHeight - rect->h / 2)
		rect->y = -rect->h/2;
	else if (rect->y < -rect->h/2)
		rect->y = windowHeight - rect->h / 2;

	//Apply movement
	rect->x += speedX;
	rect->y += speedY;
}
