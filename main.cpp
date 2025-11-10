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
    NORMAL_INFINITE,
    HARD_INFINITE
};

void load_game_mode(std::string filename, GameMode& mode, float& game_runnig_elapsed)
{
    std::ifstream file(filename, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "[Error] Cannot open load file: " << filename << std::endl;
        return;
    }
    std::string line;
    while (std::getline(file, line))
    {
        std::cout << line << std::endl;
        if (line.find("Mode States") != std::string::npos)
            break;
    }
    unsigned int m;
    file >> m >> game_runnig_elapsed;
    mode = static_cast<GameMode>(m);
}

void draw_health_and_aoe_cd(GamesEngineeringBase::Window& canvas, Manager_hero& hero)
{
    for (unsigned int w = 20; w < hero.get_health() * 6 + 20; w++)
    {
            for (unsigned int h = 20; h < 50; h++)
            {
                //std::cout << "draw health" << std::endl;
                canvas.draw(w, h, 255, 0, 0);
            }
    }

    for (unsigned int w = 320; w < 322; w++)
    {
        for (unsigned int h = 20; h < 50; h++)
        {
            //std::cout << "draw health" << std::endl;
            canvas.draw(w, h, 255, 255, 250);
        }
    }

    for (unsigned int w = 20; w < hero.get_aoe_elapsed() * 10 + 20; w++)
    {
        for (unsigned int h = 60; h < 80; h++)
        {
            //std::cout << "draw health" << std::endl;
            canvas.draw(w, h, 0, 0, 250);
        }
    }
    for (unsigned int w = 70; w < 72; w++)
    {
        for (unsigned int h = 60; h < 80; h++)
        {
            //std::cout << "draw health" << std::endl;
            canvas.draw(w, h, 255, 255, 250);
        }
    }
}

void save_game_mode(std::string filename, GameMode mode, float game_runnig_elapsed)
{
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "[Error] Cannot open save file: " << filename << std::endl;
        return;
    }
    file << "Mode States" << std::endl
        << static_cast<unsigned int>(mode) << " "
        << game_runnig_elapsed << std::endl;
    file.close();
}

void manager_update(GamesEngineeringBase::Window& canvas, float dt, Manager_hero& hero,
    Manager_map& map, Camera& cam, Manager_enemy& ene, Manager_bullet& bul, float& game_runnig_elapsed)
{

    game_runnig_elapsed += dt;
    hero.update(canvas, map, dt);
    cam.update(hero);
    ene.update(canvas, map, hero, cam, dt);
    bul.update(canvas, map, hero, ene, cam, dt);
}

void manager_infinite_update(GamesEngineeringBase::Window& canvas, float dt, Manager_hero& hero,
	Manager_map& map, Camera& cam, Manager_enemy& ene, Manager_bullet& bul, float& game_runnig_elapsed)
{

    game_runnig_elapsed += dt;
	hero.update_infinite(canvas, map, dt);
	cam.update_infinite(hero);
	ene.update_infinite(canvas, map, hero, cam, dt);
	bul.update_infinite(canvas, map, hero, ene, cam, dt);
}

void manager_draw(GamesEngineeringBase::Window& canvas, float dt, Manager_hero& hero,
    Manager_map& map, Camera& cam, Manager_enemy& ene, Manager_bullet& bul)
{
    map.draw(canvas, hero, cam);
    hero.draw(canvas, map, cam);
    ene.draw(canvas, map, cam);
    bul.draw(canvas, map, cam);
    draw_health_and_aoe_cd(canvas, hero);
}

void manager_draw_infinite(GamesEngineeringBase::Window& canvas, float dt, Manager_hero& hero,
    Manager_map& map, Camera& cam, Manager_enemy& ene, Manager_bullet& bul)
{
    map.draw_infinite(canvas, hero, cam);
    hero.draw(canvas, map, cam);
    ene.draw_infinite(canvas, map, cam);
    bul.draw_infinite(canvas, map, cam);
    draw_health_and_aoe_cd(canvas, hero);
}


int main()
{
	//Create window and set the window's size
	GamesEngineeringBase::Window canvas;
    GamesEngineeringBase::Image ui;
	canvas.create(1024, 768, "www");
    ui.load("./Resource/UI.png");

	//Make the loop running status
	bool running = true;

	// status and mode
	GameState state = GameState::MENU;
	GameMode mode = GameMode::NONE;
    bool is_stop = false;
    bool if_continue = false;

	//Create timer to caculate the time
	GamesEngineeringBase::Timer tim;
    float fps_elapsed = 0;
    float button_elapsed = 0;
    float button_threshold = 1.f;
    float game_runnig_elapsed = 0;
    float game_runnig_threshold = 120.f;
    float hero_dead_time_elasped = 0;

	Manager_hero hero(canvas);
	Manager_map map(canvas);
	//map.map_init("tiles");
	Camera cam(canvas,0,0);
	
	Manager_enemy ene(canvas);
	Manager_bullet bul(canvas);

    hero.init();
    //hero = Manager_hero(canvas);
    map.map_init("./Resource/map/normal.txt");
    cam.camera_init(canvas, map.get_map_width_pix(), map.get_map_height_pix());
    //ene = Manager_enemy(canvas);
    ene.set_mode(20);
    //bul = Manager_bullet(canvas);
    //state = GameState::PLAYING;
    //button_elapsed = 0;
    //game_runnig_elapsed = 0;

	//Game loop
	while (state != GameState::EXIT)
	{
		// Check for input (key presses or window events)
		canvas.checkInput();
		// Clear the window for the next frame rendering
		canvas.clear();

		float dt = tim.dt();
        fps_elapsed += dt;
        button_elapsed += dt;
        //std::cout << "button_elapsed : " << button_elapsed << std::endl;
        if (fps_elapsed >= 1 && state == GameState::PLAYING && !is_stop)
        {
            std::cout << "fps:" << 1 / dt << std::endl;
            std::cout << "time: " << game_runnig_elapsed << std::endl;
            fps_elapsed = 0;
        }

		srand(static_cast<int>(time(nullptr)));

		for (unsigned int w = 0; w < canvas.getWidth(); w++)
		{
			for (unsigned int h = 0; h < canvas.getHeight(); h++)
			{
				canvas.draw(w, h, 100, 100, 100);
			}
		}

        //back to the menu or quit the game
        if (canvas.keyPressed('Q') && button_elapsed >= button_threshold)
        {
            if (state == GameState::MENU)
                break;
            if (state == GameState::PLAYING && is_stop)
            {
                state = GameState::MENU;
                std::cout << "your final score is: " << ene.get_score() << std::endl;
                std::cout << "Back to MENU" << std::endl;
                is_stop = false;
                button_elapsed = 0;
            }

        }

        // control the game time
        if (game_runnig_elapsed > game_runnig_threshold && state == GameState::PLAYING)
            if (if_continue == false)
                is_stop = true;

        //if hero dead ,can't continue,game stop, press Q quit the game 
        if (hero.get_health() <= 0 && state == GameState::PLAYING)
        {
            hero_dead_time_elasped += dt;
            if (hero_dead_time_elasped > 10.0f)
                hero_dead_time_elasped = 10.0f;

            if (hero_dead_time_elasped > 0.5f)
            {
                if_continue == false;
                is_stop = true;
            }
        }
            
        switch (state)
        {
            // menu
            case GameState::MENU:
            {
                // menu ui
                for (unsigned int w = 0; w < ui.width; w++)
                {
                    //Make sure the image's pixel in the cam
                    if (w > 0 && w  < canvas.getWidth())
                        for (unsigned int h = 0; h < ui.height; h++)
                        {
                            if (h > 0 && h  < canvas.getHeight())
                                //Make sure image has pixel
                                if (ui.alphaAtUnchecked(w, h) > 0)
                                    //Draw the image pixel with it's x,y
                                    canvas.draw(w, h, ui.atUnchecked(w, h));

                        }
                }

                // choose mode
                if (canvas.keyPressed('1') && button_elapsed >= button_threshold)
                {
                    mode = GameMode::NORMAL;
                    hero.init();
                    //hero = Manager_hero(canvas);
                    map.map_init("./Resource/map/normal.txt");
                    cam.camera_init(canvas, map.get_map_width_pix(), map.get_map_height_pix());
                    ene = Manager_enemy(canvas);
                    ene.set_mode(20);
                    bul = Manager_bullet(canvas);
                    state = GameState::PLAYING;
                    button_elapsed = 0;
                    game_runnig_elapsed = 0;
                    std::cout << "Switched to NORMAL mode" << std::endl;
                }
                else if (canvas.keyPressed('2') && button_elapsed >= button_threshold)
                {
                    mode = GameMode::HARD;
                    hero.init();
                    map.map_init("./Resource/map/hard.txt");
                    cam.camera_init(canvas, map.get_map_width_pix(), map.get_map_height_pix());
                    ene = Manager_enemy(canvas);
                    ene.set_mode(50);
                    bul = Manager_bullet(canvas);
                    state = GameState::PLAYING;
                    button_elapsed = 0;
                    game_runnig_elapsed = 0;
                    std::cout << "Switched to HARD mode" << std::endl;
                }
             
                else if (canvas.keyPressed('3') && button_elapsed >= button_threshold)
                {
                    mode = GameMode::NORMAL_INFINITE;
                    hero.init();
                    map.map_init("./Resource/map/normal.txt");
                    cam.camera_init(canvas, map.get_map_width_pix(), map.get_map_height_pix());
                    ene = Manager_enemy(canvas);
                    ene.set_mode(20);
                    bul = Manager_bullet(canvas);
                    state = GameState::PLAYING;
                    button_elapsed = 0;
                    game_runnig_elapsed = 0;
                    std::cout << "Switched to NORMAL INFINITE mode" << std::endl;
                }

                else if (canvas.keyPressed('4') && button_elapsed >= button_threshold)
                {
                    mode = GameMode::HARD_INFINITE;
                    hero.init();
                    map.map_init("./Resource/map/hard.txt");
                    cam.camera_init(canvas, map.get_map_width_pix(), map.get_map_height_pix());
                    ene = Manager_enemy(canvas);
                    ene.set_mode(50);
                    bul = Manager_bullet(canvas);
                    state = GameState::PLAYING;
                    button_elapsed = 0;
                    game_runnig_elapsed = 0;
                    std::cout << "Switched to HARD INFINITE mode" << std::endl;
                }

                if (canvas.keyPressed('L') && button_elapsed >= button_threshold)
                {
                    
                    load_game_mode("save.txt", mode, game_runnig_elapsed);

                    hero.init();
                    map.map_init("./Resource/map/tiles.txt");
                    cam.camera_init(canvas, map.get_map_width_pix(), map.get_map_height_pix());
                    ene = Manager_enemy(canvas);
                    bul = Manager_bullet(canvas);
                    state = GameState::PLAYING;
                    map.load_map_state("save.txt");
                    hero.load_hero_state("save.txt");
                    ene.load_enemy_state("save.txt");
                    bul.load_bullet_state("save.txt");
                    button_elapsed = 0;
                }

                break;
            }

            //  game running
            case GameState::PLAYING:
            {

                switch (mode)
                {
                case(GameMode::NORMAL):
                {
                    // when game run to the limited time ,press esc to continue
                    if (canvas.keyPressed(VK_ESCAPE) && button_elapsed >= button_threshold)
                    {
                        if (is_stop == true && if_continue == false)
                        {
                            if_continue = true;
                            std::cout << if_continue << std::endl;
                            is_stop = false;
                            std::cout << is_stop << std::endl;
                            button_elapsed = 0;
                            break;
                        }
                        else
                        {
                            is_stop = !is_stop;
                            button_elapsed = 0;
                            break;
                        }
                    }

                    if (canvas.keyPressed('M') && button_elapsed >= button_threshold)
                    {
                        save_game_mode("save.txt", mode, game_runnig_elapsed);

                        map.save_map_state("save.txt");
                        hero.save_hero_state("save.txt");
                        ene.save_enemy_state("save.txt");
                        bul.save_bullet_state("save.txt");
                        button_elapsed = 0;
                    }
                    if (!is_stop)
                        manager_update(canvas, dt, hero, map, cam, ene, bul, game_runnig_elapsed);
                    manager_draw(canvas, dt, hero, map, cam, ene, bul);
                    break;
                }

                case(GameMode::NORMAL_INFINITE):
                {
                    if (canvas.keyPressed(VK_ESCAPE) && button_elapsed >= button_threshold)
                    {
                        if (is_stop == true && if_continue == false)
                        {
                            if_continue = true;
                            std::cout << if_continue << std::endl;
                            is_stop = false;
                            std::cout << is_stop << std::endl;
                            button_elapsed = 0;
                            break;
                        }
                        else
                        {
                            is_stop = !is_stop;
                            button_elapsed = 0;
                            break;
                        }
                    }

                    if (canvas.keyPressed('M') && button_elapsed >= button_threshold)
                    {
                        save_game_mode("save.txt", mode, game_runnig_elapsed);

                        map.save_map_state("save.txt");
                        hero.save_hero_state("save.txt");
                        ene.save_enemy_state("save.txt");
                        bul.save_bullet_state("save.txt");
                        button_elapsed = 0;
                    }

                    if(!is_stop)
                        manager_infinite_update(canvas, dt, hero, map, cam, ene, bul, game_runnig_elapsed);
                    manager_draw_infinite(canvas, dt, hero, map, cam, ene, bul);
                    break;
                }

                case(GameMode::HARD):
                {
                    if (canvas.keyPressed(VK_ESCAPE) && button_elapsed >= button_threshold)
                    {
                        if (is_stop == true && if_continue == false)
                        {
                            if_continue = true;
                            std::cout << if_continue << std::endl;
                            is_stop = false;
                            std::cout << is_stop << std::endl;
                            button_elapsed = 0;
                            break;
                        }
                        else
                        {
                            is_stop = !is_stop;
                            button_elapsed = 0;
                            break;
                        }
                    }

                    if (canvas.keyPressed('M') && button_elapsed >= button_threshold)
                    {
                        save_game_mode("save.txt", mode, game_runnig_elapsed);

                        map.save_map_state("save.txt");
                        hero.save_hero_state("save.txt");
                        ene.save_enemy_state("save.txt");
                        bul.save_bullet_state("save.txt");
                        button_elapsed = 0;
                    }
                    if (!is_stop)
                        manager_update(canvas, dt, hero, map, cam, ene, bul, game_runnig_elapsed);
                    manager_draw(canvas, dt, hero, map, cam, ene, bul);
                    break;
                }

                case(GameMode::HARD_INFINITE):
                {
                    if (canvas.keyPressed(VK_ESCAPE) && button_elapsed >= button_threshold)
                    {
                        if (is_stop == true && if_continue == false)
                        {
                            if_continue = true;
                            std::cout << if_continue << std::endl;
                            is_stop = false;
                            std::cout << is_stop << std::endl;
                            button_elapsed = 0;
                            break;
                        }
                        else
                        {
                            is_stop = !is_stop;
                            button_elapsed = 0;
                            break;
                        }
                    }

                    if (canvas.keyPressed('M') && button_elapsed >= button_threshold)
                    {
                        save_game_mode("save.txt", mode, game_runnig_elapsed);

                        map.save_map_state("save.txt");
                        hero.save_hero_state("save.txt");
                        ene.save_enemy_state("save.txt");
                        bul.save_bullet_state("save.txt");
                        button_elapsed = 0;
                    }

                    if (!is_stop)
                        manager_infinite_update(canvas, dt, hero, map, cam, ene, bul, game_runnig_elapsed);
                    manager_draw_infinite(canvas, dt, hero, map, cam, ene, bul);
                    break;
                }
                    
                default:
                    break;
                }
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