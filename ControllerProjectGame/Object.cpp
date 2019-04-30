#include "Object.h"

Object::Object()
{
}

void Object::MakeImage(SDL_Renderer * renderer, int pos_x, int pos_y, const char * image_path)
{
	image = SDL_LoadBMP(image_path);
	SDL_SetColorKey(image, SDL_TRUE, SDL_MapRGB(image->format, 0, 255, 0));
	texture = SDL_CreateTextureFromSurface(renderer, image);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	rect = new SDL_Rect({ pos_x - (w/2), pos_y - (h/2), w, h });
	center = new SDL_Point{ rect->w / 2, rect->h / 2 };
}

void Object::Render(SDL_Renderer * renderer, double rotation)
{
	SDL_RenderCopyEx(renderer, texture, NULL, rect, rotation, center, SDL_FLIP_NONE);
}