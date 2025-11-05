#pragma once
#include "charactor.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class Map;

const unsigned int tile_num = 25;
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

const unsigned int trap_status_num = 1;
class Trap : public Unit
{
	std::string trap_name;
	//Enemy_type type;
	GamesEngineeringBase::Image image[trap_status_num];
	int health; //enemy's health,storage the current health num
	unsigned int speed;
	unsigned int attack;
	unsigned int attack_cd = 0;
	Move_Status m_status;

public:
	//Trap() :
	//	trap_name(NULL),
	//	//type(ty),
	//	Unit(600.f, 600.f, Unit_Type::Trap),
	//	health(10),
	//	speed(0),
	//	attack(2),
	//	m_status(Move_Status::Front) {
	//}

	Trap(std::string name = "NULL") :
		trap_name(name),
		//type(ty),
		Unit(600.f, 600.f, Unit_Type::Trap),
		health(10),
		speed(0),
		attack(2),
		m_status(Move_Status::Front) {
	}

	Trap(std::string name, unsigned int health, unsigned int speed) :
		trap_name(name),
		//type(ty),
		Unit(600, 600, Unit_Type::Trap),
		health(health),
		speed(speed),
		attack(0),
		m_status(Move_Status::Front) {
	}

	Trap(std::string name, float x, float y, unsigned int health,
		unsigned int speed, unsigned int attack, unsigned int attack_cd) :
		trap_name(name),
		//type(ty),
		Unit(x, y, Unit_Type::Trap),
		health(health),
		speed(speed),
		attack(attack),
		attack_cd(attack_cd),
		m_status(Move_Status::Front) {
	}
	bool load_image();

	void trap_init(std::string name, Map& map, float x, float y);

	void draw(GamesEngineeringBase::Window& canvas, int x, int y);

	void update(GamesEngineeringBase::Window& canvas, float x, float y, Move_Status status);

	void update(float x, float y);

	void suffer_attack(unsigned int attack)
	{
		health -= attack;

		if (health < 0)
			health = 0;
		//std::cout << "health: " << health << std::endl;

	}

	GamesEngineeringBase::Image& operator[](unsigned int index) { return image[index]; }

	inline std::string get_name() { return trap_name; }

	//inline Enemy_type get_type() { return type; }

	unsigned int get_attack() { return attack; }

	inline unsigned int get_attack_cd() { return attack_cd; }

	//Get the inmage's width
	inline unsigned int get_width() { return image[0].width; }

	//Get the image's height
	inline unsigned int get_height() { return image[0].height; }

	//Get the charactor's speed
	inline unsigned int get_speed() { return speed; }

	//Get the charactor's health
	int get_health() { return health; }

	~Trap()
	{

	}
};

class Map
{
	unsigned int tiles_width, tiles_height;
	unsigned int map_width, map_height;
	unsigned int trap_num;
	unsigned int* tiles;
	Trap* trap;
public:
	Map() :
		tiles_width(0),
		tiles_height(0),
		map_width(0),
		map_height(0),
		trap_num(0),
		tiles(nullptr),
		trap(nullptr) {}

	unsigned int& at(unsigned int x, unsigned int y) { return tiles[y * map_width + x]; }

	unsigned int get(unsigned int x, unsigned int y) { return tiles[y * map_width + x]; }

	Position get_pix_locate(unsigned int x, unsigned int y) { return { static_cast<float>(x * tiles_width), static_cast<float>(y * tiles_height)}; }

	void allocate()
	{
		tiles = new unsigned int[map_width * map_height];
		trap = new Trap[trap_num];
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

	unsigned int get_trap_num() const { return trap_num; }

	unsigned int get_map_width() const { return map_width; }

	unsigned int get_map_height() const { return map_height; }

	unsigned int get_tiles_width() const { return tiles_width; }

	unsigned int get_tiles_height() const { return tiles_height; }

	~Map()
	{
		free_tiles();
	}
};


