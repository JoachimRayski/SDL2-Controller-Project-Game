#include "Cells.h"

Cells::Cells()
{
	cell_graphic_paths.push_back("image2.bmp");
	cell_graphic_paths.push_back("image3.bmp");
}


Cells::~Cells()
{
	for (unsigned int i = 0; i < cell_array.size(); i++)
	{
		delete cell_array[i];
	}
}

void Cells::SpawnCells(int num_of_cells, int cell_type, SDL_Renderer * renderer, int windowWidth, int windowHeight)
{
	for (int i = 0; i < num_of_cells; i++)
	{
		const char * cell_type_image;
		if (cell_type <= -1)
		{
			if (i >= num_of_cells / 2)
			{
				normal_cells = FindCellType(0);
				virus_cells = FindCellType(1);
				if (normal_cells.size() < num_of_cells / 2)
				{
					cell_type_image = cell_graphic_paths[0];
				}
				else if (virus_cells.size() < num_of_cells / 3)
				{
					cell_type_image = cell_graphic_paths[1];
				}
				else
				{
					cell_type_image = cell_graphic_paths[rand() % cell_graphic_paths.size()];
				}
			}
			else
			{
				cell_type_image = cell_graphic_paths[rand() % cell_graphic_paths.size()];
			}
		}
		else if (cell_type < (int)cell_graphic_paths.size())
			cell_type_image = cell_graphic_paths[cell_type];
		else
			cell_type_image = cell_graphic_paths[0];

		Character * new_cell = new Character(renderer,(rand() % windowWidth) + 50, (rand() % windowHeight) + 50, cell_type_image);
		if (cell_type_image == cell_graphic_paths[0])
			new_cell->is_virus = false;
		else
			new_cell->is_virus = true;

		cell_array.push_back(new_cell);
	}
}

void Cells::RenderCells(SDL_Renderer * renderer, double rotation, SDL_Rect * player_rect, int windowWidth, int windowHeight, SerialPort * arduino)
{
	for (unsigned int i = 0; i < cell_array.size(); i++)
	{
		Character * cell = cell_array[i];
		int next_type;
		if (cell->is_destroyed)
		{
			if (cell->is_virus)
				next_type = 0;
			else
				next_type = 1;

			cell_array.erase(cell_array.begin() + i);
			delete cell;
			SpawnCells(1, next_type, renderer, windowWidth, windowHeight);
			normal_cells = FindCellType(0);
			virus_cells = FindCellType(1);
		}
		else
		{
			cell->Move(rand() % 5, rand() % 5, windowWidth, windowHeight);
			cell->Render(renderer, rotation);
			cell->DetectCollision(player_rect, arduino);
		}
	}
}

std::vector<Character*> Cells::FindCellType(int type)
{
	std::vector<Character*> found_cell_array;
	for (unsigned int i = 0; i < cell_array.size(); i++)
	{
		Character * cell = cell_array[i];
		if (cell->is_virus)
		{
			if(type == 1)
				found_cell_array.push_back(cell);
		}
		else
		{
			if (type == 0)
				found_cell_array.push_back(cell);
		}
	}
	return found_cell_array;
}
