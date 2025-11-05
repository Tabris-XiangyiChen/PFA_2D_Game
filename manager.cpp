#include "manager.h"

void Manager_hero::update(GamesEngineeringBase::Window& canvas, Manager_map& map, float time) 
{
	update_cd(time);
	if(!Hero.check_if_dead())
	{
		//int x = 0;
		//int y = 0;
		float x = Hero.get_x();
		float y = Hero.get_y();
		//int speed = static_cast<int>(max(static_cast<float>(Hero.get_speed() * 50) * time, 1.0f));
		float speed = max(static_cast<float>(Hero.get_speed()) * time, 1.0f);

		//std::cout << "speed is " << speed << "the time is " << time << std::endl;

		//move_status = Move_Status::Front;
		if (canvas.keyPressed('W'))
		{
			move_status = Move_Status::Back;
			//y -= Hero.get_speed();
			y -= speed;
		}
		if (canvas.keyPressed('S'))
		{
			move_status = Move_Status::Front;
			y += speed;
		}
		if (canvas.keyPressed('A'))
		{
			move_status = Move_Status::Left;
			x -= speed;
		}
		if (canvas.keyPressed('D'))
		{
			move_status = Move_Status::Right;
			x += speed;
		}
		x = max(0, x);
		x = min(static_cast<int>(map.get_map_width_pix()) - static_cast<int>(Hero.get_width()), x);
		y = max(0, y);
		y = min(static_cast<int>(map.get_map_height_pix()) - static_cast<int>(Hero.get_height()), y);
		Hero.update(canvas, x, y, move_status);
	}
	else 
	{
		move_status = Move_Status::Dead;
		Hero.update(move_status);
	}

}

void Manager_hero::draw(GamesEngineeringBase::Window& canvas, Manager_map& map, Camera& cam)
{
	if (cam.get_is_at_boundry())
	{
		Hero.draw(canvas, static_cast<int>(cam.mapx_to_camerax(Hero.get_x())), static_cast<int>(cam.mapy_to_cameray(Hero.get_y())));
	}
	else
		Hero.draw_incenter(canvas);
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

Manager_map::Manager_map(GamesEngineeringBase::Window& canvas) : map(), tiles()
{
	tiles.tiles_init("map");
	map.load_map("./Resource/map/tiles2.txt");
	//cam.camera_init(canvas, map.get_map_width() * map.get_tiles_width(),
	//	map.get_map_height() * map.get_tiles_height());

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

Manager_enemy::Manager_enemy(GamesEngineeringBase::Window& canvas)
{
	for (unsigned int i = 0; i < max_enemy_num; i++)
	{
		enemy[i] = nullptr;
		move_status[i] = Move_Status::Front;
		enemy_attack_time_elapsed[i] = 0;
	}
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

void Manager_enemy::create_enemy(Manager_map& map, Camera& cam)
{
	//If only use srand(),it sometimes happen the continuous enemy are the same;
	static std::random_device rd;         
	static std::mt19937 gen(rd());         
	static std::uniform_int_distribution<> dist(0, e_index.get_enemy_index_num() - 1);

	if (current_size < max_enemy_num)
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
					std::cout << "creat enemy" << std::endl;
					break;
				}
			}
		}
}

void Manager_enemy::delete_enemy(unsigned int i) 
{
	if (enemy[i]->get_health() == 0)
	{
		delete enemy[i];
		enemy[i] = nullptr;
		current_size--;
		move_status[i] = Move_Status::Front;
		enemy_attack_time_elapsed[i] = 0;
		std::cout << "Destroyed: " << i << std::endl;
	}
}

void Manager_enemy::update(GamesEngineeringBase::Window& canvas, Manager_map& map, Manager_hero& hero, Camera& cam, float time)
{
	enemy_create_time_elapsed += time;
	update_enemy_attack_time_elapsed(time);
	create_enemy(map, cam);
	
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


		float newX = enemy[i]->get_hitbox_x() + dx * speed;
		float newY = enemy[i]->get_hitbox_y() + dy * speed;


		for (int j = 0; j < max_enemy_num; ++j)
		{
			if (i == j || enemy[j] == nullptr) continue;

			float dx2 = newX - enemy[j]->get_hitbox_x();
			float dy2 = newY - enemy[j]->get_hitbox_y();
			float dist = sqrt(dx2 * dx2 + dy2 * dy2);
			float minDist = static_cast<float>(enemy[i]->get_hitbox() + enemy[j]->get_hitbox());

			if (dist < minDist && dist > 0.1f) {
				float nx = dx2 / dist;
				float ny = dy2 / dist;
				float overlap = (minDist - dist);
				float push = overlap * 0.1f;
				newX += nx * push;
				newY += ny * push;
			}
		}

		float dx3 = hero.get_hitbox_x() - newX;
		float dy3 = hero.get_hitbox_y() - newY;
		float dist_enemy_to_hero = sqrt(dx3 * dx3 + dy3 * dy3);
		float mid_dist = static_cast<float>(hero.get_hitbox() + enemy[i]->get_hitbox());

		if (dist_enemy_to_hero < mid_dist && dist_enemy_to_hero > 0.1f && hero.get_invincible_time_elapsed() > 1.f) {
			//float nx = dx3 / dist_enemy_to_hero;
			//float ny = dy3 / dist_enemy_to_hero;
			//float overlap = (mid_dist - dist_enemy_to_hero);
			//float push = overlap * 0.1f;
			//newX += nx * push;
			//newY += ny * push;
			//Here is quite hard to understand,enemy[i]->get_name() to get the enemy's name
			hero.suffer_attack(e_index[enemy[i]->get_type()].attack);
			hero.zero_invincible_time_elapsed();
		}

		newX = max(0, min(newX, static_cast<float>(map.get_map_width_pix() - enemy[i]->get_width())));
		newY = max(0, min(newY, static_cast<float>(map.get_map_height_pix() - enemy[i]->get_height())));

		//std::cout << "DEBUG: Enemy " << i
		//	<< " moving from (" << enemy[i]->get_x() << "," << enemy[i]->get_y() << ")"
		//	<< " to (" << newX << "," << newY << ")"
		//	<< " direction: (" << dx << "," << dy << ")"
		//	<< " distance: " << len << std::endl;

		enemy[i]->update(canvas, newX - static_cast<float>(enemy[i]->get_width() / 2),
			newY - static_cast<float>(enemy[i]->get_height() / 2), move_status[i]);
	}
};

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

Manager_bullet::Manager_bullet(GamesEngineeringBase::Window& canvas)
{
	for (unsigned int i = 0; i < max_bullet_num; i++)
	{
		bullet[i] = nullptr;
		move_status[i] = Move_Status::Front;
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
}

void Manager_bullet::create_hero_bullet(Manager_hero& hero)
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
				//std::cout << hero.get_hitbox_x() << hero.get_hitbox_y() << std::endl;
				hero.zero_attack_elapsed();
				//std::cout << "create hero bullet" << std::endl;
				//std::cout << "create bullet pos: " << bullet[i]->get_hitbox_x() << "," << bullet[i]->get_hitbox_y() << std::endl;
				break;
			}
		}
	}
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

void Manager_bullet::move_to_nearest_enemy(unsigned int bullet_index, Position pos, Manager_enemy& enemy, float time)
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
			float dx = enemy.get_hit_box_x(i) - pos.x;
			float dy = enemy.get_hit_box_y(i) - pos.y;
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

		bullet[bullet_index]->update(pos.x + mindx * speed - static_cast<float>(bullet[bullet_index]->get_width() / 2),
			pos.y + mindy * speed - static_cast<float>(bullet[bullet_index]->get_height() / 2));
	}
	else
	{
		enemy.suffer_attack(enemy_index, bullet[bullet_index]->get_attack());
		delete_bullet(bullet_index);
	}

	//return { pos.x + mindx * speed, pos.y + mindy * speed };
}

void Manager_bullet::move_to_nearest_hero(unsigned int bullet_index, Position pos, Manager_hero& hero, float time)
{
	float dx = hero.get_hitbox_x() - pos.x;
	float dy = hero.get_hitbox_y() - pos.y;
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

		bullet[bullet_index]->update(pos.x + dx * speed - static_cast<float>(bullet[bullet_index]->get_width() / 2),
			pos.y + dy * speed - static_cast<float>(bullet[bullet_index]->get_height() / 2));
	}
	else
	{
		hero.suffer_attack(bullet[bullet_index]->get_attack());
		delete_bullet(bullet_index);
	}
}

void Manager_bullet::update(GamesEngineeringBase::Window& canvas, Manager_map& map, Manager_hero& hero,
	Manager_enemy& enemy, Camera& cam, float time)
{
	this->create_hero_bullet(hero);
	this->create_enemy_bullet(enemy);
	
	for (unsigned int i = 0; i < max_bullet_num; i++)
	{
		if (bullet[i] != nullptr)
		{
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
			if (bullet[i]->get_from() == Unit_Type::Hero)
			{
				Position pos = { bullet[i]->get_hitbox_x(), bullet[i]->get_hitbox_y() };
				//std::cout << "bullet pos: " << pos.x << "," << pos.y << std::endl;
				move_to_nearest_enemy(i, pos, enemy, time);
				continue;
			}
			if (bullet[i]->get_from() == Unit_Type::Enenmy)
			{
				bullet[i]->sub_health(time);
				Position pos = { bullet[i]->get_hitbox_x(), bullet[i]->get_hitbox_y() };
				move_to_nearest_hero(i, pos, hero, time);
				continue;
			}
		}
	}
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