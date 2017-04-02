#include "Game.h"
#include <iostream>
#include "Collisions.h"
#include "Ray.h"

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

	victory = new Rendering::UI("VICTORY!", 48, glm::vec4(255, 215, 0, 255));
	failure = new Rendering::UI("YOU DIED.", 48, glm::vec4(255, 0, 0, 255));

	scoreLabel = new Rendering::UI("SCORE: " + std::to_string(score), 24, glm::vec4(255, 132, 0, 204));
	healthLabel = new Rendering::UI("HEALTH: " + std::to_string(scene->getPlayer()->getHealth()), 24, glm::vec4(255, 132, 0, 204));
	ammoLabel =	new Rendering::UI("AMMO: " + std::to_string(scene->getPlayer()->getAmmo()), 24, glm::vec4(255, 132, 0, 204));
	timeLabel = new Rendering::UI("02:00", 24, glm::vec4(255, 132, 0, 204));
	crosshair = new Rendering::UI("res/textures/Crosshair.png");
	HUD = new Rendering::UI("res/textures/Interface.png");

	AudioManager::Init(content);
	AudioManager::PlayMusic("02 - Rip & Tear.mp3", 0.5f);

	gameTime.Initialize();	//always init last for accurate game loop startup
	countdown.startTimer();
}

void Game::DrawMinimap()
{
	if (!scene->getPlayer()->getIsDead() && !countdown.finished())
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
		renderer.setProjection(glm::ortho(-zoom / 2, zoom / 2, -zoom / 2, zoom / 2, 0.1f, 5000.0f));

		glViewport(1280 - mapWidth, 720 - mapHeight, mapWidth, mapHeight);

		glDisable(GL_DEPTH_TEST);
		renderList.push_back(&scene->getPlayer()->getPlayerModel());
		renderList.push_back(&scene->getPlayer()->getWeapon());

		auto& npcs = scene->getNPCs();
		for (auto i = 0; i < npcs.size(); i++)
		{
			renderList.push_back(&npcs[i]->getPlayerModel());
			renderList.push_back(&npcs[i]->getWeapon());
		}

		auto pickups = scene->getPickups();
		for (auto i = 0; i < pickups.size(); i++)
		{
			renderList.push_back(&pickups[i]->getPlayerModel());
		}

		renderer.drawTerrain(scene->getTerrain());
		renderer.setShader("Phong-interpolated");

		renderer.render(renderList);
		renderList.clear();

		glEnable(GL_DEPTH_TEST);
	}
}

void Game::DrawScene()
{
	//Populate render list
	auto& npcs = scene->getNPCs();
	if (!camera.isFPS()) {
		renderList.push_back(&scene->getPlayer()->getPlayerModel());
		if (scene->getPlayer()->playerState != Player::PlayerState::DEATH1)
		{
			renderList.push_back(&scene->getPlayer()->getWeapon());
		}
	}

	for (auto i = 0; i < npcs.size(); i++)
	{
		renderList.push_back(&npcs[i]->getPlayerModel());
		if (npcs[i]->playerState != Player::PlayerState::DEATH1)
		{
			renderList.push_back(&npcs[i]->getWeapon());
		}
		
	}

	auto& staticObjects = scene->getStaticObjects();
	for (auto i = 0; i < staticObjects.size(); i++)
	{
		renderList.push_back(staticObjects[i]->getModel());
	}

	auto pickups = scene->getPickups();
	for (auto i = 0; i < pickups.size(); i++)
	{
		renderList.push_back(&pickups[i]->getPlayerModel());
	}

	renderer.begin();
	renderer.setView(camera.GetViewMatrix());
	renderer.setProjection(glm::perspective(1.0f, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 5000.0f));

	glViewport(0, 0, 1280, 720);
	renderer.drawSkybox(scene->getSkybox());
	renderer.drawTerrain(scene->getTerrain());
	renderer.setShader("Phong-interpolated");

	renderer.render(renderList);

	if (camera.isFPS()) 
	{
		renderer.renderFirstPerson(&scene->getPlayer()->getVPWeapon());
	}

	renderList.clear();
}

void Game::DrawHud()
{
	glViewport(0, 0, 1280, 720);
	renderer.setShader("UI");

	if (!scene->getPlayer()->getIsDead() && !countdown.finished())
	{
		renderer.renderUI(scoreLabel, glm::vec3(0.8575f, 0.45f, 0.0f));
		renderer.renderUI(healthLabel, glm::vec3(-0.6f, 0.79f, 0.0f));
		renderer.renderUI(ammoLabel, glm::vec3(-0.825f, -0.84f, 0.0f));
		renderer.renderUI(timeLabel, glm::vec3(-0.825f, -0.76f, 0.0f));

		if (scene->getPlayer()->Aiming())
			renderer.renderUI(crosshair, glm::vec3(0), glm::vec3(50, 50, 1));

		renderer.renderUI(HUD, glm::vec3(0), glm::vec3(1280, 720, 1));
	}
	else if (!scene->getPlayer()->getIsDead() && countdown.finished())
	{
		renderer.renderUI(victory, glm::vec3(0));
	}
	else if (scene->getPlayer()->getIsDead())
	{
		renderer.renderUI(failure, glm::vec3(0));
	}
}

void Game::draw()
{
	DrawScene();

	DrawMinimap();

	DrawHud();

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

	countdown.update();
	timeLabel->setString(countdown.toString());
	scoreLabel->setString("SCORE: " + std::to_string(score));
	healthLabel->setString("HEALTH: " + std::to_string(scene->getPlayer()->getHealth()));
	ammoLabel->setString("AMMO: " + std::to_string(scene->getPlayer()->getAmmo()));

	scene->getPlayer()->UpdateVectors(camera.GetFront());
	scene->getPlayer()->setFPS(camera.isFPS());

	auto freezeControls = countdown.finished() || scene->getPlayer()->getIsDead();
	input.Update(scene->getPlayer(), camera, freezeControls);

	//Update AI controllers
	for (auto i = 0; i < npcControllers.size(); i++)
	{
		npcControllers[i].update();
	}

	auto pickups = scene->getPickups();
	for (auto i = 0; i < pickups.size(); i++)
	{
		pickups[i]->update(gameTime.GetDeltaTime());
	}

	auto& npcs = scene->getNPCs();
	for (size_t i = 0; i < npcs.size(); i++)
	{

		if (npcs[i]->getIsDead())
		{
			npcs.erase(npcs.begin() + i);
			i--;
		}
		else
		{
			npcs[i]->update(gameTime.GetDeltaTime());
			npcs[i]->ClampPosition(glm::vec3(-scene->getTerrain()->getScale().x / 2 - 1, 0, -scene->getTerrain()->getScale().z / 2 - 1), glm::vec3(scene->getTerrain()->getScale().x / 2 - 1, scene->getTerrain()->getScale().y + 50, scene->getTerrain()->getScale().z / 2 - 1));
		}

	}

	scene->getPlayer()->update(gameTime.GetDeltaTime());
	scene->getPlayer()->ClampPosition(glm::vec3(-scene->getTerrain()->getScale().x/2-1, 0, -scene->getTerrain()->getScale().z / 2-1), glm::vec3(scene->getTerrain()->getScale().x / 2-1, scene->getTerrain()->getScale().y + 250, scene->getTerrain()->getScale().z / 2-1));
	checkCollisions();

	camera.Update(gameTime.GetDeltaTime(), scene->getPlayer()->getPosition() - glm::vec3(0, -24, 0));
	if (!endSoundPlayed)
	{
		if (scene->getPlayer()->getIsDead())
		{
			AudioManager::PlaySound("res/audio/sfx/YouDied.wav");
			endSoundPlayed = true;
		}
		else if (countdown.finished())
		{
			AudioManager::PlaySound("res/audio/sfx/YouWin.wav");
			endSoundPlayed = true;
		}
	}
}

Game::Game() : countdown(2 * 60)
{
	init();
	while (running)
	{
		gameTime.Reset();

		//process individual frame's worth of updates
		while (gameTime.ProcessFrame())
		{
			gameTime.Update();
			update();
		}

		//render processed frame
		if (gameTime.FrameComplete())
		{
			gameTime.IncrementFrames();
			draw();
		}
		else
		{
			gameTime.Sleep();
		}
	}
	renderer.quit();
	content.unloadAll();
}

bool playerCollision(Player* p1, Player* p2)
{
	auto info = Collisions::TestAABBAABB(p1->getAABB(), p2->getAABB());
	if (info.collision)
	{
		auto pos = p2->getPosition();
		p2->setPosition(pos -= info.mtv);
	}
	return info.collision;
}

bool playerCollision(Player* p1, Pickup* p2)
{
	auto info = Collisions::TestAABBAABB(p1->getAABB(), p2->getAABB());
	if (info.collision)
	{
		return true;
	}
	return false;
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

	auto pickups = scene->getPickups();
	for (auto i = 0; i < pickups.size(); i++)
	{
		Collisions::terrainCollision(pickups[i], scene->getTerrain());

		if (playerCollision(scene->getPlayer(), pickups[i]))
		{
			scene->getPlayer()->giveAmmo(20);
			audioManager.PlaySound("res/audio/sfx/GunPickup.wav");
			scene->removePickup(i);
		}
	}

	//Construct a ray
	Collisions::Ray ray;
	glm::vec4 screenPos = glm::vec4(0, 0, 1.0f, 1.0f);
	glm::mat4 proj = glm::perspective(1.0f, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 5000.0f);
	glm::mat4 view = lookAt(glm::vec3(camera.getPosition()),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 invVP = glm::inverse(proj * view);
	glm::vec4 worldPos = invVP * screenPos;
	//origin = glm::normalize(origin);
	ray.origin = scene->getPlayer()->getPosition() + glm::vec3(0, 24, 0);
	ray.dir = glm::normalize(glm::vec3(worldPos));

	for (size_t i = 0; i < npcs.size(); i++)
	{
		if (playerCollision(scene->getPlayer(), npcs[i].get()))
		{
			scene->getPlayer()->takeDamage(1);
		}
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
		npcs[i]->ClampPosition(glm::vec3(-scene->getTerrain()->getScale().x / 2 - 1, 0, -scene->getTerrain()->getScale().z / 2 - 1), glm::vec3(scene->getTerrain()->getScale().x / 2 - 1, scene->getTerrain()->getScale().y + 200, scene->getTerrain()->getScale().z / 2 - 1));
		Collisions::terrainCollision(npcs[i].get(), scene->getTerrain());		
	}
	if (scene->getPlayer()->getCanShoot() && (camera.isFPS() || scene->getPlayer()->Aiming()))
	{
		Player* npc = nullptr;
		for (int i = 0; i < npcs.size(); i++)
		{
			if (Collisions::TestRayAABB(ray, npcs[i].get()->getAABB()))
			{
				if (npc == nullptr)
				{
					npc = npcs[i].get();
				}
				else
				{
					float d1 = glm::length(ray.origin - npc->getPosition());
					float d2 = glm::length(ray.origin - npcs[i]->getPosition());
					if (d2 < d1)
					{
						npc = npcs[i].get();
					}
				}
			}
		}
		if (npc != nullptr)
		{
			npc->takeDamage(50);
			if(npc->getIsDead())
			{
				score += 5000;
			}
		}
		
		scene->getPlayer()->hasShot();
		
	}
	
}
