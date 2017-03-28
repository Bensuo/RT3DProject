#include "Game.h"
#include <iostream>
#include "Collisions.h"


void Game::init()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	audioManager.PlayMusic("02 - Rip & Tear.mp3");
	//audioManager.PlaySound("impact.wav", 0.25f);
	scene = new Scene();
	scene->loadContent(content);
	uiTest = new Rendering::UI("HEALTH:", true);
	uiTest2 = new Rendering::UI("res/textures/Crosshair.png", false);
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


void Game::DrawMinimap(std::vector<Player*>& npcs)
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
	renderer.renderUI(uiTest, glm::vec3(-0.8f, 0.8f, 0.0f), glm::vec3(0.10f, 0.045f, 0.0f));//position and size of text
	
	if (scene->getPlayer()->Aiming())
	renderer.renderUI(uiTest2, glm::vec3(0), glm::vec3(0.05f));//position and size of crosshair

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
