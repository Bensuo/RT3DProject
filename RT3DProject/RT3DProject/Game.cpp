#include "Game.h"
#include <iostream>
#include "Collisions.h"



void Game::init()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	scene = new Scene();
	scene->loadContent(content);
	audioManager.PlayMusic("fortress sneaking.mp3");
	audioManager.PlaySound("impact.wav", 0.25f);

	/*skybox = new Rendering::Skybox("res/textures/front.bmp",
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
	terrain.setScale(glm::vec3(2000.0f, 100.0f, 2000.0f));
	terrain.loadContent("heightmap.bmp", "heightmap-norm.bmp", content);
	testPlayer2->setPosition(glm::vec3(100, 15, -200));*/
	timer.Initialize();	//always init last for accurate game loop startup
}


void Game::draw()
{
	//Populate render list
	std::vector<Player*>& npcs = scene->getNPCs();
	if (!camera.isFPS()) {
		renderList.push_back(&scene->getPlayer()->getPlayerModel());
		renderList.push_back(&scene->getPlayer()->getWeapon());
	}
	for (int i = 0; i < npcs.size(); i++)
	{
		renderList.push_back(&npcs[i]->getPlayerModel());
		renderList.push_back(&npcs[i]->getWeapon());
	}
	
	renderer.begin(camera);
	renderer.drawSkybox(scene->getSkybox());
	renderer.drawTerrain(scene->getTerrain());
	renderer.setShader("Phong");

	renderer.render(renderList);

	if (camera.isFPS()) {
		renderer.renderFirstPerson(&scene->getPlayer()->getVPWeapon());
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
	scene->getPlayer()->UpdateVectors(camera.GetFront());
	scene->getPlayer()->setFPS(camera.isFPS());

	input.Update(scene->getPlayer(), camera);
	
	std::vector<Player*>&npcs = scene->getNPCs();
	for (size_t i = 0; i < npcs.size(); i++)
	{
		npcs[i]->update(timer.GetDeltaTime());
	}
	
	scene->getPlayer()->update(timer.GetDeltaTime());
	scene->getPlayer()->ClampPosition(glm::vec3(-scene->getTerrain()->getScale().x/2-1, 0, -scene->getTerrain()->getScale().z / 2-1), glm::vec3(scene->getTerrain()->getScale().x / 2-1, 250, scene->getTerrain()->getScale().z / 2-1));
	checkCollisions();
	camera.Update(timer.GetDeltaTime(), scene->getPlayer()->getPosition() - glm::vec3(0,-24,0));


	

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

void playerCollision(Player* p1, Player* p2)
{
	auto info = Collisions::TestAABBAABB(p1->getAABB(), p2->getAABB());
	if (info.collision)
	{
		auto pos = p1->getPosition();
		p1->setPosition(pos += info.mtv);
	}
}
void Game::checkCollisions()
{
	std::vector<Player*>&npcs = scene->getNPCs();
	Collisions::terrainCollision(scene->getPlayer(), scene->getTerrain());
	for (size_t i = 0; i < npcs.size(); i++)
	{
		playerCollision(scene->getPlayer(), npcs[i]);
		Collisions::terrainCollision(npcs[i], scene->getTerrain());
	}
}
