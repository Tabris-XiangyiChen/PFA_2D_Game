#include "map.h"
#include <random>

class Manager_map;
class Camera;

class Manager_hero
{
	Charactor Hero;
	Move_Status move_status = Move_Status::Front;
	float invincible_time_elapsed = 0;
	float attack_elapsed;
	float aoe_elapsed;
public:
	Manager_hero(GamesEngineeringBase::Window& canvas) : Hero(), attack_elapsed(0), aoe_elapsed(0)
	{
		Hero.load_image("Wizard");
	}

	void update(GamesEngineeringBase::Window& canvas, Manager_map& map, float time);

	void update_cd(float time)
	{
		attack_elapsed += time;
		aoe_elapsed += time;
		invincible_time_elapsed += time;
	}

	void zero_invincible_time_elapsed()
	{
		invincible_time_elapsed = 0;
	}

	void zero_attack_elapsed()
	{
		attack_elapsed = 0;
	}

	void zero_aoe_elapsed()
	{
		aoe_elapsed = 0;
	}

	void draw(GamesEngineeringBase::Window& canvas, Manager_map& map, Camera& cam);

	void suffer_attack(unsigned int attack) { Hero.suffer_attack(attack); }

	unsigned int get_hero_attack() { return Hero.get_attack(); }

	float get_attack_cd() { return Hero.get_attack_cd(); }

	float get_aoe_cd() { return Hero.get_aoe_cd(); }

	float get_invincible_time() { return Hero.get_invincible_time(); }

	float get_invincible_time_elapsed() { return invincible_time_elapsed;  }

	float get_attack_elapsed() { return attack_elapsed; }

	float get_aoe_elapsed() { return aoe_elapsed; }


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
	//Trap* trap;

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
	Enemy_index e_index;
	Enemy* enemy[max_enemy_num];
	unsigned int current_size = 0;
	Move_Status move_status[max_enemy_num];

	float enemy_create_time_elapsed = 0;
	float create_threshold = 3.f;

	float enemy_attack_time_elapsed[max_enemy_num];
	float enemy_attack_time_elapse = 0;

	unsigned int slime_max_num = 10;
	unsigned int slime_current_num = 0;
	unsigned int bug_max_num = 5;
	unsigned int bug_current_num = 0;

public:
	Manager_enemy(GamesEngineeringBase::Window& canvas);

	Position create_out_camera_pos(Manager_map& map, Camera& cam,bool if_near_cam);

	void create_enemy(Manager_map& map,Camera& cam);

	void zero_enemy_attack_time_elapsed(unsigned int index) { enemy_attack_time_elapsed[index] = 0; }

	void update_enemy_attack_time_elapsed(float time) 
	{ 
		for (unsigned int i = 0; i < max_enemy_num; i++)
		{
			if (enemy[i] == nullptr)
				continue;
			if (enemy[i]->get_type() == Enemy_type::FlySpookmoth)
				enemy_attack_time_elapsed[i] += time;
		}
	}

	void delete_enemy(unsigned int i);

	void suffer_attack(unsigned int index, unsigned int attack)
	{
		enemy[index]->suffer_attack(attack);
	}

	void update(GamesEngineeringBase::Window& canvas, Manager_map& map, Manager_hero& hero, Camera& cam, float time);

	void draw(GamesEngineeringBase::Window& canvas, Manager_map& map, Camera& cam);

	float get_enemy_attack_time_elapsed(unsigned int index) { return enemy_attack_time_elapsed[index]; }

	unsigned int get_enemy_current_num() { return current_size; }

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

	Enemy* operator[] (unsigned int index) { return enemy[index]; }

	~Manager_enemy()
	{
		for (unsigned int i = 0; i < max_enemy_num; i++)
		{
			if (enemy[i] == nullptr)
			{
				delete enemy[i];
			}
		}
	}
};

const unsigned int max_bullet_num = 100;
class Manager_bullet
{
	Bullet* bullet[max_bullet_num];
	Bullet_index e_index;
	Move_Status move_status[max_bullet_num];
	Position forward[max_bullet_num];
	//float enemy_create_time_elapsed = 0;
	unsigned int current_size = 0;

public:
	Manager_bullet(GamesEngineeringBase::Window& canvas);

	Position create_out_camera_pos(Manager_map& map, Camera& cam, bool if_near_cam);

	Position set_forward(unsigned int bullet_index, Manager_enemy& enemy);

	void keep_move_to_enemy(unsigned int bullet_index, Position pos, Manager_enemy& enemy, float time);

	void move_to_nearest_hero(unsigned int bullet_index, Position pos, Manager_hero& hero, float time);

	void forward_to_nearest_enemy(unsigned int bullet_index, Position pos, Manager_enemy& enemy, float time);

	void create_bullet(std::string name, Bullet_type ty, Unit_Type fr);

	void create_hero_bullet(Manager_hero& hero, Manager_enemy& enemy);

	void create_enemy_bullet(Manager_enemy& enemy);

	void delete_bullet(unsigned int i);

	void update(GamesEngineeringBase::Window& canvas, Manager_map& map, Manager_hero& hero, Manager_enemy& enemy,  Camera& cam, float time);

	void draw(GamesEngineeringBase::Window& canvas, Manager_map& map, Camera& cam);

	//Get the unit's X location
	inline float get_x(unsigned int index) { return bullet[index]->get_x(); }

	//Get the unit's Y location
	inline float get_y(unsigned int index) { return bullet[index]->get_y(); }

	//int get_health(unsigned int index) { return bullet[index]->get_health(); }

	unsigned int get_attack(unsigned int index) { return bullet[index]->get_attack(); }

	//Get the image's width
	inline unsigned int get_width(unsigned int index) { return bullet[index]->get_width(); }

	//Get the image's height
	inline unsigned int get_height(unsigned int index) { return bullet[index]->get_height(); }

	inline float get_hit_box_x(unsigned int index) { return bullet[index]->get_hitbox_x(); }

	inline float get_hit_box_y(unsigned int index) { return bullet[index]->get_hitbox_y(); }

	Bullet* operator[] (unsigned int index) { return bullet[index]; }

	~Manager_bullet()
	{
		for (unsigned int i = 0; i < max_bullet_num; i++)
		{
			if (bullet[i] == nullptr)
			{
				delete bullet[i];
			}
		}
	}
};