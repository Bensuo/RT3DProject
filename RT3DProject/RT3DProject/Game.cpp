#include "Game.h"
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

	AudioManager::init(content);
	AudioManager::playMusic("res/audio/music/02 - Rip & Tear.mp3", 0.5f);

	gameTime.initialize();	//always init last for accurate game loop startup
	countdown.startTimer();
}

void Game::drawMinimap()
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
		renderList.push_back(scene->getPlayer()->getPlayerModel());
		renderList.push_back(scene->getPlayer()->getWeapon());

		auto& npcs = scene->getNPCs();
		for (auto i = 0; i < npcs.size(); i++)
		{
			renderList.push_back(npcs[i]->getPlayerModel());
			renderList.push_back(npcs[i]->getWeapon());
		}

		auto pickups = scene->getPickups();
		for (auto i = 0; i < pickups.size(); i++)
		{
			renderList.push_back(pickups[i]->getModel());
		}

		renderer.drawTerrain(scene->getTerrain());
		renderer.setShader("Phong-interpolated");

		renderer.render(renderList);
		renderList.clear();

		glEnable(GL_DEPTH_TEST);
	}
}

void Game::drawScene()
{
	//Populate render list
	auto& npcs = scene->getNPCs();
	if (!camera.isFPS()) {
		renderList.push_back(scene->getPlayer()->getPlayerModel());
		if (scene->getPlayer()->playerState != Player::PlayerState::DEATH1)
		{
			renderList.push_back(scene->getPlayer()->getWeapon());
		}
	}

	for (auto i = 0; i < npcs.size(); i++)
	{
		renderList.push_back(npcs[i]->getPlayerModel());
		if (npcs[i]->playerState != Player::PlayerState::DEATH1)
		{
			renderList.push_back(npcs[i]->getWeapon());
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
		renderList.push_back(pickups[i]->getModel());
	}

	renderer.begin();
	renderer.setView(camera.getViewMatrix());
	renderer.setProjection(glm::perspective(1.0f, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 5000.0f));

	glViewport(0, 0, 1280, 720);
	renderer.drawSkybox(scene->getSkybox());
	renderer.drawTerrain(scene->getTerrain());
	renderer.setShader("Phong-interpolated");

	renderer.render(renderList);

	if (camera.isFPS()) 
	{
		renderer.renderFirstPerson(scene->getPlayer()->getVPWeapon());
	}

	renderList.clear();
}

void Game::drawHud()
{
	glViewport(0, 0, 1280, 720);
	renderer.setShader("UI");

	if (!scene->getPlayer()->getIsDead() && !countdown.finished())
	{
		renderer.renderUI(scoreLabel, glm::vec3(0.8575f, 0.45f, 0.0f));
		renderer.renderUI(healthLabel, glm::vec3(-0.6f, 0.79f, 0.0f));
		renderer.renderUI(ammoLabel, glm::vec3(-0.825f, -0.84f, 0.0f));
		renderer.renderUI(timeLabel, glm::vec3(-0.825f, -0.76f, 0.0f));

		if (scene->getPlayer()->aiming())
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
	drawScene();

	drawMinimap();

	drawHud();

	renderer.swapBuffers();
}

bool Game::quit() const
{
	return input.quit();
}

void Game::update()
{
	running = !quit();

	countdown.update();
	timeLabel->setString(countdown.toString());
	scoreLabel->setString("SCORE: " + std::to_string(score));
	healthLabel->setString("HEALTH: " + std::to_string(scene->getPlayer()->getHealth()));
	ammoLabel->setString("AMMO: " + std::to_string(scene->getPlayer()->getAmmo()));

	scene->getPlayer()->updateVectors(camera.getFront());
	scene->getPlayer()->setFPS(camera.isFPS());

	auto freezeControls = countdown.finished() || scene->getPlayer()->getIsDead();
	input.update(scene->getPlayer(), camera, freezeControls);

	//Update AI controllers
	for (auto i = 0; i < npcControllers.size(); i++)
	{
		npcControllers[i].update();
	}

	auto pickups = scene->getPickups();
	for (auto i = 0; i < pickups.size(); i++)
	{
		pickups[i]->update(gameTime.getDeltaTime());
	}

	auto& npcs = scene->getNPCs();
	for (size_t i = 0; i < npcs.size(); i++)
	{
		if (npcs[i]->getIsDead())
		{
			scene->removeNPC(i);
			i--;
		}
		else
		{
			npcs[i]->update(gameTime.getDeltaTime());
			npcs[i]->clampPosition(glm::vec3(-scene->getTerrain()->getScale().x / 2 - 1, 0, -scene->getTerrain()->getScale().z / 2 - 1), glm::vec3(scene->getTerrain()->getScale().x / 2 - 1, scene->getTerrain()->getScale().y + 50, scene->getTerrain()->getScale().z / 2 - 1));
		}
	}

	scene->getPlayer()->update(gameTime.getDeltaTime());
	scene->getPlayer()->clampPosition(glm::vec3(-scene->getTerrain()->getScale().x/2-1, 0, -scene->getTerrain()->getScale().z / 2-1), glm::vec3(scene->getTerrain()->getScale().x / 2-1, scene->getTerrain()->getScale().y + 250, scene->getTerrain()->getScale().z / 2-1));
	checkCollisions();

	camera.update(gameTime.getDeltaTime(), scene->getPlayer()->getPosition() - glm::vec3(0, -24, 0));
	if (!endSoundPlayed)
	{
		if (scene->getPlayer()->getIsDead())
		{
			AudioManager::playSound("res/audio/sfx/YouDied.wav");
			endSoundPlayed = true;
		}
		else if (countdown.finished())
		{
			AudioManager::playSound("res/audio/sfx/YouWin.wav");
			endSoundPlayed = true;
		}
	}
}

Game::Game() : countdown(60 * 60)
{
	init();
	while (running)
	{
		gameTime.reset();

		//process individual frame's worth of updates
		while (gameTime.processFrame())
		{
			gameTime.update();
			update();
		}

		//render processed frame
		if (gameTime.frameComplete())
		{
			gameTime.incrementFrames();
			draw();
		}
		else
		{
			gameTime.sleep();
		}
	}
	renderer.quit();
	content.unloadAll();
}

bool playerCollision(Player* p1, Player* p2)
{
	auto info = Collisions::testAABBAABB(p1->getAABB(), p2->getAABB());
	if (info.collision)
	{
		auto pos = p2->getPosition();
		p2->setPosition(pos -= info.mtv);
	}
	return info.collision;
}

bool playerCollision(Player* p1, Pickup* p2)
{
	auto info = Collisions::testAABBAABB(p1->getAABB(), p2->getAABB());
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
		auto info = Collisions::testAABBAABB(staticObjects[i]->getAABB(), scene->getPlayer()->getAABB());
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
			audioManager.playSound("res/audio/sfx/GunPickup.wav");
			scene->removePickup(i);
		}
	}

	//Construct a ray
	Collisions::Ray ray;
	auto screenPos = glm::vec4(0, 0, 1.0f, 1.0f);
	auto proj = glm::perspective(1.0f, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 5000.0f);
	auto view = lookAt(glm::vec3(camera.getPosition()),
		glm::vec3(0.0, 0.0, 0.0),
		glm::vec3(0.0, 1.0, 0.0));
	auto invVP = inverse(proj * view);
	auto worldPos = invVP * screenPos;

	ray.origin = scene->getPlayer()->getPosition() + glm::vec3(0, 24, 0);
	ray.dir = normalize(glm::vec3(worldPos));

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
		for (auto k = 0; k < staticObjects.size(); k++)
		{
			auto info = Collisions::testAABBAABB(staticObjects[k]->getAABB(), npcs[i]->getAABB());
			if (info.collision)
			{
				auto pos = npcs[i]->getPosition();
				npcs[i]->setPosition(pos -= info.mtv);
			}
			
		}
		npcs[i]->clampPosition(glm::vec3(-scene->getTerrain()->getScale().x / 2 - 1, 0, -scene->getTerrain()->getScale().z / 2 - 1), glm::vec3(scene->getTerrain()->getScale().x / 2 - 1, scene->getTerrain()->getScale().y + 200, scene->getTerrain()->getScale().z / 2 - 1));
		Collisions::terrainCollision(npcs[i].get(), scene->getTerrain());		
	}
	if (scene->getPlayer()->getCanShoot() && (camera.isFPS() || scene->getPlayer()->aiming()))
	{
		Player* npc = nullptr;
		for (auto i = 0; i < npcs.size(); i++)
		{
			if (testRayAABB(ray, npcs[i].get()->getAABB()))
			{
				if (npc == nullptr)
				{
					npc = npcs[i].get();
				}
				else
				{
					auto d1 = length(ray.origin - npc->getPosition());
					auto d2 = length(ray.origin - npcs[i]->getPosition());
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
