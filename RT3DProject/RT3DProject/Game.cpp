#include "Game.h"



void Game::init()
{
	m_camera.Position = glm::vec3(0, 15.5f, 100);

	m_skybox = new Rendering::Skybox("res/textures/front.bmp",
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
	testPlayer2 = new Player();
	testPlayer2->loadContent(content);
	testPlayer2->setPosition(glm::vec3(100, 0, 0));
	
}

void Game::draw()
{
	renderer.begin(m_camera);
	renderer.drawSkybox(m_skybox);
	renderer.setShader("Phong");
	renderer.renderFirstPerson(&testPlayer->getVPWeapon());
	renderList.emplace_back(&testPlayer->getPlayerModel());
	renderList.emplace_back(&testPlayer->getWeapon());
	renderer.render(renderList);
	renderer.setShader("Phong2");
	renderList.clear();
	renderList.emplace_back(&testBox);
	renderer.render(renderList);
	renderer.end();
	renderList.clear();
}

void Game::update()
{
	testBox.update();
	testPlayer->update(0.1f);
	testPlayer2->update(0.1f);
	m_camera.Update(1 / 60.0f);

	auto currentKeyStates = SDL_GetKeyboardState(nullptr);
	if (currentKeyStates[SDL_SCANCODE_W])
	{
		m_camera.Position.z -= 1.0f;
	}
	else if (currentKeyStates[SDL_SCANCODE_A])
	{
		m_camera.Position.x -= 1.0f;
	}
	else if (currentKeyStates[SDL_SCANCODE_D])
	{
		m_camera.Position.x += 1.0f;
	}
	else if (currentKeyStates[SDL_SCANCODE_S])
	{
		m_camera.Position.z += 1.0f;
	}
}

Game::Game()
{


	init();
	bool running = true; // set running to true
	SDL_Event sdlEvent; // variable to detect SDL events
	while (running)
	{ // the event loop
		while (SDL_PollEvent(&sdlEvent))
		{
			if (sdlEvent.type == SDL_QUIT)
				running = false;
		}
		update();
		draw(); // call the draw function
	}

	renderer.quit();
}