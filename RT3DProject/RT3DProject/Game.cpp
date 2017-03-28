#include "Game.h"
#include <iostream>
#include "Collisions.h"


void Game::init()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	scene = new Scene();
	scene->loadContent(content);
	auto& npcs = scene->getNPCs();
	for (size_t i = 0; i < npcs.size(); i++)
	{
		npcControllers.push_back(AIController());
		npcControllers[i].setActor(npcs[i]);
		npcControllers[i].setTarget(scene->getPlayer());
	}
	
	healthLabel = new Rendering::UI("HEALTH: 100", true);
	ammoLabel =	  new Rendering::UI("AMMO:   100", true);
	crosshair = new Rendering::UI("res/textures/Crosshair.png", false);
	AudioManager::Init();
	AudioManager::PlayMusic("02 - Rip & Tear.mp3", 0.5f);
	timer.Initialize();	//always init last for accurate game loop startup
}


void Game::DrawMinimap(std::vector<std::shared_ptr<Player>>& npcs)
{
	auto zoom = 300.0f;
	auto mapWidth = 182;
	auto mapHeight = 182;

	glm::mat4 view = lookAt(glm::vec3(0, zoom, 0),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(-1.0, 0.0, 0.0));

	view = rotate(view, camera.getYaw(), glm::vec3(0, 1, 0));
	view = translate(view, glm::vec3(-scene->getPlayer()->getPosition().x, 0, -scene->getPlayer()->getPosition().z));

	renderer.setView(view);
	renderer.setProjection(glm::ortho(-zoom / 2, zoom / 2, -zoom / 2, zoom / 2, -2000.0f, 2000.0f));
	//renderer.setProjection(glm::perspective(1.0f, static_cast<float>(mapWidth) / mapHeight, 0.1f, 2000.0f));

	glViewport(1280 - mapWidth, 720 - mapHeight, mapWidth, mapHeight);
	glDisable(GL_DEPTH_TEST);
	renderList.push_back(&scene->getPlayer()->getPlayerModel());
	renderList.push_back(&scene->getPlayer()->getWeapon());
	for (auto i = 0; i < npcs.size(); i++)
	{
		renderList.push_back(&npcs[i]->getPlayerModel());
		renderList.push_back(&npcs[i]->getWeapon());
	}
	renderer.drawTerrain(scene->getTerrain());
	renderer.setShader("Phong");
	renderer.render(renderList);
	renderList.clear();
	glEnable(GL_DEPTH_TEST);
}

void Game::draw()
{
	//Populate render list
	auto& npcs = scene->getNPCs();
	if (!camera.isFPS()) {
		renderList.push_back(&scene->getPlayer()->getPlayerModel());
		renderList.push_back(&scene->getPlayer()->getWeapon());
	}
	for (auto i = 0; i < npcs.size(); i++)
	{
		renderList.push_back(&npcs[i]->getPlayerModel());
		renderList.push_back(&npcs[i]->getWeapon());
	}
	auto& staticObjects = scene->getStaticObjects();
	for (auto i = 0; i < staticObjects.size(); i++)
	{
		renderList.push_back(staticObjects[i]->getModel());
	}
	
	renderer.begin();
	renderer.setView(camera.GetViewMatrix());
	renderer.setProjection(glm::perspective(1.0f, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 2000.0f));

	glViewport(0, 0, 1280, 720);
	renderer.drawSkybox(scene->getSkybox());
	renderer.drawTerrain(scene->getTerrain());
	renderer.setShader("Phong");

	renderer.render(renderList);

	if (camera.isFPS()) {
		renderer.renderFirstPerson(&scene->getPlayer()->getVPWeapon());
	}
	renderer.renderUI(healthLabel, glm::vec3(-0.866f, -0.8f, 0.0f), glm::vec3(0.10f, 0.025f, 0.0f));//position and size of text
	renderer.renderUI(ammoLabel, glm::vec3(-0.866f, -0.9f, 0.0f), glm::vec3(0.10f, 0.025f, 0.0f));//position and size of text

	if (scene->getPlayer()->Aiming())
	renderer.renderUI(crosshair, glm::vec3(0), glm::vec3(0.05f));//position and size of crosshair

	renderList.clear();

	DrawMinimap(npcs);

	renderer.swapBuffers();
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
	//Update AI controllers
	for (int i = 0; i < npcControllers.size(); i++)
	{
		npcControllers[i].update();
	}
	auto& npcs = scene->getNPCs();
	for (size_t i = 0; i < npcs.size(); i++)
	{
		npcs[i]->update(timer.GetDeltaTime());
		npcs[i]->ClampPosition(glm::vec3(-scene->getTerrain()->getScale().x / 2 - 1, 0, -scene->getTerrain()->getScale().z / 2 - 1), glm::vec3(scene->getTerrain()->getScale().x / 2 - 1, scene->getTerrain()->getScale().y + 50, scene->getTerrain()->getScale().z / 2 - 1));
	}
	
	scene->getPlayer()->update(timer.GetDeltaTime());
	scene->getPlayer()->ClampPosition(glm::vec3(-scene->getTerrain()->getScale().x/2-1, 0, -scene->getTerrain()->getScale().z / 2-1), glm::vec3(scene->getTerrain()->getScale().x / 2-1, scene->getTerrain()->getScale().y + 250, scene->getTerrain()->getScale().z / 2-1));
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
		auto pos = p2->getPosition();
		p2->setPosition(pos -= info.mtv);
	}
}
void Game::checkCollisions()
{
	auto& npcs = scene->getNPCs();
	auto& staticObjects = scene->getStaticObjects();
	Collisions::terrainCollision(scene->getPlayer(), scene->getTerrain());
	for (int i = 0; i < staticObjects.size(); i++)
	{
		auto info = Collisions::TestAABBAABB(staticObjects[i]->getAABB(), scene->getPlayer()->getAABB());
		if (info.collision)
		{
			auto pos = scene->getPlayer()->getPosition();
			scene->getPlayer()->setPosition(pos -= info.mtv);
		}
	}
	for (size_t i = 0; i < npcs.size(); i++)
	{
		playerCollision(scene->getPlayer(), npcs[i].get());
		for (int j = i+1; j < npcs.size(); j++)
		{
			playerCollision(npcs[j].get(), npcs[i].get());
		}
		for (int k = 0; k < staticObjects.size(); k++)
		{
			auto info = Collisions::TestAABBAABB(staticObjects[k]->getAABB(), npcs[i]->getAABB());
			if (info.collision)
			{
				auto pos = npcs[i]->getPosition();
				npcs[i]->setPosition(pos -= info.mtv);
			}
		}
		npcs[i]->ClampPosition(glm::vec3(-scene->getTerrain()->getScale().x / 2 - 1, 0, -scene->getTerrain()->getScale().z / 2 - 1), glm::vec3(scene->getTerrain()->getScale().x / 2 - 1, 250, scene->getTerrain()->getScale().z / 2 - 1));
		Collisions::terrainCollision(npcs[i].get(), scene->getTerrain());
	}
}
