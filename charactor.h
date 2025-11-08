#pragma once
#include "GamesEngineeringBase.h"
#include <iostream>

enum class Move_Status {
	Front = 0,
	Back = 1,
	Left = 2,
	Right = 3,
	Dead = 4,
};

enum class Unit_Type {
	Hero,
	Enenmy,
	Bullet,
	Trap,
	Null
};

struct Position
{
	float x;
	float y;
};

//#include <iostream>
//#include <stdexcept>

template<typename T>
class My_Stack {
private:
	struct Node {
		T data;
		Node* next;
		Node(const T& value) : data(value), next(nullptr){}
	};

	Node* head;
	unsigned int  stack_size;

public:
	My_Stack() :
		head(nullptr), 
		stack_size(0) {}

	bool empty() const { return stack_size == 0; }
	unsigned int get_size() const { return stack_size; }

	T get_head() { return head->data; }

	bool find(T value) 
	{
		Node* curr = nullptr;
		if (head == nullptr)
			return false;

		curr = head;
		
		while(curr !=nullptr)
		{
			if (curr->data == value)
				return true;
			else
				curr = curr->next;
		}
	}

	void push(const T& value) {
		Node* new_node = new Node(value);
		if (empty()) 
			head = new_node;
		else {
			new_node->next = head;
			head = new_node;
		}
		stack_size++;
	}

	void pop() {

		Node* temp = head;
		head = head->next;
		delete temp;
		stack_size--;
	}

	void clear() {
		while (!empty()) {
			pop();
		}
	}

	~My_Stack() {
		clear();
	}
};



//The class of every unit, include the main charactor and enemies
class Unit
{
protected:
	float locate_x, locate_y;
	unsigned int hitbox;
	float hitbox_center_x;
	float hitbox_center_y;
	Unit_Type unit_type;

public:
	Unit() : 
		locate_x(480.0f), 
		locate_y(352.0f),
		hitbox(0), 
		hitbox_center_x(0), 
		hitbox_center_y(0), 
		unit_type(Unit_Type::Null) {}

	Unit(Unit_Type ut) : 
		locate_x(0), 
		locate_y(0), 
		hitbox(0),
		hitbox_center_x(0),
		hitbox_center_y(0),
		unit_type(ut) {}

	Unit(float x, float y, Unit_Type ut) :
		locate_x(static_cast<float>(x)),
		locate_y(static_cast<float>(y)),
		hitbox(0),
		hitbox_center_x(0),
		hitbox_center_y(0),
		unit_type(ut) {}


	//Load the image
	//bool load_image(std::string filename);

	//The default function of draw the image
	//virtual void draw(GamesEngineeringBase::Window& canvas) = 0;

	//Get the unit's X location
	inline float get_x() { return locate_x; }
	
	//Get the unit's Y location
	inline float get_y() {	return locate_y; }

	inline unsigned int get_hitbox() { return hitbox; }

	//inline unsigned int get_hitbox_x() { return hitbox_center_x; }
	inline float get_hitbox_x() { return hitbox_center_x; }

	//inline unsigned int get_hitbox_y() { return hitbox_center_y; }
	inline float get_hitbox_y() { return hitbox_center_y; }


	//Get the inmage's width
	virtual unsigned int get_width() = 0;


	//Get the image's height
	virtual unsigned int get_height() = 0;


	~Unit() {}
};

const unsigned int charactor_move_status_num = 5;
class Charactor : public Unit
{

private:
	GamesEngineeringBase::Image image[charactor_move_status_num];
	int health; //charactor's health,storage the current health num
	unsigned int speed;
	unsigned int attack;
	float attack_cd;
	float aoe_cd;
	float aoe_range = 300.f;
	unsigned int aoe_num = 5;
	float invincible_time;
	Move_Status m_status;

public:
	Charactor() :
		Unit(480.f, 532.f, Unit_Type::Hero),
		health(50),
		speed(250),
		attack(5),
		attack_cd(0.5f),
		invincible_time(1.f),
		aoe_cd(5.f),
		m_status(Move_Status::Front) {}

	Charactor(float x, float y, int health, int speed, int attack) :
		Unit(x, y, Unit_Type::Hero),
		health(health),
		speed(speed),
		attack(attack),
		attack_cd(0.5f),
		aoe_cd(5.f),
		invincible_time(1.f),
		m_status(Move_Status::Front) {
	}

	void suffer_attack(unsigned int attack) 
	{ 
		health -= attack; 

		if (health < 0)
			health = 0;
		std::cout << "health: " << health << std::endl;

	}

	bool check_if_dead() 
	{
		if (health <= 0)
			return true;
		else
			return false;
	}

	unsigned int get_attack() { return attack; }

	float get_attack_cd() { return attack_cd; }

	float get_aoe_cd() { return aoe_cd; }

	float get_aoe_range() { return aoe_range; }

	unsigned int get_aoe_num() { return aoe_num; }

	float get_invincible_time() { return invincible_time; }

	bool load_image(std::string Heroname);

	void draw(GamesEngineeringBase::Window& canvas, unsigned int x, unsigned int y);

	void draw_incenter(GamesEngineeringBase::Window& canvas);

	void update(GamesEngineeringBase::Window& canvas, float x, float y, Move_Status status);

	void update(Move_Status stauts);

	void load(float x, float y,unsigned int hb, float hb_x, float hb_y, int hea, unsigned int att, 
		float att_cd, float ao_cd, float aoe_ran, unsigned int aoe_n, float invin);

	GamesEngineeringBase::Image& operator[](unsigned int index) { return image[index]; }

	//Get the inmage's width
	inline unsigned int get_width() { return image[0].width; }

	//Get the image's height
	inline unsigned int get_height() {return image[0].height; }

	//Get the charactor's speed
	inline unsigned int get_speed() { return speed; }

	//Get the charactor's health
	int get_health() { return health; }

	~Charactor() 
	{

	}
};

enum class Enemy_type
{
	Slime = 0,
	Bug = 1,
	FlySpookmoth = 2,
	Pebblin = 3,
	MAX_TYPES
};

struct Enemy_data {
	std::string name;
	Enemy_type type;
	int health;
	unsigned int speed;
	unsigned int attack;
	unsigned int attack_cd;
};

class Enemy_index
{
	unsigned int num = static_cast<unsigned int>(Enemy_type::MAX_TYPES);
	Enemy_data enemy_templates[static_cast<unsigned int>(Enemy_type::MAX_TYPES)] = {
	{"Slime",         Enemy_type::Slime,          10,   50, 3, 0},
	{"Bug",           Enemy_type::Bug,            5,  100, 1, 0},
	{"FlySpookmoth",  Enemy_type::FlySpookmoth,  10,    0, 5, 2},
	{"Pebblin",       Enemy_type::Pebblin,       20,   10, 5, 0}
	};
	//std::string arr[enemy_type_n] = { Slime, Bug, FlySpookmoth, Pebblin};
public:

	unsigned int const get_enemy_index_num() { return num; }

	Enemy_type string_to_enemy_type(std::string name)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(Enemy_type::MAX_TYPES); i++)
		{
			if (name == enemy_templates[i].name)
				return static_cast<Enemy_type>(i);
		}
	}

	std::string enemy_type_to_string(Enemy_type e_type)
	{
		return enemy_templates[static_cast<unsigned int>(e_type)].name;
	}

	Enemy_data& operator[] (unsigned int index) { return enemy_templates[index]; }
	Enemy_data& operator[] (Enemy_type name) { return enemy_templates[static_cast<int>(name)]; }
};


const unsigned int enemy_move_status_num = 4;
class Enemy : public Unit
{
	std::string enemy_name;
	Enemy_type type;
	GamesEngineeringBase::Image image[enemy_move_status_num];
	int health; //enemy's health,storage the current health num
	unsigned int speed;
	unsigned int attack;
	unsigned int attack_cd = 0;
	Move_Status m_status;

public:
	Enemy(std::string name, Enemy_type ty) :
		enemy_name(name),
		type(ty),
		Unit(600.f, 600.f, Unit_Type::Enenmy),
		health(10),
		speed(2),
		attack(0),
		m_status(Move_Status::Front) {}

	Enemy(std::string name, Enemy_type ty, unsigned int health, unsigned int speed) :
		enemy_name(name),
		type(ty),
		Unit(600, 600, Unit_Type::Enenmy),
		health(health),
		speed(speed),
		attack(0),
		m_status(Move_Status::Front) {
	}

	Enemy(std::string name, Enemy_type ty, float x, float y, unsigned int health,
		unsigned int speed, unsigned int attack, unsigned int attack_cd) :
		enemy_name(name),
		type(ty),
		Unit(x, y, Unit_Type::Enenmy),
		health(health),
		speed(speed),
		attack(attack),
		attack_cd(attack_cd),
		m_status(Move_Status::Front) {
	}
	bool load_image();

	void load(float x, float y, unsigned int hb, float hb_x, float hb_y, std::string ene_name,
		Enemy_type ene_ty, int hea, unsigned int sp, unsigned int att, unsigned int att_cd);

	void draw(GamesEngineeringBase::Window& canvas,int x, int y);

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

	inline std::string get_name() { return enemy_name; }

	inline Enemy_type get_type() { return type; }

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

	~Enemy()
	{

	}
};


enum class Bullet_type
{
	Blue = 0,
	Red = 1,
	Light = 2,
	MAX_TYPES
};

struct Bullet_data {
	std::string name;
	Bullet_type type;
	unsigned int speed;
	unsigned int attack;
};

class Bullet_index
{
	unsigned int num = static_cast<unsigned int>(Bullet_type::MAX_TYPES);
	Bullet_data bullet_templates[static_cast<unsigned int>(Bullet_type::MAX_TYPES)] = {
	{"Blue",         Bullet_type::Blue,         5, 5},
	{"Red",           Bullet_type::Red,         7, 2},
	{"Light",	    Bullet_type::Light,        10, 20},
	};
	//std::string arr[enemy_type_n] = { Slime, Bug, FlySpookmoth, Pebblin};
public:

	unsigned int const get_bullet_index_num() { return num; }

	Bullet_type string_to_bullet_type(std::string name)
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(Bullet_type::MAX_TYPES); i++)
		{
			if (name == bullet_templates[i].name)
				return static_cast<Bullet_type>(i);
		}
	}

	std::string bullet_type_to_string(Bullet_type e_type)
	{
		return bullet_templates[static_cast<unsigned int>(e_type)].name;
	}

	Bullet_data& operator[] (unsigned int index) { return bullet_templates[index]; }
	Bullet_data& operator[] (Bullet_type name) { return bullet_templates[static_cast<int>(name)]; }
};


const unsigned int bullet_move_status_num = 1;
class Bullet : public Unit
{
	Bullet_index b_index;
	std::string bullet_name;
	Bullet_type type;
	Unit_Type from;
	GamesEngineeringBase::Image image[bullet_move_status_num];
	float health = 5;
	unsigned int speed;
	unsigned int attack;
	Move_Status m_status = Move_Status::Front;

public:
	Bullet(std::string name, Bullet_type ty, Unit_Type fr) :
		bullet_name(name),
		type(ty),
		from(fr),
		Unit(600.f, 600.f, Unit_Type::Bullet),
		//health(10),
		speed(4),
		attack(0)
		//m_status(Move_Status::Front) 
	{}

	Bullet(std::string name, Bullet_type ty, Unit_Type fr, float x, float y) :
		bullet_name(name),
		type(ty),
		from(fr),
		Unit(x, y, Unit_Type::Bullet)
	{
		speed = b_index[ty].speed;
		attack = b_index[ty].attack;
	}

	Bullet(std::string name, Bullet_type ty, Unit_Type fr, float x, float y,
		unsigned int speed, unsigned int attack) :
		bullet_name(name),
		type(ty),
		from(fr),
		Unit(x, y, Unit_Type::Bullet),
		//health(health),
		speed(speed),
		attack(attack){}

	bool load_image();

	void draw(GamesEngineeringBase::Window& canvas, int x, int y);

	void update(GamesEngineeringBase::Window& canvas, float x, float y);

	void update(float x, float y);

	void sub_health(float x) { health -= x; };

	GamesEngineeringBase::Image& operator[](unsigned int index) { return image[index]; }

	inline unsigned int get_health() { return health; }

	inline std::string get_name() { return bullet_name; }

	inline Bullet_type get_type() { return type; }

	inline Unit_Type get_from() { return from; }

	inline unsigned int get_attack() { return attack; }

	//Get the inmage's width
	inline unsigned int get_width() { return image[0].width; }

	//Get the image's height
	inline unsigned int get_height() { return image[0].height; }

	//Get the charactor's speed
	inline unsigned int get_speed() { return speed; }

	//Get the charactor's health
	//int get_health() { return health; }

	~Bullet()
	{

	}
};


