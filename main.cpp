#pragma once
#include <iostream>
#include "GamesEngineeringBase.h"
#include "manager.h"
#include <time.h>

int main()
{
	//Create window and set the window's size
	GamesEngineeringBase::Window canvas;
	canvas.create(1024, 768, "www");
	
	//Make the loop running status
	bool running = true;

	//Create timer to caculate the time
	GamesEngineeringBase::Timer tim;
	//Charactor un;
	//un.load_image("Wizard");
	//un.load_image("Resource/Wizard_Back_1.png");
	/*un.load_image("./Resource/Wizard_Front_1.png");*/
	Manager_hero hero(canvas);
	Manager_map map(canvas);
	Camera cam(canvas, map.get_map_width_pix(), map.get_map_height_pix());
	
	Manager_enemy ene(canvas);
	//Game loop
	while (running)
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

		if (canvas.keyPressed(VK_ESCAPE)) break;
		
		
		hero.update(canvas, map, dt);
		cam.update(hero);
		ene.update(canvas, map, hero,cam, dt);
		map.draw(canvas, hero, cam);
		hero.draw(canvas, map, cam);
		ene.draw(canvas, map, cam);

		// Display the frame on the screen. This must be called once the frame 
		//is finished in order to display the frame.
		canvas.present();
	}
	return 0;
}