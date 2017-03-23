#include "Game.h"
#include <iostream>
#include "Collisions.h"

void Game::init()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);

	audioManager.PlayMusic("fortress sneaking.mp3");
	audioManager.PlaySound("impact.wav", 0.25f);

	skybox = new Rendering::Skybox("res/textures/front.bmp",
		"res/textures/back.bmp",
		"res/textures/top.bmp",
		"res/textures/bottom.bmp",
		"res/textures/left.bmp",
		"res/textures/right.bmp",
		"res/shaders/skyboxVertex.vs",
		"res/shaders/skyboxFragment.fs");

	testBox = Rendering::Box(glm::vec3(1000, 1, 1000), glm::vec3(0, -23, 0));
	floor = AABB{ glm::vec3(0, -23, 0), glm::vec3(500, 0.5, 500) };
	testBox.loadContent(content);

	testPlayer = new Player();
	testPlayer->loadContent(content, "rampage");
	testPlayer->setState(Player::STAND);
	testPlayer->setPosition(glm::vec3(0, 15, -150));
	testPlayer2 = new Player();
	testPlayer2->loadContent(content, "rampage");
	testPlayer2->setState(Player::STAND);
	testPlayer2->setPosition(glm::vec3(0, 15, -200));
	timer.Initialize();	//always init last for accurate game loop startup
}


void Game::draw()
{
	renderer.begin(camera);
	renderer.drawSkybox(skybox);
	renderer.setShader("Phong");
	renderList.emplace_back(&testPlayer->getPlayerModel());
	renderList.emplace_back(&testPlayer->getWeapon());

	if (!camera.isFPS()) {
		renderList.emplace_back(&testPlayer2->getPlayerModel());
		renderList.emplace_back(&testPlayer2->getWeapon());
	}

	renderList.emplace_back(&testBox);
	renderList.emplace_back(&testBox1);
	renderList.emplace_back(&testBox2);
	renderer.render(renderList);

	if (camera.isFPS()) {
		renderer.renderFirstPerson(&testPlayer2->getVPWeapon());
	}

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

	testPlayer2->UpdateVectors(camera.GetFront());
	testPlayer2->setFPS(camera.isFPS());

	input.Update(testPlayer2, camera);
	testBox.update();

	testPlayer->update(timer.GetDeltaTime());
	testPlayer2->update(timer.GetDeltaTime());
	testPlayer2->ClampPosition(glm::vec3(-500, -23, -500), glm::vec3(500, 100, 500));

	camera.Update(timer.GetDeltaTime(), testPlayer2->getPosition() - glm::vec3(0,-24,0));

	auto info = Collisions::TestAABBAABB(testPlayer->getAABB(), testPlayer2->getAABB());
	if (info.collision)
	{
		auto pos = testPlayer2->getPosition();
		testPlayer2->setPosition(pos -= info.mtv);
	}

	info = Collisions::TestAABBAABB(testPlayer2->getAABB(), floor);
	if (info.collision)
	{
		auto pos = testPlayer2->getPosition();
		testPlayer2->setPosition(pos += info.mtv);
	}

	info = Collisions::TestAABBAABB(testPlayer->getAABB(), floor);
	if (info.collision)
	{
		auto pos = testPlayer->getPosition();
		testPlayer->setPosition(pos += info.mtv);
	}

	testBox1 = Rendering::Box(testPlayer->getAABB().r * 2.0f, testPlayer->getAABB().c);
	testBox1.setMaterial(material);
	testBox1.loadContent(content);

	testBox2 = Rendering::Box(testPlayer2->getAABB().r * 2.0f, testPlayer2->getAABB().c);
	testBox2.setMaterial(material);
	testBox2.loadContent(content);
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
			timer.IncrementFrames();
			draw();
		}
		else
		{
			timer.Sleep();
		}
	}
	renderer.quit();
}