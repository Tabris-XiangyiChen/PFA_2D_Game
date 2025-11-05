#include "charactor.h"
#include "map.h"

bool Charactor::load_image(std::string Heroname)
{
	for (unsigned int i = 0; i < charactor_move_status_num; i++)
	{
		std::string filename = "./Resource/" + Heroname + "_" + std::to_string(i) + ".png";
		if (!image[i].load(filename))
			return false;
	}

	hitbox = static_cast<unsigned int>(image[0].width / 3);
	hitbox_center_x = static_cast<float>(locate_x + static_cast<float>(image[0].width / 2));
	hitbox_center_y = static_cast<float > (locate_y + static_cast<float>(image[0].height / 2));
	return true;
}

void Charactor::draw(GamesEngineeringBase::Window& canvas, unsigned int x, unsigned int y)
{
	for (unsigned int w = 0; w < image[static_cast<unsigned int>(this->m_status)].width; w++)
	{
		//Make sure the image's pixel in the canvas
		if (w + x > 0 && w + x < canvas.getWidth())
			for (unsigned int h = 0; h < image[static_cast<unsigned int>(this->m_status)].height; h++)
			{
				if (h + y > 0 && h + y < canvas.getHeight())
					//Make sure image has pixel
					if (image[static_cast<unsigned int>(this->m_status)].alphaAtUnchecked(w, h) > 0)
						//Draw the image pixel with it's x,y
						canvas.draw(x + w, y + h, image[static_cast<unsigned int>(this->m_status)].atUnchecked(w, h));

			}
	}
}

void Charactor::draw_incenter(GamesEngineeringBase::Window& canvas)
{
	for (unsigned int w = 0; w < image[static_cast<unsigned int>(this->m_status)].width; w++)
	{
		//Make sure the image's pixel in the canvas
		//if (w + locate_x > 0 && w + locate_x < map.get_map_width_pix())
			for (unsigned int h = 0; h < image[static_cast<unsigned int>(this->m_status)].height; h++)
			{
				//if (h + locate_x > 0 && h + locate_y < map.get_map_height_pix())
					//Make sure image has pixel
					if (image[static_cast<unsigned int>(this->m_status)].alphaAtUnchecked(w, h) > 0)
						//Draw the image pixel with it's x,y
						canvas.draw(canvas.getWidth() / 2 - image[0].width / 2 + w, 
							canvas.getHeight() / 2 - image[0].height / 2 + h,
							image[static_cast<unsigned int>(this->m_status)].atUnchecked(w, h));

			}
	}
}

void Charactor::update(GamesEngineeringBase::Window& canvas, float x, float y, Move_Status status)
{
		locate_x = x;
		locate_y = y;
		m_status = status;
		hitbox_center_x = static_cast<float>(x + image[0].width / 2);
		hitbox_center_y = static_cast<float>(y + image[0].height / 2);
	//std::cout << "(" << locate_x << "," << locate_y << ")" << std::endl;
}

void Charactor::update(Move_Status status)
{
	m_status = status;
}

bool Enemy::load_image()
{
	for (unsigned int i = 0; i < enemy_move_status_num; i++)
	{
		std::string filename = "./Resource/" + this->enemy_name + "_" + std::to_string(i) + ".png";
		if (!image[i].load(filename))
			return false;
	}

	hitbox = static_cast<unsigned int>(image[0].width / 3);
	hitbox_center_x = locate_x + static_cast<float>(image[0].width / 2);
	hitbox_center_y = locate_y + static_cast<float>(image[0].height / 2);
	return true;
}

void Enemy::draw(GamesEngineeringBase::Window& canvas, int x, int y)
{
	for (unsigned int w = 0; w < image[static_cast<unsigned int>(this->m_status)].width; w++)
	{
		//Make sure the image's pixel in the canvas
		if (w + x > 0 && w + x < canvas.getWidth())
			for (unsigned int h = 0; h < image[static_cast<unsigned int>(this->m_status)].height; h++)
			{
				if (h + y > 0 && h + y < canvas.getHeight())
					//Make sure image has pixel
					if (image[static_cast<unsigned int>(this->m_status)].alphaAtUnchecked(w, h) > 0)
						//Draw the image pixel with it's x,y
						canvas.draw(x + w, y + h, image[static_cast<unsigned int>(this->m_status)].atUnchecked(w, h));

			}
	}
}

void Enemy::update(GamesEngineeringBase::Window& canvas, float x, float y, Move_Status status)
{
	locate_x = x;
	locate_y = y;
	hitbox_center_x = static_cast<float>(x + static_cast<float>(image[0].width / 2));
	hitbox_center_y = static_cast<float>(y + static_cast<float>(image[0].height / 2));
	m_status = status;
}

void Enemy::update(float x, float y)
{
	locate_x = x;
	locate_y = y;
	//hitbox_center_x = static_cast<unsigned int>(x + image[0].width / 2);
	//hitbox_center_y = static_cast<unsigned int>(y + image[0].height / 2);
	hitbox_center_x = static_cast<float>(x + static_cast<float>(image[0].width / 2));
	hitbox_center_y = static_cast<float>(y + static_cast<float>(image[0].height / 2));
}


bool Bullet::load_image()
{
	for (unsigned int i = 0; i < bullet_move_status_num; i++)
	{
		std::string filename = "./Resource/Bullet_" + this->bullet_name + "_" + std::to_string(i) + ".png";
		if (!image[i].load(filename))
			return false;
	}

	hitbox = static_cast<unsigned int>(image[0].width) / 2;
	hitbox_center_x = locate_x + static_cast<float>(image[0].width / 2);
	hitbox_center_y = locate_y + static_cast<float>(image[0].height / 2);
	return true;
}

void Bullet::draw(GamesEngineeringBase::Window& canvas, int x, int y)
{
	for (unsigned int w = 0; w < image[static_cast<unsigned int>(this->m_status)].width; w++)
	{
		//Make sure the image's pixel in the canvas
		if (w + x > 0 && w + x < canvas.getWidth())
			for (unsigned int h = 0; h < image[static_cast<unsigned int>(this->m_status)].height; h++)
			{
				if (h + y > 0 && h + y < canvas.getHeight())
					//Make sure image has pixel
					if (image[static_cast<unsigned int>(this->m_status)].alphaAtUnchecked(w, h) > 0)
						//Draw the image pixel with it's x,y
						canvas.draw(x + w, y + h, image[static_cast<unsigned int>(this->m_status)].atUnchecked(w, h));

			}
	}
}

void Bullet::update(GamesEngineeringBase::Window& canvas, float x, float y)
{
	locate_x = x;
	locate_y = y;
	hitbox_center_x = static_cast<float>(x + static_cast<float>(image[0].width / 2));
	hitbox_center_y = static_cast<float>(y + static_cast<float>(image[0].height / 2));
	//m_status = status;
}

void Bullet::update(float x, float y)
{
	locate_x = x;
	locate_y = y;
	hitbox_center_x = static_cast<float>(x + static_cast<float>(image[0].width / 2));
	hitbox_center_y = static_cast<float>(y + static_cast<float>(image[0].height / 2));
}
