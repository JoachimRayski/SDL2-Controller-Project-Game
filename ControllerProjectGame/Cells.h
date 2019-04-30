#pragma once
#include "stdafx.h"
#include "Character.h"

class Cells
{
public:
	Cells();
	~Cells();

	std::vector< const char * > cell_graphic_paths;
	std::vector< Character * > cell_array;

	void SpawnCells(int num_of_cells, int cell_type, SDL_Renderer * renderer, int windowWidth, int windowHeight);
	void RenderCells(SDL_Renderer * renderer, double rotation, SDL_Rect * player_rect, int windowWidth, int windowHeight, SerialPort * arduino);
	std::vector< Character * > FindCellType(int type);
	std::vector< Character * > normal_cells;
	std::vector< Character * > virus_cells;
};

