#pragma once
#include "charactor.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

const unsigned int tile_num = 24;
class Tiles
{
	GamesEngineeringBase::Image tile[tile_num];
	unsigned int tile_width, tile_height;
	std::string fileroot;

public:
	Tiles(std::string fn = "") : fileroot(fn), tile_width(0), tile_height(0) {};

	void tiles_init();

	void tiles_init(std::string fr);

	void draw(GamesEngineeringBase::Window& canvas, unsigned int id,
		unsigned int map_to_cam_x, unsigned int map_to_cam_y);

	GamesEngineeringBase::Image& operator[] (unsigned int index) { return tile[index]; }
};

class Map
{
	unsigned int tiles_width, tiles_height;
	unsigned int map_width, map_height;
	unsigned int* tiles;
public:
	Map() :
		tiles_width(0),
		tiles_height(0),
		map_width(0),
		map_height(0),
		tiles(nullptr) { }

	unsigned int& at(unsigned int x, unsigned int y) { return tiles[y * map_width + x]; }

	unsigned int get(unsigned int x, unsigned int y) { return tiles[y * map_width + x]; }

	void allocate()
	{
		tiles = new unsigned int[map_width * map_height];
	}

	void free_tiles()
	{
		delete[] tiles;
		tiles = nullptr;
	}

	bool load_map(const char* filename);


	bool load_map(const std::string& filename)
	{
		std::ifstream map_file(filename.c_str());
		if (!map_file.is_open())
			return false;
	}

	unsigned int get_map_width() const { return map_width; }

	unsigned int get_map_height() const { return map_height; }

	unsigned int get_tiles_width() const { return tiles_width; }

	unsigned int get_tiles_height() const { return tiles_height; }

	~Map()
	{
		free_tiles();
	}
};


