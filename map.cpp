#include "map.h"

void Tiles::tiles_init()
{
	for (unsigned int i = 0; i < tile_num; i++)
	{
		std::string filename;
		filename = "./Resource/" + fileroot + "/" + std::to_string(i) + ".png";
		tile[i].load(filename);
	}
	tile_width = tile[0].width;
	tile_height = tile[0].height;
}

void Tiles::tiles_init(std::string fr)
{
	fileroot = fr;
	for (unsigned int i = 0; i < tile_num; i++)
	{
		std::string filename;
		filename = "./Resource/" + fileroot + "/" + std::to_string(i) + ".png";
		tile[i].load(filename);
	}
	tile_width = tile[0].width;
	tile_height = tile[0].height;
}

void Tiles::draw(GamesEngineeringBase::Window& canvas, unsigned int id,
	unsigned int map_to_cam_x, unsigned int map_to_cam_y)
{
	for (unsigned int w = 0; w < tile_width; w++)
	{
		if (map_to_cam_x + w > 0 && map_to_cam_x + w < canvas.getWidth())
			for (unsigned int h = 0; h < tile_height; h++)
			{
				if (map_to_cam_y + h > 0 && map_to_cam_y + h < canvas.getHeight())
				{
					//std::cout << id << std::endl;
					canvas.draw(map_to_cam_x + w, map_to_cam_y + h, tile[id].atUnchecked(w, h));
				}
			}
	}
}

void Trap::trap_init(std::string name, Map& map, float x, float y)
{
	for (unsigned int i = 0; i < trap_status_num; i++)
	{
		//std::string filename;
		//filename = "./Resource/" + trap_name + "/" + std::to_string(i) + ".png";
		//tile[i].load(filename);
	}
	locate_x = x;
	locate_y = y;
	hitbox = map.get_tiles_width() / 2;
	hitbox_center_x = locate_x + map.get_tiles_width() / 2;
	hitbox_center_y = locate_y + map.get_tiles_height() / 2;
 }










bool Map::load_map(const char* filename)
{
	std::ifstream map_file(filename, std::ios::in);
	if (!map_file.is_open())
	{
		std::cerr << "Error: File failed to open." << std::endl;
		return false;
	}

	std::string line;
	std::getline(map_file, line);
	if (!(std::stringstream(line.substr(line.find(' ') + 1, '\n')) >> map_width))
	{
		std::cerr << "Error: map_width invalid number: " << line << std::endl;
		return false;
	}
	else
		std::cout << "The map width is : " << map_width << std::endl;

	std::getline(map_file, line);
	if (!(std::stringstream(line.substr(line.find(' ') + 1, '\n')) >> map_height))
	{
		std::cerr << "Error: map_height invalid number: " << line << std::endl;
		return false;
	}
	else
		std::cout << "The map height is : " << map_height << std::endl;

	std::getline(map_file, line);
	if (!(std::stringstream(line.substr(line.find(' ') + 1, '\n')) >> tiles_width))
	{
		std::cerr << "Error: tiles_width invalid number: " << line << std::endl;
		return false;
	}
	else
		std::cout << "The tiles width is : " << tiles_width << std::endl;

	std::getline(map_file, line);
	if (!(std::stringstream(line.substr(line.find(' ') + 1, '\n')) >> tiles_height))
	{
		std::cerr << "Error: tiles_height invalid number: " << line << std::endl;
		return false;
	}
	else
		std::cout << "The tiles height is : " << tiles_height << std::endl;

	std::getline(map_file, line);
	if (!(std::stringstream(line.substr(line.find(' ') + 1, '\n')) >> trap_num))
	{
		std::cerr << "Error: tiles_height invalid number: " << line << std::endl;
		return false;
	}
	else
		std::cout << "The trap num is : " << trap_num << std::endl;
	std::getline(map_file, line);
	this->allocate();

	for (unsigned int h = 0; h < map_height; h++)
	{
		std::getline(map_file, line);
		std::stringstream ss(line);
		for (unsigned int w = 0; w < map_width; w++)
		{
			int map_id;
			char comma;
			ss >> map_id;
			ss >> comma;
			at(w, h) = map_id;
			if(map_id == 24)

			std::cout << at(w, h) << " ";
		}
		std::cout << std::endl;
	}
	return true;
}