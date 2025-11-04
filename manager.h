#include "map.h"
#include <random>

class Manager_map;
class Camera;

class Manager_hero
{
	Charactor Hero;
	Move_Status move_status = Move_Status::Front;
public:
	Manager_hero(GamesEngineeringBase::Window& canvas) : Hero()
	{
		Hero.load_image("Wizard");
	}

	void update(GamesEngineeringBase::Window& canvas, Manager_map& map, float time);

	void draw(GamesEngineeringBase::Window& canvas, Manager_map& map, Camera& cam);

	void get_attack(unsigned int attack) { Hero.get_attack(attack); }

	//Get the unit's X location
	inline float get_x() { return Hero.get_x(); }

	//Get the unit's Y location
	inline float get_y() { return Hero.get_y(); }

	int get_health() { return Hero.get_health(); }

	//Get the image's width
	inline unsigned int get_width() { return Hero.get_width(); }

	//Get the image's height
	inline unsigned int get_height() { return Hero.get_height(); }

	inline unsigned int get_hitbox() { return Hero.get_hitbox(); }

	inline float get_hitbox_x() { return Hero.get_hitbox_x(); }

	inline float get_hitbox_y() { return Hero.get_hitbox_y(); }
};

class Camera
{
	float locate_x, locate_y;
	unsigned int camera_width, camera_height;
	unsigned int map_width_pix, map_height_pix;
	bool is_at_boundry;
public:
	Camera() :
		locate_x(0),
		locate_y(0),
		camera_width(0),
		camera_height(0),
		map_width_pix(0),
		map_height_pix(0),
		is_at_boundry(false) { }

	Camera(GamesEngineeringBase::Window& canvas, unsigned int map_w, unsigned int map_h);

	void camera_init(GamesEngineeringBase::Window& canvas, unsigned int map_w, unsigned int map_h);

	const float mapx_to_camerax(const float map_x) { return map_x - locate_x; }

	const float mapy_to_cameray(const float map_y) { return map_y - locate_y; }

	float get_x() { return locate_x; }

	float get_y() { return locate_y; }

	unsigned int get_cam_width() { return camera_width; }

	unsigned int get_cam_height() { return camera_height; }

	unsigned int get_map_width_pix() { return map_width_pix; }

	unsigned int get_map_height_pix() { return map_height_pix; }

	bool get_is_at_boundry() { return is_at_boundry;  }

	void update(Manager_hero& hero);
};


class Manager_map
{
	Map map;
	Tiles tiles;

public:
	Manager_map(GamesEngineeringBase::Window& canvas);

	void draw(GamesEngineeringBase::Window& canvas, Manager_hero& hero, Camera& cam);

	unsigned int get_map_width_pix()
	{
		return map.get_map_width() * map.get_tiles_width();
	}


	unsigned int get_map_height_pix()
	{
		return map.get_map_height() * map.get_tiles_height();
	}
};

const unsigned int max_enemy_num = 100;
class Manager_enemy
{
	Enemy* enemy[max_enemy_num];
	Enemy_index e_index;
	Move_Status move_status[max_enemy_num];
	float enemy_create_time_elapsed = 0;
	float hero_invincible_time_elapsed = 0;
	unsigned int current_size = 0;
	float create_threshold = 3.f;

	unsigned int slime_max_num = 10;
	unsigned int slime_current_num = 0;
	unsigned int bug_max_num = 5;
	unsigned int bug_current_num = 0;

public:
	Manager_enemy(GamesEngineeringBase::Window& canvas);

	Position create_out_camera_pos(Manager_map& map, Camera& cam,bool if_near_cam);

	void create_enemy(Manager_map& map,Camera& cam);

	void check_delete_enemy(unsigned int i);

	void update(GamesEngineeringBase::Window& canvas, Manager_map& map, Manager_hero& hero, Camera& cam, float time);

	void draw(GamesEngineeringBase::Window& canvas, Manager_map& map, Camera& cam);

	//Get the unit's X location
	inline float get_x(unsigned int index) { return enemy[index] -> get_x(); }

	//Get the unit's Y location
	inline float get_y(unsigned int index) { return enemy[index]->get_y(); }

	int get_health(unsigned int index) { return enemy[index]->get_health(); }

	//Get the image's width
	inline unsigned int get_width(unsigned int index) { return enemy[index]->get_width(); }

	//Get the image's height
	inline unsigned int get_height(unsigned int index) { return enemy[index]->get_height(); }

	inline float get_hit_box_x(unsigned int index) { return enemy[index]->get_hitbox_x(); }

	inline float get_hit_box_y(unsigned int index) { return enemy[index]->get_hitbox_y(); }
};