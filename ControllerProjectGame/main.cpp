#include "stdafx.h"
#include "Cells.h"

//Constants
//Target frame rate in frames per second (second/frames)
#define FRAME_RATE 1000/60
#define MOVEMENT_SPEED 5
#define ROTATION_SPEED 6
const int windowWidth = 800;
const int windowHeight = 600;
SDL_Point window_center = { windowWidth / 2, windowHeight / 2 };
SDL_Window *window;
SDL_Renderer *renderer;

//Stores the time remaining to the next frame
static Uint32 next_wait_time;

//Stores the current keyboard input
const Uint8 * keystate;

//Arduino serial port data
SerialPort * arduino;
char output[MAX_DATA_LENGTH];
std::string port_input;
std::string port_name;
char incoming[MAX_DATA_LENGTH];
int read_data;
std::stringstream ss;
std::vector<std::string> potentiometer_value_strings;
int raw_num = 0;
double potentiometer = 0;
bool read_potentiometer = false;
int button = 1;
bool read_button = false;

//Audio data
Mix_Chunk * background_music;

//Object data
Cells * cells;
const int cell_population = 15;
Object * end_message;
Object * start_message;
Character * player;
double rotation_in_degrees = 0;
double rotation_in_radians = 0;
double speedX = 0;
double speedY = 0;

//Game states
bool end_game = false;
bool start_game = false;
bool start_inputs = false;

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

void Start_Game()
{
	/**
	Initializes the game properties
	*/

	::ShowWindow(::GetConsoleWindow(), SW_HIDE);

	//Background colour
	SDL_SetRenderDrawColor(renderer, 255, 127, 127, 255);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	//Clear the renderer
	SDL_RenderClear(renderer);

	//Start message setup
	start_message = new Object();
	start_message->MakeImage(renderer, windowWidth / 2, windowHeight / 2, "menuimg.bmp");
	start_message->Render(renderer, 0);

	//Player Setup
	player = new Character(renderer, window_center.x, window_center.y, "image1.bmp");

	//Enemies setup
	cells = new Cells();
	cells->SpawnCells(cell_population, -1, renderer, windowWidth, windowHeight);
	cells->normal_cells = cells->FindCellType(0);
	cells->virus_cells = cells->FindCellType(1);

	//End message setup
	end_message = new Object();

	//Initialize audio
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	background_music = Mix_LoadWAV("BearInsides.wav");
	Mix_PlayChannel(-1, background_music, -1);

	//Read arduino serial port data and initialize framerate synchronization
	read_data = arduino->readSerialPort(incoming, MAX_DATA_LENGTH);
	next_wait_time = SDL_GetTicks() + FRAME_RATE;
}

void Restart()
{
	keystate = NULL;
	potentiometer = 0;
	button = 1;
    rotation_in_degrees = 0;
	rotation_in_radians = 0;
	speedX = 0;
	speedY = 0;
	start_game = false;
	start_inputs = false;
	end_game = false;
	Sleep(1000);
	delete player;
	delete cells;
	Mix_CloseAudio();
	Mix_FreeChunk(background_music);
	Start_Game();
	if (arduino->isConnected())
	{
		char message = 'W';
		arduino->writeSerialPort(&message, 1);
	}
}

int main(int argc, char **argv)
{
	//Initialize SDL utilities
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	window = SDL_CreateWindow("ControllerProjectGame", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	::ShowWindow(::GetConsoleWindow(), SW_SHOW);

	//Initialize arduino serial port
	std::cout << "Enter your arduino controller serial port name: ";
	std::cin >> port_input;
	ss << "\\\.\\" << port_input;
	port_name = ss.str();

	arduino = new SerialPort(port_name.c_str());

	if (arduino->isConnected())
	{
		printf("CONNECTION ESTABLISHED");
	}
	else
	{
		printf("INVALID PORT NAME %s\n", SDL_GetError());
	}

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

	Sleep(1000);

	Start_Game();

	/**
	GAME LOOP
	*/
	while(true)
	{
		/**
		EVENTS
		*/
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				break;
			}
		}
		/**
		ARDUINO INPUT/OUTPUT
		*/
		if (arduino->isConnected())
		{
			read_data = arduino->readSerialPort(incoming, MAX_DATA_LENGTH);
			if (read_data)
			{
				for (unsigned int c = 0; c < strlen(incoming); c++)
				{
					unsigned char current_char = incoming[c];
					if (isdigit(current_char) || isalpha(current_char))
					{
						std::cout << current_char << std::endl;
						if (current_char == 'b')
						{
							read_button = true;
							read_potentiometer = false;
						}
						else if (current_char == 'p')
						{
							read_potentiometer = true;
							read_button = false;
							potentiometer_value_strings.clear();
							std::stringstream().swap(ss);
						}
						else if (isdigit(current_char))
						{
							if (read_button)
							{
								button = (int)current_char - 48;
								std::cout << "BUTTON: " << button << std::endl;

								for (unsigned int i = 0; i < potentiometer_value_strings.size(); i++)
									ss << potentiometer_value_strings[i];
								if (ss.str().length() > 0 && isdigit(ss.str().c_str()[0]))
								{
									raw_num = std::stoi(ss.str());
									potentiometer = raw_num / 1.4208 - 360;
									std::cout << "POTENTIOMETER: " << potentiometer << std::endl;
								}
							}
							else if (read_potentiometer)
							{
								std::string str = std::to_string((int)current_char - 48);
								potentiometer_value_strings.push_back(str);
							}
						}
					}
				}
			}
			else
				printf("ERROR OCCURED WHILE READING DATA \n");
		}

		/**
		KEYBOARD INPUT
		*/

		keystate = SDL_GetKeyboardState(NULL);

		//Start game inputs
		if (keystate[SDL_SCANCODE_SPACE] || button == 0)
		{
			start_game = true;
		}
		else if (start_game)
		{
			start_inputs = true;
		}

		//Gameplay inputs
		if (start_game)
		{
			if (keystate[SDL_SCANCODE_RIGHT])
			{
				rotation_in_degrees += ROTATION_SPEED;
				potentiometer = rotation_in_degrees;
			}
			if (keystate[SDL_SCANCODE_LEFT])
			{
				rotation_in_degrees -= ROTATION_SPEED;
				potentiometer = rotation_in_degrees;
			}
			if (!keystate[SDL_SCANCODE_RIGHT] && !keystate[SDL_SCANCODE_LEFT])
				rotation_in_degrees = potentiometer;

			if (start_inputs)
			{
				if (keystate[SDL_SCANCODE_SPACE] || button == 0)
				{
					rotation_in_radians = rotation_in_degrees * M_PI / 180;
					speedX = sin(rotation_in_radians) * MOVEMENT_SPEED;
					speedY = cos(rotation_in_radians) * -MOVEMENT_SPEED;
				}
				else if (!keystate[SDL_SCANCODE_SPACE] || button == 1)
				{
					speedX = 0;
					speedY = 0;
				}
			}
		}

		/**
		BEHAVIOUR AND RENDERING
		*/
		if (start_game)
		{
			//Game States
			if (cells->virus_cells.size() == 0)
			{
				if (!end_game)
				{
					Mix_HaltChannel(-1);
					end_message->MakeImage(renderer, windowWidth / 2, windowHeight / 2, "wonimg.bmp");
					end_message->Render(renderer, 0);
					end_game = true;
				}
			}
			else if (cells->normal_cells.size() < cell_population / 2)
			{
				if (!end_game)
				{
					Mix_HaltChannel(-1);
					end_message->MakeImage(renderer, windowWidth / 2, windowHeight / 2, "lostimg.bmp");
					end_message->Render(renderer, 0);
					end_game = true;
				}
			}
			else
			{
				//Clear the renderer
				SDL_RenderClear(renderer);
				player->Move(speedX, speedY, windowWidth, windowHeight);
				//RenderObjects
				player->Render(renderer, rotation_in_degrees);
				cells->RenderCells(renderer, 0, player->rect, windowWidth, windowHeight, arduino);
			}
		}
		//Update the renderer
		SDL_RenderPresent(renderer);
		if (end_game)
			Restart();
		//Synchronize the loop with the target framerate
		SDL_Delay(time_left());
		next_wait_time += FRAME_RATE;
	}
	//Cleanup
	delete player;
	delete cells;
	delete arduino;
	Mix_CloseAudio();
	Mix_FreeChunk(background_music);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}