#include "Game.h"



void Game::init()
{
	m_camera.Position = glm::vec3(0, 0, 200);

	m_skybox = new Rendering::Skybox("res/textures/front.bmp",
		"res/textures/back.bmp",
		"res/textures/top.bmp",
		"res/textures/bottom.bmp",
		"res/textures/left.bmp",
		"res/textures/right.bmp",
		"res/shaders/skyboxVertex.vs",
		"res/shaders/skyboxFragment.fs");

	playerCharacter.loadContent(content, "res/md2/rampage");
	playerCharacter.setAnimation(1);
	playerWeapon.loadContent(content, "res/md2/weapon");
	playerWeapon.setAnimation(1);
	viewportWeapon.loadContent(content, "res/md2/v_machn");
	viewportWeapon.setAnimation(2);

	testBox = Rendering::Box(glm::vec3(100, 1, 100), glm::vec3(0, -23, 0));
	testBox.loadContent(content);

	testPlayer = new Player();
	testPlayer->loadContent(content);
	testPlayer2 = new Player();
	testPlayer2->loadContent(content);
	testPlayer2->setPosition(glm::vec3(100, 0, 0));
	renderList.emplace_back(&testPlayer->getPlayerModel());
	renderList.emplace_back(&testPlayer->getWeapon());
	
	renderList.emplace_back(&testBox);
}

void Game::draw()
{
	renderer.begin(m_camera);
	renderer.drawSkybox(m_skybox, m_camera);
	renderer.setShader("Phong");
	renderer.renderFirstPerson(&testPlayer->getVPWeapon(), m_camera);
	renderer.render(renderList, m_camera);
	renderer.end();

}

void Game::update()
{
	playerWeapon.update(0.1f);
	playerCharacter.update(0.1f);
	viewportWeapon.update(0.1f);
	testBox.update();
	testPlayer->update(0.1f);
	testPlayer2->update(0.1f);
	m_camera.Update(1 / 60.0f);
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