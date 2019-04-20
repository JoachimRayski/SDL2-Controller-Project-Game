#include "stdafx.h"
#include "Object.h"
//Target frame rate in frames per second (second/frames)
#define FRAME_RATE 1000/60
#define MOVEMENT_SPEED 5
//Stores the time remaining to the next frame
static Uint32 next_wait_time;

Uint32 time_left(void)
{
	/**
	Returns the time needed to wait before moving onto the next frame
	*/
	Uint32 current_ticks = SDL_GetTicks();

	if (next_wait_time <= current_ticks)
		return 0;
	else
		return next_wait_time - current_ticks;
}

void Start_Game(SDL_Window *window, SDL_Renderer *renderer)
{
	/**
	Initializes the game properties
	*/
	SDL_SetRenderDrawColor(renderer, 255, 127, 127, 255);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

int main(int argc, char **argv)
{
	//Initialize SDL utilities
	SDL_Init(SDL_INIT_EVERYTHING);
	int windowWidth = 640;
	int windowHeight = 480;
	SDL_Window *window = SDL_CreateWindow("ControllerProjectGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (window == NULL)
	{
		printf("could not create the window: %s\n", SDL_GetError());
		return 1;
	}
	if (renderer == NULL)
	{
		printf("could not create a renderer: %s\n", SDL_GetError());
		return -1;
	}

	Start_Game(window, renderer);
	Object * testObj = new Object(renderer, { 640 / 2 - 50 / 2, 480 / 2 - 50 / 2, 50, 50 }, "image1.bmp");
	const Uint8 * keystate;
	int speedX = 0;
	int speedY = 0;
	int rotation = 0;

	next_wait_time = SDL_GetTicks() + FRAME_RATE;
	while(true)
	{
		/**
		EVENT LOOP
		*/
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			//Cleanup on quit
			if (event.type == SDL_QUIT)
			{
				testObj->~Object();
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				SDL_Quit();
				break;
			}
		}
		/**
		INPUT LOOP
		*/
		keystate = SDL_GetKeyboardState(NULL);
		//Handles vertical input
		if (keystate[SDL_SCANCODE_W] && !keystate[SDL_SCANCODE_S])
		{
			speedY = -MOVEMENT_SPEED;
			if (keystate[SDL_SCANCODE_D])
				rotation = 45;
			else if (keystate[SDL_SCANCODE_A])
				rotation = -45;
			else
				rotation = 0;
		}
		else if (!keystate[SDL_SCANCODE_W] && keystate[SDL_SCANCODE_S])
		{
			speedY = MOVEMENT_SPEED;
			if (keystate[SDL_SCANCODE_D])
				rotation = 225;
			else if (keystate[SDL_SCANCODE_A])
				rotation = 135;
			else
				rotation = 180;
		}
		else
			speedY = 0;

		//Handles horizontal input
		if (keystate[SDL_SCANCODE_D] && !keystate[SDL_SCANCODE_A])
		{
			speedX = MOVEMENT_SPEED;
			if (keystate[SDL_SCANCODE_W])
				rotation = 45;
			else if (keystate[SDL_SCANCODE_S])
				rotation = 135;
			else
				rotation = 90;
		}
		else if (!keystate[SDL_SCANCODE_D] && keystate[SDL_SCANCODE_A])
		{
			speedX = -MOVEMENT_SPEED;
			if (keystate[SDL_SCANCODE_W])
				rotation = -45;
			else if (keystate[SDL_SCANCODE_S])
				rotation = -135;
			else
				rotation = -90;
		}
		else
			speedX = 0;

		/**
		GAME LOOP
		*/
		testObj->Move(speedX, speedY, windowWidth, windowHeight);

		//Update the renderer
		SDL_RenderClear(renderer);
		//RenderObjects
		testObj->Render(renderer, rotation);
		SDL_RenderPresent(renderer);
		//Synchronize the loop with the target framerate
		SDL_Delay(time_left());
		next_wait_time += FRAME_RATE;
	}
	return 0;
}