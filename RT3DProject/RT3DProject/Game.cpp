#include "Game.h"



void Game::init()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);

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
	renderList.emplace_back(&testPlayer->getPlayerModel());
	renderList.emplace_back(&testPlayer->getWeapon());
	renderList.emplace_back(&testBox);
	renderer.render(renderList);
	renderer.renderFirstPerson(&testPlayer->getVPWeapon());
	renderer.end();
	renderList.clear();
}

bool Game::Quit() const
{
	return m_input.Quit();
}

void Game::update()
{
	m_input.Update(m_camera);
	testBox.update();
	testPlayer->update(0.1f);
	testPlayer2->update(0.1f);
	m_camera.Update(1 / 60.0f);
}

Game::Game()
{
	init();
	auto running = true;

	while (running)
	{
		running = !Quit();
		update();
		draw();
	}
	renderer.quit();
}