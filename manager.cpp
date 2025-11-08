#include "manager.h"


Manager_hero::Manager_hero(GamesEngineeringBase::Window& canvas) : Hero(nullptr), move_status(Move_Status::Front), attack_elapsed(0), aoe_elapsed(0)
{
	Hero = new Charactor();
	Hero->load_image("Wizard");
}

void Manager_hero::init()
{
	delete Hero;
	Hero == nullptr;
	Hero = new Charactor();
	Hero->load_image("Wizard");
	move_status = Move_Status::Front;
	invincible_time_elapsed = 0;
	attack_elapsed = 0; 
	aoe_elapsed = 0;
}

void Manager_hero::update(GamesEngineeringBase::Window& canvas, Manager_map& map, float time) 
{
	if(!Hero->check_if_dead())
	{
		update_cd(time);
		Position pos = { 0, 0 };
		float new_x = Hero->get_x();
		float new_y = Hero->get_y();
		//int speed = static_cast<int>(max(static_cast<float>(Hero.get_speed() * 50) * time, 1.0f));
		float speed = max(static_cast<float>(Hero->get_speed()) * time, 1.0f);

		//std::cout << "speed is " << speed << "the time is " << time << std::endl;

		for (unsigned int i = 0; i < map.get_trap_num(); i++)
		{
			
			float dx = Hero->get_hitbox_x() - map.get_trap_position(i).x;
			float dy = Hero->get_hitbox_y() - map.get_trap_position(i).y;
			float dist = sqrt(dx * dx + dy * dy);
			float hitbox_dist = static_cast<float>(Hero->get_hitbox() + map.get_trap_hitbox(i));
			if (dist < hitbox_dist)
			{
				if (invincible_time_elapsed > Hero->get_invincible_time())
				{
					Hero->suffer_attack(map.get_trap_attack(i));
					zero_invincible_time_elapsed();
				}
				pos = rebound({ Hero->get_hitbox_x(), Hero->get_hitbox_y() }, map.get_trap_position(i), hitbox_dist);
				new_x = pos.x - static_cast<float>(Hero->get_width() / 2);
				new_y = pos.y - static_cast<float>(Hero->get_height() / 2);
				//std::cout << " touch the trap" << std::endl;
				Hero->update(canvas, new_x, new_y, move_status);
				//return;
			}
		}

		//move_status = Move_Status::Front;
		if (canvas.keyPressed('W'))
		{
			move_status = Move_Status::Back;
			//y -= Hero.get_speed();
			new_y -= speed;
		}
		if (canvas.keyPressed('S'))
		{
			move_status = Move_Status::Front;
			new_y += speed;
		}
		if (canvas.keyPressed('A'))
		{
			move_status = Move_Status::Left;
			new_x -= speed;
		}
		if (canvas.keyPressed('D'))
		{
			move_status = Move_Status::Right;
			new_x += speed;
		}
		new_x = max(0, new_x);
		new_x = min(static_cast<int>(map.get_map_width_pix()) - static_cast<int>(Hero->get_width()), new_x);
		new_y = max(0, new_y);
		new_y = min(static_cast<int>(map.get_map_height_pix()) - static_cast<int>(Hero->get_height()), new_y);

		Hero->update(canvas, new_x, new_y, move_status);
		return;
	}
	else 
	{
		move_status = Move_Status::Dead;
		Hero->update(move_status);
	}

}

void Manager_hero::update_infinite(GamesEngineeringBase::Window& canvas, Manager_map& map, float time)
{
	if (!Hero->check_if_dead())
	{
		update_cd(time);
		Position pos = { 0, 0 };
		float new_x = Hero->get_x();
		float new_y = Hero->get_y();
		//int speed = static_cast<int>(max(static_cast<float>(Hero.get_speed() * 50) * time, 1.0f));
		float speed = max(static_cast<float>(Hero->get_speed()) * time, 1.0f);

		//std::cout << "speed is " << speed << "the time is " << time << std::endl;

		for (unsigned int i = 0; i < map.get_trap_num(); i++)
		{

			float dx = Hero->get_hitbox_x() - map.get_trap_position(i).x;
			float dy = Hero->get_hitbox_y() - map.get_trap_position(i).y;
			float dist = sqrt(dx * dx + dy * dy);
			float hitbox_dist = static_cast<float>(Hero->get_hitbox() + map.get_trap_hitbox(i));
			if (dist < hitbox_dist)
			{
				if (invincible_time_elapsed > Hero->get_invincible_time())
				{
					Hero->suffer_attack(map.get_trap_attack(i));
					zero_invincible_time_elapsed();
				}
				pos = rebound({ Hero->get_hitbox_x(), Hero->get_hitbox_y() }, map.get_trap_position(i), hitbox_dist);
				new_x = pos.x - static_cast<float>(Hero->get_width() / 2);
				new_y = pos.y - static_cast<float>(Hero->get_height() / 2);
				std::cout << " touch the trap" << std::endl;
				Hero->update(canvas, new_x, new_y, move_status);
				//return;
			}
		}

		//move_status = Move_Status::Front;
		if (canvas.keyPressed('W'))
		{
			move_status = Move_Status::Back;
			//y -= Hero.get_speed();
			new_y -= speed;
		}
		if (canvas.keyPressed('S'))
		{
			move_status = Move_Status::Front;
			new_y += speed;
		}
		if (canvas.keyPressed('A'))
		{
			move_status = Move_Status::Left;
			new_x -= speed;
		}
		if (canvas.keyPressed('D'))
		{
			move_status = Move_Status::Right;
			new_x += speed;
		}


		float map_width_pix = map.get_map_width_pix();
		float map_height_pix = map.get_map_height_pix();

		new_x = fmod(new_x, map_width_pix);
		new_y = fmod(new_y, map_height_pix);

		if (new_x < 0)
			new_x += map_width_pix;

		if (new_y < 0)
			new_y += map_height_pix;

		//std::cout << "{" << new_x << "," << new_y << "}" << std::endl;


		//new_x = max(0, new_x);
		//new_x = min(static_cast<float>(map.get_map_width_pix()) - static_cast<float>(Hero->get_width()), new_x);
		//new_y = max(0, new_y);
		//new_y = min(static_cast<float>(map.get_map_height_pix()) - static_cast<float>(Hero->get_height()), new_y);

		Hero->update(canvas, new_x, new_y, move_status);
		return;
	}
	else
	{
		move_status = Move_Status::Dead;
		Hero->update(move_status);
	}

}

void Manager_hero::draw(GamesEngineeringBase::Window& canvas, Manager_map& map, Camera& cam)
{
	if (cam.get_is_at_boundry())
	{
		Hero->draw(canvas, static_cast<int>(cam.mapx_to_camerax(Hero->get_x())), static_cast<int>(cam.mapy_to_cameray(Hero->get_y())));
	}
	else
		Hero->draw_incenter(canvas);
}

void Manager_hero::save_hero_state(const std::string& filename)
{
	std::ofstream file(filename, std::ios::out | std::ios::app);
	if (!file.is_open()) {
		std::cerr << "[Error] Cannot open save file: " << filename << std::endl;
		return;
	}

	file << "Hero States" << std::endl
		<< Hero->get_x() << " "
		<< Hero->get_y() << " "
		<< Hero->get_hitbox() << " "
		<< Hero->get_hitbox_x() << " "
		<< Hero->get_hitbox_y() << " "
		<< Hero->get_health() << " "
		<< Hero->get_attack() << " "
		<< Hero->get_attack_cd() << " "
		<< Hero->get_aoe_cd() << " "
		<< Hero->get_aoe_range() << " "
		<< Hero->get_aoe_num() << " "
		<< Hero->get_invincible_time() << " "
		<< invincible_time_elapsed << " "
		<< attack_elapsed << " "
		<< aoe_elapsed << " "
		<< static_cast<int>(move_status)
		<< std::endl;

	file.close();
	std::cout << "[Save] Hero state saved to " << filename << std::endl;
}

void Manager_hero::load_hero_state(std::string filename)
{
	std::ifstream file(filename, std::ios::in);
	if (!file.is_open()) {
		std::cerr << "[Error] Cannot open load file: " << filename << std::endl;
		return;
	}

	float x, y, hb, hb_x, hb_y;
	int health;
	unsigned int attack, aoe_num;
	float attack_cd, aoe_cd, aoe_range, invincible_time;
	float invincible_elapsed, atk_elapsed, aoe_elapsed;
	int status_int;

	std::string line;
	while (std::getline(file, line))
	{
		std::cout << line << std::endl;
		if (line.find("Hero States") != std::string::npos)
			break;
	}

	file >> x >> y >> hb>> hb_x >> hb_y
		>> health >> attack
		>> attack_cd >> aoe_cd >> aoe_range >> aoe_num
		>> invincible_time >> invincible_elapsed
		>> atk_elapsed >> aoe_elapsed >> status_int;

	file.close();
	
	std::cout << x << " " << y << std::endl;
	Hero->load(x, y, hb, hb_x, hb_y, health, attack, attack_cd, aoe_cd, aoe_range, aoe_num, invincible_time);
	invincible_time_elapsed = invincible_elapsed;
	attack_elapsed = atk_elapsed;
	aoe_elapsed = aoe_elapsed;
	move_status = static_cast<Move_Status>(status_int);

	std::cout << "[Load] Hero state loaded from " << filename << std::endl;

}

Camera::Camera(GamesEngineeringBase::Window& canvas, unsigned int map_w, unsigned int map_h) :
	camera_width(canvas.getWidth()),
	camera_height(canvas.getHeight()),
	map_width_pix(map_w),
	map_height_pix(map_h),
	is_at_boundry(false)

{
	locate_x = static_cast<float>(map_width_pix / 2 - camera_width / 2);
	locate_y = static_cast<float>(map_height_pix / 2 - camera_height / 2);
}

void Camera::camera_init(GamesEngineeringBase::Window& canvas, unsigned int map_w, unsigned int map_h)
{
	camera_width = canvas.getWidth();
	camera_height = canvas.getHeight();
	map_width_pix = map_w;
	map_height_pix = map_h;
	locate_x = static_cast<float>(map_width_pix / 2 - camera_width / 2);
	locate_y = static_cast<float>(map_height_pix / 2 - camera_height / 2);
}

void Camera::update(Manager_hero& hero)
{
	is_at_boundry = false;
	locate_x = hero.get_x() + hero.get_width() / 2 - camera_width / 2;
	locate_y = hero.get_y() + hero.get_height() / 2 - camera_height / 2;

	if (locate_x <= 0)
	{
		is_at_boundry = true;
		locate_x = max(locate_x, 0);
	}
	if (static_cast<int>(locate_x) + camera_width >= map_width_pix)
	{
		is_at_boundry = true;
		locate_x = min(locate_x, static_cast<float>(map_width_pix - camera_width));
	}
	if (locate_y <= 0)
	{
		is_at_boundry = true;
		locate_y = max(locate_y, 0);
	}
	if (static_cast<int>(locate_y)+camera_height >= map_height_pix)
	{
		is_at_boundry = true;
		locate_y = min(locate_y, static_cast<float>(map_height_pix - camera_height));
	}

	//std::cout << "(" << locate_x << "," << locate_y << ")" << std::endl;
}

void Camera::update_infinite(Manager_hero& hero)
{
	is_at_boundry = false;
	locate_x = hero.get_x() + hero.get_width() / 2 - camera_width / 2;
	locate_y = hero.get_y() + hero.get_height() / 2 - camera_height / 2;

	//if (locate_x <= 0)
	//{
	//	is_at_boundry = true;
	//	locate_x = max(locate_x, 0);
	//}
	//if (static_cast<int>(locate_x) + camera_width >= map_width_pix)
	//{
	//	is_at_boundry = true;
	//	locate_x = min(locate_x, static_cast<float>(map_width_pix - camera_width));
	//}
	//if (locate_y <= 0)
	//{
	//	is_at_boundry = true;
	//	locate_y = max(locate_y, 0);
	//}
	//if (static_cast<int>(locate_y) + camera_height >= map_height_pix)
	//{
	//	is_at_boundry = true;
	//	locate_y = min(locate_y, static_cast<float>(map_height_pix - camera_height));
	//}

	//std::cout << "(" << locate_x << "," << locate_y << ")" << std::endl;
}

Manager_map::Manager_map(GamesEngineeringBase::Window& canvas) : map(), tiles()
{
	tiles.tiles_init("map");
	//map.load_map("./Resource/map/tiles.txt");
	//trap = new Trap[map.get_trap_num()];
	//cam.camera_init(canvas, map.get_map_width() * map.get_tiles_width(),
	//	map.get_map_height() * map.get_tiles_height());

}

bool Manager_map::map_init(std::string filename)
{
	//std::string f_name = "./Resource/map/" + filename + ".txt";
	//std::cout << f_name << "init map" << std::endl;
	map.load_map(filename);
	//if (map.load_map(f_name))
	//	return true;
	//else
	//	return false;

	return true;
}

void Manager_map::draw(GamesEngineeringBase::Window& canvas, Manager_hero& hero, Camera& cam)
{
	/*cam.update(hero);*/
	for (unsigned int w = 0; w < map.get_map_width(); w++)
	{
		//std::cout << w << " ";
		for (unsigned int h = 0; h < map.get_map_height(); h++)
		{
			tiles.draw(canvas, map.get(w, h),
				static_cast<int>(cam.mapx_to_camerax(static_cast<float>(w * map.get_tiles_width()))),
				static_cast<int>(cam.mapy_to_cameray(static_cast<float>(h * map.get_tiles_height()))));
			//if(w == 0)
			//{
			//	std::cout << "(" << w * map.get_tiles_width() << "," << h * map.get_tiles_height() << ")";
			//}
		}
		//std::cout << std::endl;
	}
}

void Manager_map::draw_infinite(GamesEngineeringBase::Window& canvas, Manager_hero& hero, Camera& cam)
{
	int map_w_pix = get_map_width_pix();
	int map_h_pix = get_map_height_pix();

	// 当前相机相对原点的偏移
	float cam_offset_x = fmod(cam.get_x(), static_cast<float>(map_w_pix));
	float cam_offset_y = fmod(cam.get_y(), static_cast<float>(map_h_pix));

	// 让相机始终在循环范围内
	if (cam_offset_x < 0) cam_offset_x += map_w_pix;
	if (cam_offset_y < 0) cam_offset_y += map_h_pix;

	// 绘制 3×3 个地图块（中间是当前地图，周围是镜像副本）
	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			int offset_x = dx * map_w_pix - static_cast<int>(cam_offset_x);
			int offset_y = dy * map_h_pix - static_cast<int>(cam_offset_y);

			// 绘制当前偏移地图
			for (unsigned int w = 0; w < map.get_map_width(); w++)
			{
				for (unsigned int h = 0; h < map.get_map_height(); h++)
				{
					tiles.draw(canvas, map.get(w, h),
						static_cast<int>(w * map.get_tiles_width() + offset_x),
						static_cast<int>(h * map.get_tiles_height() + offset_y));
				}
			}
		}
	}
}


void Manager_map::save_map_state(std::string filename)
{
	std::ofstream file(filename, std::ios::out | std::ios::app);
	if (!file.is_open()) {
		std::cerr << "[Error] Cannot open save file: " << filename << std::endl;
		return;
	}

	file << "Map States" << std::endl
		<< map.get_map_name() << " "
		<< std::endl;

	file.close();
	std::cout << "[Save] Map state saved to " << filename << std::endl;
}

void Manager_map::load_map_state(std::string filename)
{
	std::ifstream file(filename, std::ios::in);
	if (!file.is_open()) {
		std::cerr << "[Error] Cannot open load file: " << filename << std::endl;
		return;
	}

	std::string name;
	std::string line;
	while (std::getline(file, line))
	{
		std::cout << line << std::endl;
		if (line.find("Map States") != std::string::npos)
			break;
	}

	std::getline(file, name);
	std::cout << name << std::endl;

	map_init(name);

	file.close();


	std::cout << "[Load] Map state loaded from " << filename << std::endl;

}

Manager_enemy::Manager_enemy(GamesEngineeringBase::Window& canvas)
{
	for (unsigned int i = 0; i < max_enemy_num; i++)
	{
		enemy[i] = nullptr;
		move_status[i] = Move_Status::Front;
		enemy_attack_time_elapsed[i] = 0;
	}
	score = 0;
	current_size = 0;
}

Position Manager_enemy::create_out_camera_pos(Manager_map& map, Camera& cam, bool if_near_cam)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());

	Position pos{ 0, 0 };
	float margin = 20.f;

	if (if_near_cam) {

		std::uniform_int_distribution<int> side_dist(0, 3);
		std::uniform_int_distribution<int> offset_x(0, cam.get_cam_width());
		std::uniform_int_distribution<int> offset_y(0, cam.get_cam_height());

		int side = side_dist(gen);
		switch (side) {
		case 0: // Up
			pos.x = cam.get_x() + offset_x(gen);
			pos.y = cam.get_y() - margin;
			break;
		case 1: // Bottom
			pos.x = cam.get_x() + offset_x(gen);
			pos.y = cam.get_y() + cam.get_cam_height() + margin;
			break;
		case 2: // Left
			pos.x = cam.get_x() - margin;
			pos.y = cam.get_y() + offset_y(gen);
			break;
		case 3: // Right
			pos.x = cam.get_x() + cam.get_cam_width() + margin;
			pos.y = cam.get_y() + offset_y(gen);
			break;
		}
	}
	else {
		std::uniform_int_distribution<int> distX(0, map.get_map_width_pix());
		std::uniform_int_distribution<int> distY(0, map.get_map_height_pix());

		while (true) {
			pos.x = static_cast<float>(distX(gen));
			pos.y = static_cast<float>(distY(gen));

			bool inCameraView = (pos.x > cam.get_x() && pos.x < cam.get_x() + cam.get_cam_width() &&
				pos.y > cam.get_y() && pos.y < cam.get_y() + cam.get_cam_height());
			if (!inCameraView)
				break;
		}
	}

	pos.x = max(0, min(pos.x, map.get_map_width_pix()));
	pos.y = max(0, min(pos.y, map.get_map_height_pix()));

	return pos;
}

Position Manager_enemy::create_out_camera_pos_infinite(Manager_map& map, Camera& cam, bool if_near_cam)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());

	Position pos{ 0, 0 };
	float margin = 20.f;
	float map_w = map.get_map_width_pix();
	float map_h = map.get_map_height_pix();

	if (if_near_cam) {
		std::uniform_int_distribution<int> side_dist(0, 3);
		std::uniform_int_distribution<int> offset_x(0, cam.get_cam_width());
		std::uniform_int_distribution<int> offset_y(0, cam.get_cam_height());

		int side = side_dist(gen);
		switch (side) {
		case 0: // Up
			pos.x = fmod(cam.get_x() + offset_x(gen), map_w);
			pos.y = fmod(cam.get_y() - margin + map_h, map_h);
			break;
		case 1: // Bottom
			pos.x = fmod(cam.get_x() + offset_x(gen), map_w);
			pos.y = fmod(cam.get_y() + cam.get_cam_height() + margin, map_h);
			break;
		case 2: // Left
			pos.x = fmod(cam.get_x() - margin + map_w, map_w);
			pos.y = fmod(cam.get_y() + offset_y(gen), map_h);
			break;
		case 3: // Right
			pos.x = fmod(cam.get_x() + cam.get_cam_width() + margin, map_w);
			pos.y = fmod(cam.get_y() + offset_y(gen), map_h);
			break;
		}
	}
	else {
		std::uniform_real_distribution<float> distX(0.f, map_w);
		std::uniform_real_distribution<float> distY(0.f, map_h);

		while (true) {
			pos.x = distX(gen);
			pos.y = distY(gen);

			// 判断是否在摄像机视野内
			float cam_x = cam.get_x();
			float cam_y = cam.get_y();
			float cam_w = cam.get_cam_width();
			float cam_h = cam.get_cam_height();

			bool inCameraView = (pos.x > cam_x && pos.x < cam_x + cam_w &&
				pos.y > cam_y && pos.y < cam_y + cam_h);
			if (!inCameraView) break;
		}
	}

	return pos;
}


void Manager_enemy::create_enemy(Manager_map& map, Camera& cam, Manager_hero& hero)
{
	//If only use srand(),it sometimes happen the continuous enemy are the same;
	static std::random_device rd;         
	static std::mt19937 gen(rd());
	// avoid create the upgrade
	static std::uniform_int_distribution<> dist(0, e_index.get_enemy_index_num() - 2);

	if (current_size < max_size)
		if (enemy_create_time_elapsed > create_threshold)
		{
			for (unsigned int i = 0; i < max_enemy_num; i++)
			{
				if (enemy[i] == nullptr)
				{
					Position pos = create_out_camera_pos(map, cam, true);
					unsigned int index = dist(gen);
					enemy[i] = new Enemy(e_index[index].name, e_index[index].type, pos.x, pos.y, e_index[index].health,
						e_index[index].speed, e_index[index].attack, e_index[index].attack_cd);
					//enemy_attack_time_elapsed[i] = e_index[index].attack_cd;
					enemy[i]->load_image();
					current_size++;
					enemy_create_time_elapsed = 0.f;
					create_threshold -= 0.2f;
					create_threshold = max(create_threshold, 0.5f);
					//std::cout << "creat enemy" << std::endl;
					break;
				}
			}
		}

	if (score > 100 && hero.get_is_upgrade() == false)
	{
		for (unsigned int i = 0; i < max_enemy_num; i++)
		{
			if (enemy[i] == nullptr)
			{
				Position pos = create_out_camera_pos(map, cam, false);
				enemy[i] = new Enemy(e_index[4].name, e_index[4].type, pos.x, pos.y, e_index[4].health,
					e_index[4].speed, e_index[4].attack, e_index[4].attack_cd);
				//enemy_attack_time_elapsed[i] = e_index[index].attack_cd;
				enemy[i]->load_image();
				current_size++;
				hero.set_upgrade(true);
				std::cout << "creat upgrade" << std::endl;
				break;
			}
		}
	}
}

void Manager_enemy::create_enemy_infinite(Manager_map& map, Camera& cam, Manager_hero& hero)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> dist(0, e_index.get_enemy_index_num() - 1);

	if (current_size < max_size && enemy_create_time_elapsed > create_threshold)
	{
		for (unsigned int i = 0; i < max_enemy_num; i++)
		{
			if (enemy[i] == nullptr)
			{
				Position pos = create_out_camera_pos_infinite(map, cam, true);
				unsigned int index = dist(gen);

				enemy[i] = new Enemy(e_index[index].name, e_index[index].type,
					pos.x, pos.y,
					e_index[index].health,
					e_index[index].speed,
					e_index[index].attack,
					e_index[index].attack_cd);
				enemy[i]->load_image();
				current_size++;
				enemy_create_time_elapsed = 0.f;

				// 加快生成速度，但有下限
				create_threshold = max(create_threshold - 0.2f, 0.5f);

				//std::cout << "create enemy at (" << pos.x << ", " << pos.y << ")" << std::endl;
				break;
			}
		}
	}

	if (score > 100 && hero.get_is_upgrade() == false)
	{
		for (unsigned int i = 0; i < max_enemy_num; i++)
		{
			if (enemy[i] == nullptr)
			{
				Position pos = create_out_camera_pos(map, cam, false);
				enemy[i] = new Enemy(e_index[4].name, e_index[4].type, pos.x, pos.y, e_index[4].health,
					e_index[4].speed, e_index[4].attack, e_index[4].attack_cd);
				//enemy_attack_time_elapsed[i] = e_index[index].attack_cd;
				enemy[i]->load_image();
				current_size++;
				hero.set_upgrade(true);
				std::cout << "creat upgrade" << std::endl;
				break;
			}
		}
	}
}


void Manager_enemy::delete_enemy(unsigned int i) 
{
	if (enemy[i]->get_health() <= 0)
	{
		score += e_index[enemy[i]->get_type()].health;
		std::cout << "score: " << score << std::endl;
		delete enemy[i];
		enemy[i] = nullptr;
		current_size--;
		move_status[i] = Move_Status::Front;
		enemy_attack_time_elapsed[i] = 0;
		//std::cout << "Destroyed: " << i << std::endl;
	}
}

void Manager_enemy::update(GamesEngineeringBase::Window& canvas, Manager_map& map, Manager_hero& hero, Camera& cam, float time)
{
	enemy_create_time_elapsed += time;
	update_enemy_attack_time_elapsed(time);
	create_enemy(map, cam, hero);
	
	float speed;
	/*for (unsigned int i = 0; i < max_enemy_num; i++)
	{
		if (enemy[i] != nullptr)
		{
			int x = enemy[i]->get_x();
			int y = enemy[i]->get_y();
			//move_status = Move_Status::Front;
			// TODO: set the speed when hero at slime's southeast and so on to make the move speed equal the
			// horizontal and virtical speed;
			if (hero.get_hit_box_y() < enemy[i]->get_hit_box_y())
			{
				//move_status = Move_Status::Back;
				y -= enemy[i]->get_speed();
			}
			if (hero.get_hit_box_y() > enemy[i]->get_hit_box_y())
			{
				//move_status = Move_Status::Front;
				y += enemy[i]->get_speed();
			}
			if (hero.get_hit_box_x() < enemy[i]->get_hit_box_x())
			{
				move_status[i] = Move_Status::Left;
				x -= enemy[i]->get_speed();
			}
			if (hero.get_hit_box_x() > enemy[i]->get_hit_box_x())
			{
				move_status[i] = Move_Status::Right;
				x += enemy[i]->get_speed();
			}
			if (hero.get_hit_box_x() == enemy[i]->get_hit_box_x() && hero.get_hit_box_y() == enemy[i]->get_hit_box_y())
			{
				move_status[i] = Move_Status::Front;
			}

			x = max(0, x);
			x = min(static_cast<int>(map.get_map_width_pix()) - static_cast<int>(enemy[i] -> get_width()), x);
			y = max(0, y);
			y = min(static_cast<int>(map.get_map_height_pix()) - static_cast<int>(enemy[i] -> get_height()), y);
			enemy[i] -> update(canvas, x, y, move_status[i]);
		}
	}*/


	for (unsigned int i = 0; i < max_enemy_num; i++)
	{
		if (enemy[i] == nullptr) continue;

		if (enemy[i]->get_health() == 0)
		{
			delete_enemy(i);
			continue;
		}
		//The speed follow the time to change
		speed = static_cast<float>(enemy[i]->get_speed()) * time;

		float dx = hero.get_hitbox_x() - enemy[i]->get_hitbox_x();
		//std::cout << hero.get_hitbox_x() << " " << enemy[i]->get_hitbox_x() << std::endl;
		float dy = hero.get_hitbox_y() - enemy[i]->get_hitbox_y();
		float len = sqrt(dx * dx + dy * dy);
		if (dx <= 0)
			move_status[i] = Move_Status::Left;
		else
			move_status[i] = Move_Status::Right;

		if (len > 0.01f)
		{
			dx /= len;
			dy /= len;
		}


		float new_x = enemy[i]->get_hitbox_x() + dx * speed;
		float new_y = enemy[i]->get_hitbox_y() + dy * speed;


		for (int j = 0; j < max_enemy_num; ++j)
		{
			if (i == j || enemy[j] == nullptr) continue;

			float dx2 = new_x - enemy[j]->get_hitbox_x();
			float dy2 = new_y - enemy[j]->get_hitbox_y();
			float dist = sqrt(dx2 * dx2 + dy2 * dy2);
			float minDist = static_cast<float>(enemy[i]->get_hitbox() + enemy[j]->get_hitbox());

			if (dist < minDist && dist > 0.1f)
			{
				float nx = dx2 / dist;
				float ny = dy2 / dist;
				float overlap = (minDist - dist);
				float push = overlap * 0.1f;
				new_x += nx * push;
				new_y += ny * push;
			}
		}

		float dx3 = hero.get_hitbox_x() - new_x;
		float dy3 = hero.get_hitbox_y() - new_y;
		float dist_enemy_to_hero = sqrt(dx3 * dx3 + dy3 * dy3);
		float mid_dist = static_cast<float>(hero.get_hitbox() + enemy[i]->get_hitbox());

		if (dist_enemy_to_hero < mid_dist && dist_enemy_to_hero > 0.1f && hero.get_invincible_time_elapsed() > hero.get_invincible_time()) {
			hero.suffer_attack(e_index[enemy[i]->get_type()].attack);
			hero.zero_invincible_time_elapsed();
		}

		if (dist_enemy_to_hero < mid_dist && enemy[i]->get_type() == Enemy_type::Upgrade)
		{
			hero.upgrade();
			// kill the upgrade object
			enemy[i]->suffer_attack(1);
			delete_enemy(i);
			//std::cout << "delete upgrade " << std::endl;
			break;
		}

		new_x = max(0, min(new_x, static_cast<float>(map.get_map_width_pix() - enemy[i]->get_width())));
		new_y = max(0, min(new_y, static_cast<float>(map.get_map_height_pix() - enemy[i]->get_height())));

		//std::cout << "DEBUG: Enemy " << i
		//	<< " moving from (" << enemy[i]->get_x() << "," << enemy[i]->get_y() << ")"
		//	<< " to (" << new_x << "," << new_y << ")"
		//	<< " direction: (" << dx << "," << dy << ")"
		//	<< " distance: " << len << std::endl;

		enemy[i]->update(canvas, new_x - static_cast<float>(enemy[i]->get_width() / 2),
			new_y - static_cast<float>(enemy[i]->get_height() / 2), move_status[i]);
	}
};

void Manager_enemy::update_infinite(GamesEngineeringBase::Window& canvas, Manager_map& map, Manager_hero& hero, Camera& cam, float time)
{
	enemy_create_time_elapsed += time;
	update_enemy_attack_time_elapsed(time);
	create_enemy_infinite(map, cam, hero);

	float speed;
	float map_width_pix = map.get_map_width_pix();
	float map_height_pix = map.get_map_height_pix();

	for (unsigned int i = 0; i < max_enemy_num; i++)
	{
		if (enemy[i] == nullptr) continue;

		if (enemy[i]->get_health() == 0)
		{
			delete_enemy(i);
			continue;
		}

		// hero's forward
		float dx = hero.get_hitbox_x() - enemy[i]->get_hitbox_x();
		float dy = hero.get_hitbox_y() - enemy[i]->get_hitbox_y();

		// check the shortest distance in infinite map
		if (fabs(dx) > map_width_pix / 2)
		{
			if (dx > 0) dx -= map_width_pix;
			else dx += map_width_pix;
		}

		if (fabs(dy) > map_height_pix / 2)
		{
			if (dy > 0) dy -= map_height_pix;
			else dy += map_height_pix;
		}

		float len = sqrt(dx * dx + dy * dy);

		// set forward
		move_status[i] = (dx <= 0) ? Move_Status::Left : Move_Status::Right;

		if (len > 0.01f)
		{
			dx /= len;
			dy /= len;
		}

		speed = static_cast<float>(enemy[i]->get_speed()) * time;

		float new_x = enemy[i]->get_hitbox_x() + dx * speed;
		float new_y = enemy[i]->get_hitbox_y() + dy * speed;

		// bounce between enemies
		for (int j = 0; j < max_enemy_num; ++j)
		{
			if (i == j || enemy[j] == nullptr) continue;

			float dx2 = new_x - enemy[j]->get_hitbox_x();
			float dy2 = new_y - enemy[j]->get_hitbox_y();

			// also check the shortest distance in infinite map
			if (fabs(dx2) > map_width_pix / 2)
			{
				if (dx2 > 0) dx2 -= map_width_pix;
				else dx2 += map_width_pix;
			}
			if (fabs(dy2) > map_height_pix / 2)
			{
				if (dy2 > 0) dy2 -= map_height_pix;
				else dy2 += map_height_pix;
			}

			float dist = sqrt(dx2 * dx2 + dy2 * dy2);
			float minDist = static_cast<float>(enemy[i]->get_hitbox() + enemy[j]->get_hitbox());

			if (dist < minDist && dist > 0.1f)
			{
				float nx = dx2 / dist;
				float ny = dy2 / dist;
				float overlap = (minDist - dist);
				float push = overlap * 0.1f;
				new_x += nx * push;
				new_y += ny * push;
			}
		}

		// check collision
		float dx3 = hero.get_hitbox_x() - new_x;
		float dy3 = hero.get_hitbox_y() - new_y;

		if (fabs(dx3) > map_width_pix / 2)
		{
			if (dx3 > 0) dx3 -= map_width_pix;
			else dx3 += map_width_pix;
		}

		if (fabs(dy3) > map_height_pix / 2)
		{
			if (dy3 > 0) dy3 -= map_height_pix;
			else dy3 += map_height_pix;
		}

		float dist_enemy_to_hero = sqrt(dx3 * dx3 + dy3 * dy3);
		float mid_dist = static_cast<float>(hero.get_hitbox() + enemy[i]->get_hitbox());

		if (dist_enemy_to_hero < mid_dist && dist_enemy_to_hero > 0.1f &&
			hero.get_invincible_time_elapsed() > hero.get_invincible_time())
		{
			hero.suffer_attack(e_index[enemy[i]->get_type()].attack);
			hero.zero_invincible_time_elapsed();
		}

		if (dist_enemy_to_hero < mid_dist && enemy[i]->get_type() == Enemy_type::Upgrade)
		{
			hero.upgrade();
			enemy[i]->suffer_attack(1);
			delete_enemy(i);
			std::cout << "delete upgrade " << std::endl;
			break;
		}

		// reset the location
		new_x = fmod(new_x, map_width_pix);
		new_y = fmod(new_y, map_height_pix);
		if (new_x < 0) new_x += map_width_pix;
		if (new_y < 0) new_y += map_height_pix;

		enemy[i]->update(canvas,
			new_x - static_cast<float>(enemy[i]->get_width() / 2),
			new_y - static_cast<float>(enemy[i]->get_height() / 2),
			move_status[i]);
	}
}


void Manager_enemy::save_enemy_state(const std::string& filename)
{
	std::ofstream file(filename, std::ios::out | std::ios::app);
	if (!file.is_open()) {
		std::cerr << "[Error] Cannot open save file: " << filename << std::endl;
		return;
	}

	file << "Enemy States" << std::endl
		<< current_size << " " << score << " "
		<< enemy_create_time_elapsed << std::endl;

	for (unsigned int i = 0; i < max_enemy_num; i++) {
		if (enemy[i] == nullptr) continue;

		file 
			<< enemy[i]->get_x() << " "
			<< enemy[i]->get_y() << " "
			<< enemy[i]->get_hitbox() << " "
			<< enemy[i]->get_hitbox_x() << " "
			<< enemy[i]->get_hitbox_y() << " "
			<< enemy[i]->get_name() << " "
			<< static_cast<int>(enemy[i]->get_type()) << " "
			<< enemy[i]->get_health() << " "
			<< enemy[i]->get_speed() << " "
			<< enemy[i]->get_attack() << " "
			<< enemy[i]->get_attack_cd() << " "
			<< static_cast<int>(move_status[i]) << " "
			<< enemy_attack_time_elapsed[i] << std::endl;
	}

	file.close();
	std::cout << "[Save] Enemies saved to " << filename << " (" << current_size << " units)" << std::endl;
}

void Manager_enemy::load_enemy_state(const std::string filename)
{
	std::ifstream file(filename, std::ios::in);
	if (!file.is_open()) {
		std::cerr << "[Error] Cannot open load file: " << filename << std::endl;
		return;
	}

	for (unsigned int i = 0; i < max_enemy_num; i++)
	{
		if(current_size != 0)
		{
			if (enemy[i] == nullptr)
				continue;

			delete enemy[i];
			enemy[i] = nullptr;
			move_status[i] = Move_Status::Front;
			enemy_attack_time_elapsed[i] = 0;
			current_size--;
		}
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::cout << line << std::endl;
		if (line.find("Enemy States") != std::string::npos)
			break;
	}

	file >> current_size >> score >>create_threshold;

	for (unsigned int i = 0; i < current_size; i++)
	{
		float x, y, hb, hb_x, hb_y;
		int health;
		unsigned int attack, attack_cd, speed, enemy_type;
		std::string enemy_name;
		int m_status;
		
		file >> x >> y >> hb >> hb_x >> hb_y
			>> enemy_name >> enemy_type >> health
			>> speed >> attack >> attack_cd
			>> m_status >> enemy_attack_time_elapsed[i];

		move_status[i] = static_cast<Move_Status>(m_status);
		enemy[i] = new Enemy(enemy_name, static_cast<Enemy_type>(enemy_type), x, y, health, speed, attack, attack_cd);
		enemy[i]->load_image();
	}


	file.close();


	std::cout << "[Load] Enemy state loaded from " << filename << std::endl;

}

void Manager_enemy::draw(GamesEngineeringBase::Window& canvas, Manager_map& map, Camera& cam)
{
	for (unsigned int i = 0; i < max_enemy_num; i++)
	{
		if (enemy[i] != nullptr)
		{
			enemy[i]->draw(canvas, static_cast<int>(cam.mapx_to_camerax(enemy[i]->get_x())), static_cast<int>(cam.mapy_to_cameray(enemy[i]->get_y())));
			//std::cout << "Draw "  << std::endl;
		}
	}
}

void Manager_enemy::draw_infinite(GamesEngineeringBase::Window& canvas, Manager_map& map, Camera& cam)
{
	float mapW = map.get_map_width_pix();
	float mapH = map.get_map_height_pix();

	for (unsigned int i = 0; i < max_enemy_num; i++)
	{
		if (enemy[i] == nullptr) continue;

		float ex = enemy[i]->get_x();
		float ey = enemy[i]->get_y();

		// 让敌人在相邻区域（-1,0,1）都绘制一次，实现环绕
		for (int dx = -1; dx <= 1; dx++)
		{
			for (int dy = -1; dy <= 1; dy++)
			{
				float draw_x = ex + dx * mapW;
				float draw_y = ey + dy * mapH;

				// 可见性判断（避免全图都画）
				float screen_x = cam.mapx_to_camerax(draw_x);
				float screen_y = cam.mapy_to_cameray(draw_y);

				if (screen_x + enemy[i]->get_width() < 0 ||
					screen_y + enemy[i]->get_height() < 0 ||
					screen_x > cam.get_cam_width() ||
					screen_y > cam.get_cam_height())
				{
					continue; // 不在屏幕范围内则跳过
				}

				enemy[i]->draw(canvas, static_cast<int>(screen_x), static_cast<int>(screen_y));
			}
		}
	}
}


Manager_bullet::Manager_bullet(GamesEngineeringBase::Window& canvas)
{
	for (unsigned int i = 0; i < max_bullet_num; i++)
	{
		bullet[i] = nullptr;
		move_status[i] = Move_Status::Front;
		forward[i] = { 0, 0 };
	}
}

void Manager_bullet::create_bullet(std::string name, Bullet_type ty, Unit_Type fr)
{

	if (current_size < max_bullet_num)

		for (unsigned int i = 0; i < max_bullet_num; i++)
		{
			if (bullet[i] == nullptr)
			{
				bullet[i] = new Bullet(name, ty, fr);
				bullet[i]->load_image();
				current_size++;
				break;
			}
		}
}

void Manager_bullet::delete_bullet(unsigned int i)
{
	delete bullet[i];
	bullet[i] = nullptr;
	current_size--;
	move_status[i] = Move_Status::Front;
	forward[i] = { 0, 0 };
}

void Manager_bullet::create_hero_bullet(Manager_hero& hero, Manager_enemy& enemy)
{
	if (hero.get_attack_elapsed() > hero.get_attack_cd())
	{
		for (unsigned int i = 0; i < max_bullet_num; i++)
		{
			if (bullet[i] == nullptr)
			{
				bullet[i] = new Bullet("Blue", Bullet_type::Blue, Unit_Type::Hero, 
					hero.get_hitbox_x(), hero.get_hitbox_y(), 250, hero.get_hero_attack());
				bullet[i]->load_image();
				current_size++;
				//std::cout << hero.get_hitbox_x() << hero.get_hitbox_y() << std::endl;
				hero.zero_attack_elapsed();
				Position fw = set_forward(i, enemy);
				forward[i] = fw;
				//std::cout << "create hero bullet" << std::endl;
				//std::cout << "create bullet pos: " << bullet[i]->get_hitbox_x() << "," << bullet[i]->get_hitbox_y() << std::endl;
				break;
			}
		}
	}
}

/*void Manager_bullet::create_hero_aoe_bullet_(Manager_hero& hero, Manager_enemy& enemy)
{
	if (hero.get_aoe_elapsed() > hero.get_aoe_cd())
	{
		unsigned int current_aoe_bullet = 0;
		My_Stack<unsigned int> max_health_enemy_index;
		float max_health = 0;
		unsigned int max_health_enemy = NULL;
		for (unsigned int i = 0; i < hero.get_aoe_num(); i++)
		{
			for (unsigned int j = 0; j < max_enemy_num; j++)
			{
				if (enemy[j] != nullptr)
				{
					float dx = enemy.get_hit_box_x(j) - hero.get_hitbox_x();
					float dy = enemy.get_hit_box_y(j) - hero.get_hitbox_y();
					float dist = sqrt(dx * dx + dy * dy);

					if (dist < hero.get_aoe_range() && current_aoe_bullet < hero.get_aoe_num())
					{
						if (enemy[j]->get_health() > max_health && !max_health_enemy_index.find(j))
						{
							max_health = enemy[j]->get_health();
						}
					}
				}

			}
			if (max_health_enemy != NULL)
			{
				max_health_enemy_index.push(max_health_enemy);
				current_aoe_bullet++;
			}
			max_health = 0;
		}

		while(max_health_enemy_index.get_size() != 0)
		{
			for (unsigned int i = 0; i < max_bullet_num; i++)
			{
				if (bullet[i] == nullptr)
				{
					bullet[i] = new Bullet("Light", Bullet_type::Light, Unit_Type::Hero,
						enemy[max_health_enemy_index.get_head()]->get_hitbox_x(),
						enemy[max_health_enemy_index.get_head()]->get_hitbox_y());
					max_health_enemy_index.pop();
					current_aoe_bullet--;
					bullet[i]->load_image();
					//std::cout << hero.get_hitbox_x() << hero.get_hitbox_y() << std::endl;
					//std::cout << "create hero bullet" << std::endl;
					//std::cout << "create bullet pos: " << bullet[i]->get_hitbox_x() << "," << bullet[i]->get_hitbox_y() << std::endl;d
				}
				break;
			}
		}
		hero.zero_aoe_elapsed();
		std::cout << "creat aoe" << std::endl;
	}
}*/

void Manager_bullet::create_hero_aoe_bullet(Manager_hero& hero, Manager_enemy& enemy, Manager_map& map)
{
	//std::cout << "[AOE] === create_hero_aoe_bullet() called ===" << std::endl;

	if (hero.get_aoe_elapsed() > hero.get_aoe_cd())
	{
		std::cout << "AOE!!!" << std::endl;

		unsigned int current_aoe_bullet = 0;
		My_Stack<unsigned int> max_health_enemy_index;
		float max_health = 0.f;
		unsigned int max_health_enemy = 1e9f;

		unsigned int aoe_num = hero.get_aoe_num();
		//std::cout << "[AOE] Target AOE bullet num: " << aoe_num
		//	<< ", Range = " << hero.get_aoe_range() << std::endl;

		// === find biggest health enemy index ===
		for (unsigned int i = 0; i < aoe_num; i++)
		{
			max_health = 0.f;
			max_health_enemy = 1e9f;

			for (unsigned int j = 0; j < max_enemy_num; j++)
			{
				if (enemy[j] != nullptr && enemy[j]->get_type() != Enemy_type::Upgrade)
				{
					float dx = enemy.get_hit_box_x(j) - hero.get_hitbox_x();
					float dy = enemy.get_hit_box_y(j) - hero.get_hitbox_y();

					if (fabs(dx) > map.get_map_width_pix() / 2)
					{
						if (dx > 0) dx -= map.get_map_width_pix();
						else dx += map.get_map_width_pix();
					}
					if (fabs(dy) > map.get_map_height_pix() / 2)
					{
						if (dy > 0) dy -= map.get_map_height_pix();
						else dy += map.get_map_height_pix();
					}

					float dist = sqrt(dx * dx + dy * dy);

					if (dist < hero.get_aoe_range() && current_aoe_bullet < aoe_num)
					{
						float hp = enemy[j]->get_health();
						if (hp > max_health && !max_health_enemy_index.find(j))
						{
							max_health = hp;
							max_health_enemy = j;
						}
					}
				}
			}

			if (max_health_enemy != 1e9f)
			{
				max_health_enemy_index.push(max_health_enemy);
				current_aoe_bullet++;
				//std::cout << "[AOE] Selected enemy #" << max_health_enemy
				//	<< " with health=" << max_health
				//	<< " | total selected=" << current_aoe_bullet << std::endl;
			}
			//else
			//{
			//	std::cout << "[AOE] No valid enemy found in range for bullet #" << i << std::endl;
			//}
		}

		// === create bullet ===
		//std::cout << "[AOE] Begin creating bullets, total targets = "
			//<< max_health_enemy_index.get_size() << std::endl;

		while (max_health_enemy_index.get_size() != 0)
		{
			unsigned int target_index = max_health_enemy_index.get_head();
			max_health_enemy_index.pop();

			bool bullet_created = false;
			for (unsigned int i = 0; i < max_bullet_num; i++)
			{
				if (bullet[i] == nullptr)
				{
					bullet[i] = new Bullet(
						"Light",
						Bullet_type::Light,
						Unit_Type::Hero,
						enemy[target_index]->get_x(),
						enemy[target_index]->get_y());

					bullet[i]->load_image();
					current_size++;
					current_aoe_bullet--;

					//std::cout << "[AOE] Created bullet at enemy #" << target_index
					//	<< " pos=(" << enemy[target_index]->get_hitbox_x()
					//	<< "," << enemy[target_index]->get_hitbox_y() << ")"
					//	<< " | Remaining bullets=" << current_aoe_bullet << std::endl;

					bullet_created = true;
					break;
				}
			}

			//if (!bullet_created)
				//std::cout << "[AOE] WARNING: bullet pool full, could not create bullet!" << std::endl;
		}

		hero.zero_aoe_elapsed();
		//std::cout << "[AOE] === AOE creation complete ===" << std::endl;
	}
	//else
	//{
	//	std::cout << "[AOE] Cooldown not ready. Elapsed=" << hero.get_aoe_elapsed()
	//		<< " / CD=" << hero.get_aoe_cd() << std::endl;
	//}
}

void Manager_bullet::check_delete_Light(unsigned int bullet_index, Manager_enemy& enemy, float time)
{
	Position nearest = { 0, 0 };
	unsigned int enemy_index = 0;
	float min_dist = 1e9f;
	float speed = 0;
	float mindx = 0;
	float mindy = 0;
	float hitbox = 0;
	for (unsigned int i = 0; i < max_enemy_num; i++)
	{
		if (enemy[i] != nullptr)
		{
			float dx = enemy.get_hit_box_x(i) - bullet[bullet_index]->get_hitbox_x();
			float dy = enemy.get_hit_box_y(i) - bullet[bullet_index]->get_hitbox_y();
			float dist = sqrt(dx * dx + dy * dy);
			if (dist < min_dist)
			{
				nearest = { enemy[i]->get_hitbox_x(), enemy[i]->get_hitbox_y() };
				enemy_index = i;
				mindx = dx;
				mindy = dy;
				min_dist = dist;
				hitbox = bullet[bullet_index]->get_hitbox() + enemy[i]->get_hitbox();
			}
		}
	}

	if (min_dist > hitbox)
	{
		if (aoe_Light_render_start > 0.5f)
			delete_bullet(bullet_index);
	}
	else
	{
		enemy.suffer_attack(enemy_index, bullet[bullet_index]->get_attack());
		if (aoe_Light_render_start > 0.5f)
			delete_bullet(bullet_index);
	}
	aoe_Light_render_start += time;
}

void Manager_bullet::create_enemy_bullet(Manager_enemy& enemy)
{

	for (unsigned int j = 0; j < max_enemy_num; j++)
	{
		if (enemy[j] == nullptr)
			continue;
		if (enemy[j]->get_type() == Enemy_type::FlySpookmoth)
		{
			//std::cout << "enemy_attack_time_elapsed" << j << ":" << enemy.get_enemy_attack_time_elapsed(j) << std::endl;
			if (enemy.get_enemy_attack_time_elapsed(j) < enemy[j]->get_attack_cd())
				continue;
			for (unsigned int i = 0; i < max_bullet_num; i++)
			{
				if (bullet[i] != nullptr)
					continue;
				bullet[i] = new Bullet("Red", Bullet_type::Red, Unit_Type::Enenmy,
					enemy[j]->get_hitbox_x(), enemy[j]->get_hitbox_y(), 4, enemy[j]->get_attack());
				bullet[i]->load_image();
				current_size++;
				//std::cout << hero.get_hitbox_x() << hero.get_hitbox_y() << std::endl;
				//std::cout << "create enemy bullet" << std::endl;
				//std::cout << "create bullet pos: " << bullet[i]->get_hitbox_x() << "," << bullet[i]->get_hitbox_y() << std::endl;
				break;
			}
			
		}
		//std::cout << "time elapsed " << enemy.get_enemy_attack_time_elapsed() << std::endl;
		enemy.zero_enemy_attack_time_elapsed(j);
	}
}

Position Manager_bullet::set_forward(unsigned int bullet_index, Manager_enemy& enemy)
{
	//Position speed = { 0, 0 };
	//unsigned int enemy_index = 0;
	float min_dist = 1e9f;
	//float speed = 0;
	float mindx = 0;
	float mindy = 0;
	//float hitbox = 0;
	for (unsigned int i = 0; i < max_enemy_num; i++)
	{
		if (enemy[i] != nullptr)
		{
			float dx = enemy.get_hit_box_x(i) - bullet[bullet_index]->get_hitbox_x();
			float dy = enemy.get_hit_box_y(i) - bullet[bullet_index]->get_hitbox_y();
			float dist = sqrt(dx * dx + dy * dy);
			if (dist < min_dist)
			{
				//nearest = { enemy[i]->get_hitbox_x(), enemy[i]->get_hitbox_y() };
				//enemy_index = i;
				mindx = dx;
				mindy = dy;
				min_dist = dist;
				//hitbox = bullet[bullet_index]->get_hitbox() + enemy[i]->get_hitbox();
			}
		}
	}

	//float speed = max(static_cast<float>(bullet[bullet_index]->get_speed()) * time, 1.0f);

	if (min_dist > 0.01f)
	{
		mindx /= min_dist;
		mindy /= min_dist;
	}

	return { mindx, mindy };
}

// this function is not very safe,it could be improve
void Manager_bullet::keep_move_to_enemy(unsigned int bullet_index, Manager_enemy& enemy, float time)
{
	//Position nearest = { 0, 0 };
	if (bullet[bullet_index] == nullptr)
		return;

	unsigned int enemy_index = 0;
	float min_dist = 1e9f;
	float speed = 0;
	float hitbox = 0;
	for (unsigned int i = 0; i < max_enemy_num; i++)
	{
		if (enemy[i] != nullptr && enemy[i]->get_type() != Enemy_type::Upgrade)
		{
			float dx = enemy.get_hit_box_x(i) - bullet[bullet_index]->get_hitbox_x();
			float dy = enemy.get_hit_box_y(i) - bullet[bullet_index]->get_hitbox_y();
			float dist = sqrt(dx * dx + dy * dy);
			if (dist < min_dist)
			{
				//nearest = { enemy[i]->get_hitbox_x(), enemy[i]->get_hitbox_y() };
				enemy_index = i;
				min_dist = dist;
				hitbox = bullet[bullet_index]->get_hitbox() + enemy[i]->get_hitbox();
			}
		}
	}
	//std::cout << "12" << std::endl;
	//if (min_dist == 1e9f)
	//hitbox = bullet[bullet_index]->get_hitbox() + hitbox;
	//std::cout << "34" << std::endl;

	if (min_dist > hitbox)
	{

		speed = max(static_cast<float>(bullet[bullet_index]->get_speed()) * time, 1.0f);

		//if (mindx <= 0)
		//	move_status[i] = Move_Status::Left;
		//else
		//	move_status[i] = Move_Status::Right;

		bullet[bullet_index]->update(bullet[bullet_index]->get_hitbox_x() + forward[bullet_index].x * speed - static_cast<float>(bullet[bullet_index]->get_width() / 2),
			bullet[bullet_index]->get_hitbox_y() + forward[bullet_index].y * speed - static_cast<float>(bullet[bullet_index]->get_height() / 2));
	}
	else
	{
		enemy.suffer_attack(enemy_index, bullet[bullet_index]->get_attack());
		delete_bullet(bullet_index);
	}

	//return { pos.x + mindx * speed, pos.y + mindy * speed };
}

void Manager_bullet::move_to_nearest_hero(unsigned int bullet_index, Manager_hero& hero, float time)
{
	float dx = hero.get_hitbox_x() - bullet[bullet_index]->get_hitbox_x();
	float dy = hero.get_hitbox_y() - bullet[bullet_index]->get_hitbox_y();
	float dist = sqrt(dx * dx + dy * dy);

	if (dist > hero.get_hitbox())
	{

		float speed = max(static_cast<float>(bullet[bullet_index]->get_speed()) * time, 1.0f);

		//if (mindx <= 0)
		//	move_status[i] = Move_Status::Left;
		//else
		//	move_status[i] = Move_Status::Right;

		if (dist > 0.01f)
		{
			dx /= dist;
			dy /= dist;
		}

		bullet[bullet_index]->update(bullet[bullet_index]->get_hitbox_x() + dx * speed - static_cast<float>(bullet[bullet_index]->get_width() / 2),
			bullet[bullet_index]->get_hitbox_y() + dy * speed - static_cast<float>(bullet[bullet_index]->get_height() / 2));
	}
	else
	{
		hero.suffer_attack(bullet[bullet_index]->get_attack());
		delete_bullet(bullet_index);
	}
}

void Manager_bullet::move_to_nearest_hero_infinite(unsigned int bullet_index, Manager_hero& hero, float time, Manager_map& map)
{
	float dx = hero.get_hitbox_x() - bullet[bullet_index]->get_hitbox_x();
	float dy = hero.get_hitbox_y() - bullet[bullet_index]->get_hitbox_y();

	if (fabs(dx) > map.get_map_width_pix() / 2)
	{
		if (dx > 0) dx -= map.get_map_width_pix();
		else dx += map.get_map_width_pix();
	}
	if (fabs(dy) > map.get_map_height_pix() / 2)
	{
		if (dy > 0) dy -= map.get_map_height_pix();
		else dy += map.get_map_height_pix();
	}
	float dist = sqrt(dx * dx + dy * dy);

	if (dist > hero.get_hitbox())
	{

		float speed = max(static_cast<float>(bullet[bullet_index]->get_speed()) * time, 1.0f);

		//if (mindx <= 0)
		//	move_status[i] = Move_Status::Left;
		//else
		//	move_status[i] = Move_Status::Right;

		if (dist > 0.01f)
		{
			dx /= dist;
			dy /= dist;
		}

		bullet[bullet_index]->update(bullet[bullet_index]->get_hitbox_x() + dx * speed - static_cast<float>(bullet[bullet_index]->get_width() / 2),
			bullet[bullet_index]->get_hitbox_y() + dy * speed - static_cast<float>(bullet[bullet_index]->get_height() / 2));
	}
	else
	{
		hero.suffer_attack(bullet[bullet_index]->get_attack());
		delete_bullet(bullet_index);
	}
}

void Manager_bullet::move_to_nearest_enemy(unsigned int bullet_index, Manager_enemy& enemy, float time)
{
	Position nearest = { 0, 0 };
	unsigned int enemy_index = 0;
	float min_dist = 1e9f;
	float speed = 0;
	float mindx = 0;
	float mindy = 0;
	float hitbox = 0;
	for (unsigned int i = 0; i < max_enemy_num; i++)
	{
		if (enemy[i] != nullptr)
		{
			float dx = enemy.get_hit_box_x(i) - bullet[bullet_index]->get_hitbox_x();
			float dy = enemy.get_hit_box_y(i) - bullet[bullet_index]->get_hitbox_y();
			float dist = sqrt(dx * dx + dy * dy);
			if (dist < min_dist)
			{
				nearest = { enemy[i]->get_hitbox_x(), enemy[i]->get_hitbox_y() };
				enemy_index = i;
				mindx = dx;
				mindy = dy;
				min_dist = dist;
				hitbox = bullet[bullet_index]->get_hitbox() + enemy[i]->get_hitbox();
			}
		}
	}

	if (min_dist > hitbox )
	{

		speed = max(static_cast<float>(bullet[bullet_index]->get_speed()) * time, 1.0f);

		//if (mindx <= 0)
		//	move_status[i] = Move_Status::Left;
		//else
		//	move_status[i] = Move_Status::Right;

		if (min_dist > 0.01f)
		{
			mindx /= min_dist;
			mindy /= min_dist;
		}

		bullet[bullet_index]->update(bullet[bullet_index]->get_hitbox_x() + mindx * speed - static_cast<float>(bullet[bullet_index]->get_width() / 2),
			bullet[bullet_index]->get_hitbox_y() + mindy * speed - static_cast<float>(bullet[bullet_index]->get_height() / 2));
	}
	else
	{
		enemy.suffer_attack(enemy_index, bullet[bullet_index]->get_attack());
		delete_bullet(bullet_index);
	}
}

void Manager_bullet::update(GamesEngineeringBase::Window& canvas, Manager_map& map, Manager_hero& hero,
	Manager_enemy& enemy, Camera& cam, float time)
{
	if (enemy.get_enemy_current_num() > 0)
		this->create_hero_bullet(hero, enemy);
	this->create_enemy_bullet(enemy);

	
	for (unsigned int i = 0; i < max_bullet_num; i++)
	{
		if (bullet[i] != nullptr)
		{
			//check if bullet is fly out the map
			if (bullet[i]->get_hitbox_x() < 0 || bullet[i]->get_hitbox_x() > map.get_map_width_pix() ||
				bullet[i]->get_hitbox_y() < 0 || bullet[i]->get_hitbox_y() > map.get_map_height_pix())
			{
				delete_bullet(i);
				continue;
			}
			if (bullet[i]->get_health() <= 0)
			{
				delete_bullet(i);
				continue;
			}
			if (bullet[i]->get_from() == Unit_Type::Hero && bullet[i]->get_type() == Bullet_type::Blue)
			{
				keep_move_to_enemy(i, enemy, time);
				continue;
			}
			if (bullet[i]->get_from() == Unit_Type::Enenmy)
			{
				bullet[i]->sub_health(time);
				move_to_nearest_hero(i, hero, time);
				continue;
			}
			if (bullet[i]->get_from() == Unit_Type::Hero && bullet[i]->get_type() == Bullet_type::Light)
			{
				check_delete_Light(i, enemy, time);
				continue;
			}
		}
	}

	if (canvas.keyPressed('J'))
	{
		create_hero_aoe_bullet(hero, enemy, map);
		std::cout << "AOE!" << std::endl;
	}
}

void Manager_bullet::update_infinite(GamesEngineeringBase::Window& canvas, Manager_map& map, Manager_hero& hero,
	Manager_enemy& enemy, Camera& cam, float time)
{
	if (enemy.get_enemy_current_num() > 0)
		this->create_hero_bullet(hero, enemy);
	this->create_enemy_bullet(enemy);


	for (unsigned int i = 0; i < max_bullet_num; i++)
	{
		if (bullet[i] != nullptr)
		{
			//check if bullet is fly out the map
			//if (bullet[i]->get_hitbox_x() < 0 || bullet[i]->get_hitbox_x() > map.get_map_width_pix() ||
			//	bullet[i]->get_hitbox_y() < 0 || bullet[i]->get_hitbox_y() > map.get_map_height_pix())
			//{
			//	delete_bullet(i);
			//	continue;
			//}
			if (bullet[i]->get_health() <= 0)
			{
				delete_bullet(i);
				continue;
			}
			if (bullet[i]->get_from() == Unit_Type::Hero && bullet[i]->get_type() == Bullet_type::Blue)
			{
				keep_move_to_enemy(i, enemy, time);
				continue;
			}
			if (bullet[i]->get_from() == Unit_Type::Enenmy)
			{
				bullet[i]->sub_health(time);
				move_to_nearest_hero_infinite(i, hero, time, map);
				continue;
			}
			if (bullet[i]->get_from() == Unit_Type::Hero && bullet[i]->get_type() == Bullet_type::Light)
			{
				check_delete_Light(i, enemy, time);
				continue;
			}
		}
	}

	if (canvas.keyPressed('J'))
	{
		create_hero_aoe_bullet(hero, enemy, map);
		std::cout << "AOE!" << std::endl;
	}
}

void Manager_bullet::save_bullet_state(const std::string& filename)
{
	std::ofstream file(filename, std::ios::out | std::ios::app);
	if (!file.is_open()) {
		std::cerr << "[Error] Cannot open save file: " << filename << std::endl;
		return;
	}

	file << "Bullet States" << std::endl
		<< current_size << " "
		<< aoe_Light_render_start << std::endl;

	for (unsigned int i = 0; i < max_bullet_num; i++) {
		if (bullet[i] == nullptr) continue;

		file
			<< bullet[i]->get_x() << " "
			<< bullet[i]->get_y() << " "
			<< bullet[i]->get_hitbox() << " "
			<< bullet[i]->get_hitbox_x() << " "
			<< bullet[i]->get_hitbox_y() << " "
			<< bullet[i]->get_name() << " "
			<< static_cast<int>(bullet[i]->get_type()) << " "
			<< static_cast<int>(bullet[i]->get_from()) << " "
			<< bullet[i]->get_health() << " "
			<< bullet[i]->get_speed() << " "
			<< bullet[i]->get_attack() << " "
			<< static_cast<int>(move_status[i]) << " "
			<< forward[i].x << " "
			<< forward[i].y << std::endl;
	}

	file.close();
	std::cout << "[Save] Bullet saved to " << filename << " (" << current_size << " units)" << std::endl;
}

void Manager_bullet::load_bullet_state(const std::string& filename)
{
	std::ifstream file(filename, std::ios::in);
	if (!file.is_open()) {
		std::cerr << "[Error] Cannot open load file: " << filename << std::endl;
		return;
	}

	for (unsigned int i = 0; i < max_bullet_num; i++)
	{
		if (current_size != 0)
		{
			if (bullet[i] == nullptr)
				continue;

			delete bullet[i];
			bullet[i] = nullptr;
			move_status[i] = Move_Status::Front;
			forward[i] = { 0, 0 };
			current_size--;
		}
	}

	std::string line;
	while (std::getline(file, line))
	{
		std::cout << line << std::endl;
		if (line.find("Bullet States") != std::string::npos)
			break;
	}

	file >> current_size >> aoe_Light_render_start;

	for (unsigned int i = 0; i < current_size; i++)
	{
		float x, y, hb, hb_x, hb_y;
		float for_x, for_y;
		int health;
		unsigned int attack, speed, bullet_type, bullet_from;
		std::string bullet_name;
		int m_status;

		file >> x >> y >> hb >> hb_x >> hb_y
			>> bullet_name >> bullet_type >> bullet_from 
			>> health >> speed >> attack 
			>> m_status >> for_x >> for_y;

		move_status[i] = static_cast<Move_Status>(m_status);
		bullet[i] = new Bullet(bullet_name, static_cast<Bullet_type>(bullet_type), static_cast<Unit_Type>(bullet_from),
			x, y, speed, attack);
		bullet[i]->load_image();
		forward[i] = { for_x , for_y };
	}


	file.close();

	std::cout << "[Load] Bullet state loaded from " << filename << std::endl;

}

void Manager_bullet::draw(GamesEngineeringBase::Window& canvas, Manager_map& map, Camera& cam)
{
	for (unsigned int i = 0; i < max_bullet_num; i++)
	{
		if (bullet[i] != nullptr)
		{
			bullet[i]->draw(canvas, static_cast<int>(cam.mapx_to_camerax(bullet[i]->get_x())), static_cast<int>(cam.mapy_to_cameray(bullet[i]->get_y())));
			//std::cout << "Draw "  << std::endl;
		}
	}
}

void Manager_bullet::draw_infinite(GamesEngineeringBase::Window& canvas, Manager_map& map, Camera& cam)
{
	float mapW = map.get_map_width_pix();
	float mapH = map.get_map_height_pix();

	for (unsigned int i = 0; i < max_enemy_num; i++)
	{
		if (bullet[i] == nullptr) continue;

		float ex = bullet[i]->get_x();
		float ey = bullet[i]->get_y();

		// 让敌人在相邻区域（-1,0,1）都绘制一次，实现环绕
		for (int dx = -1; dx <= 1; dx++)
		{
			for (int dy = -1; dy <= 1; dy++)
			{
				float draw_x = ex + dx * mapW;
				float draw_y = ey + dy * mapH;

				// 可见性判断（避免全图都画）
				float screen_x = cam.mapx_to_camerax(draw_x);
				float screen_y = cam.mapy_to_cameray(draw_y);

				if (screen_x + bullet[i]->get_width() < 0 ||
					screen_y + bullet[i]->get_height() < 0 ||
					screen_x > cam.get_cam_width() ||
					screen_y > cam.get_cam_height())
				{
					continue; // 不在屏幕范围内则跳过
				}

				bullet[i]->draw(canvas, static_cast<int>(screen_x), static_cast<int>(screen_y));
			}
		}
	}
}