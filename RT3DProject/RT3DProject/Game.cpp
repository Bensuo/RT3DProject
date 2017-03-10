#include "Game.h"

void Game::init()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);

	camera.Position = glm::vec3(0, 15.5f, 100);

	skybox = new Rendering::Skybox("res/textures/front.bmp",
		"res/textures/back.bmp",
		"res/textures/top.bmp",
		"res/textures/bottom.bmp",
		"res/textures/left.bmp",
		"res/textures/right.bmp",
		"res/shaders/skyboxVertex.vs",
		"res/shaders/skyboxFragment.fs");

	testBox = Rendering::Box(glm::vec3(1000, 1, 1000), glm::vec3(0, -23, 0));
	testBox.loadContent(content);

	testPlayer = new Player();
	testPlayer->loadContent(content);

	timer.Initialize();	//always init last for accurate game loop startup
}

void Game::draw()
{
	renderer.begin(camera);
	renderer.drawSkybox(skybox);
	renderer.setShader("Phong");
	renderList.emplace_back(&testPlayer->getPlayerModel());
	renderList.emplace_back(&testPlayer->getWeapon());
	renderList.emplace_back(&testBox);
	renderer.render(renderList);
	renderer.renderFirstPerson(&testPlayer->getVPWeapon());
	renderer.end();
	renderList.clear();
	auto test = 0;
}

bool Game::Quit() const
{
	return input.Quit();
}

void Game::update()
{
	running = !Quit();
	input.Update(camera);
	testBox.update();
	testPlayer->update(timer.GetDeltaTime());
	camera.Update(timer.GetDeltaTime());
}

Game::Game()
{
	init();
	while (running)
	{
		timer.Reset();

		//process individual frame's worth of updates
		while (timer.ProcessFrame())
		{
			timer.Update();
			update();
		}

		//render processed frame
		if (timer.FrameComplete())
		{
			draw();
			timer.IncrementFrames();
		}
		else
		{
			timer.Sleep();
		}
	}
	renderer.quit();
}