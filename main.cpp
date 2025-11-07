#pragma once
#include <iostream>
#include "GamesEngineeringBase.h"
#include "manager.h"
#include <time.h>

enum class GameState {
	MENU,
	PLAYING,
	EXIT
};

enum class GameMode {
	NONE,
	NORMAL,
	HARD,
	SURVIVAL
};

void manager(GamesEngineeringBase::Window& canvas, float dt, Manager_hero& hero,
	Manager_map& map, Camera& cam, Manager_enemy& ene, Manager_bullet& bul)
{


	hero.update(canvas, map, dt);
	cam.update(hero);
	ene.update(canvas, map, hero, cam, dt);
	bul.update(canvas, map, hero, ene, cam, dt);
}

int main()
{
	//Create window and set the window's size
	GamesEngineeringBase::Window canvas;
	canvas.create(1024, 768, "www");
	
	//Make the loop running status
	bool running = true;

	// status and mode
	GameState state = GameState::MENU;
	GameMode mode = GameMode::NONE;

	//Create timer to caculate the time
	GamesEngineeringBase::Timer tim;

	Manager_hero hero(canvas);
	Manager_map map(canvas);
	//map.map_init("tiles");
	Camera cam(canvas,0,0);
	
	Manager_enemy ene(canvas);
	Manager_bullet bul(canvas);
	//Game loop
	while (state != GameState::EXIT)
	{
		// Check for input (key presses or window events)
		canvas.checkInput();
		// Clear the window for the next frame rendering
		canvas.clear();

		float dt = tim.dt();
		srand(static_cast<int>(time(nullptr)));

		for (unsigned int w = 0; w < canvas.getWidth(); w++)
		{
			for (unsigned int h = 0; h < canvas.getHeight(); h++)
			{
				canvas.draw(w, h, 100, 100, 100);
			}
		}

		if (canvas.keyPressed('Q')) break;

        switch (state)
        {
            // menu
        case GameState::MENU:
        {
            // 绘制菜单
            //canvas.drawText(350, 200, "=== SELECT GAME MODE ===", 255, 255, 255);
            //canvas.drawText(380, 250, "[1] Normal Mode", 200, 200, 200);
            //canvas.drawText(380, 280, "[2] Hard Mode", 200, 200, 200);
            //canvas.drawText(380, 310, "[3] Survival Mode", 200, 200, 200);
            //canvas.drawText(380, 350, "[Q] Quit", 200, 200, 200);

            // choose mode
            if (canvas.keyPressed('1'))
            {
                mode = GameMode::NORMAL;
                hero.init();
                //hero = Manager_hero(canvas);
                map.map_init("tiles");
                cam.camera_init(canvas, map.get_map_width_pix(), map.get_map_height_pix());
                ene = Manager_enemy(canvas);
                bul = Manager_bullet(canvas);
                state = GameState::PLAYING;
                std::cout << "Switched to NORMAL mode" << std::endl;
            }
            //else if (canvas.keyPressed('2'))
            //{
            //    mode = GameMode::HARD;
            //    map.map_init("tiles_hard");
            //    cam.camera_init(canvas, map.get_map_width_pix(), map.get_map_height_pix());
            //    ene = Manager_enemy(canvas);
            //    bul = Manager_bullet(canvas);
            //    state = GameState::PLAYING;
            //    std::cout << "Switched to HARD mode" << std::endl;
            //}
            // 
            //else if (canvas.keyPressed('3'))
            //{
            //    mode = GameMode::SURVIVAL;
            //    map.map_init("tiles_survival");
            //    cam.camera_init(canvas, map.get_map_width_pix(), map.get_map_height_pix());
            //    ene = Manager_enemy(canvas);
            //    bul = Manager_bullet(canvas);
            //    state = GameState::PLAYING;
            //    std::cout << "Switched to SURVIVAL mode" << std::endl;
            //}

            break;
        }

        //  game running
        case GameState::PLAYING:
        {
            if (canvas.keyPressed(VK_ESCAPE))
            {
                state = GameState::MENU;
                std::cout << "Back to MENU" << std::endl;
                break;
            }

            if (canvas.keyPressed('M'))
            {
                hero.save_hero_state("save.txt");
                ene.save_enemy_state("save.txt");
                bul.save_bullet_state("save.txt");
            }

            if (canvas.keyPressed('L'))
            {
                hero.load_hero_state("save.txt");
            }

            manager(canvas, dt, hero, map, cam, ene, bul);
            map.draw(canvas, hero, cam);
            hero.draw(canvas, map, cam);
            ene.draw(canvas, map, cam);
            bul.draw(canvas, map, cam);
            break;
        }

        default:
            break;
        }
		// Display the frame on the screen. This must be called once the frame 
		//is finished in order to display the frame.
		canvas.present();
	}
	return 0;
}