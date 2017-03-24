#include "Game.h"
#include <iostream>
#include "Collisions.h"
void Game::init()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);

	camera.Position = glm::vec3(0, 100.0f, 100);

	skybox = new Rendering::Skybox("res/textures/front.bmp",
		"res/textures/back.bmp",
		"res/textures/top.bmp",
		"res/textures/bottom.bmp",
		"res/textures/left.bmp",
		"res/textures/right.bmp",
		"res/shaders/skyboxVertex.vs",
		"res/shaders/skyboxFragment.fs");

	testBox = Rendering::Box(glm::vec3(1000, 1, 1000), glm::vec3(0, -23, 0));
	floor = AABB{ glm::vec3(0, -23, 0), glm::vec3(1000, 1, 1000) };
	testBox.loadContent(content);

	testPlayer = new Player();
	testPlayer->loadContent(content, "rampage");
	testPlayer->setState(Player::STAND);
	testPlayer2 = new Player();
	testPlayer2->loadContent(content, "rampage");
	testPlayer2->setState(Player::STAND);
	timer.Initialize();	//always init last for accurate game loop startup
	terrain.setScale(glm::vec3(40.0f, 5.0f, 40.0f));
	terrain.loadContent("hm.bmp", content);
	
}


void Game::draw()
{
	renderer.begin(camera);
	renderer.drawSkybox(skybox);
	renderer.drawTerrain(&terrain);
	renderer.setShader("Phong");
	renderList.emplace_back(&testPlayer->getPlayerModel());
	//renderList.emplace_back(&testPlayer->getWeapon());
	//renderList.emplace_back(&testBox);
	//renderList.emplace_back(&testBox1);
	//renderList.emplace_back(&testBox2);
	renderer.render(renderList);
	renderer.renderFirstPerson(&testPlayer2->getVPWeapon());
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
	//Simulate some gravity!
	//camera.Position.y -= 50.0f * timer.GetDeltaTime();
	testBox.update();
	testPlayer2->setPosition(camera.Position);
	testPlayer->update(timer.GetDeltaTime());
	testPlayer2->update(timer.GetDeltaTime());
	
	camera.Update(timer.GetDeltaTime());
	
	
	Collisions::CollisionInfo info = Collisions::TestAABBAABB(testPlayer->getAABB(), testPlayer2->getAABB());
	if (info.collision)
	{
		camera.Position -= info.mtv;
	}
	info = Collisions::TestAABBAABB(testPlayer2->getAABB(), floor);
	if (info.collision)
	{
		//camera.Position += info.mtv;
	}
	//testBox1 = Rendering::Box(testPlayer->getAABB().r, testPlayer->getAABB().c);
	//testBox1.setMaterial(material);
	//testBox2 = Rendering::Box(testPlayer2->getAABB().r, testPlayer2->getAABB().c);
	//testBox2.setMaterial(material);
	//testBox1.loadContent(content);
	//testBox2.loadContent(content);
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