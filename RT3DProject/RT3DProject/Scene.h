#pragma once
#include "Player.h"
#include "Terrain.h"
#include "ResourceManager.h"
#include "Skybox.h"
class Scene
{
public:
	Scene();
	virtual ~Scene();
	void loadContent(Utilities::ResourceManager& content);
	Player* getPlayer() { return player; }
	Rendering::Skybox* getSkybox() { return skybox; }
	Terrain* getTerrain() { return terrain; }
	std::vector<Player*>& getNPCs() { return npcs; }

private:
	//Scene objects
	Rendering::Skybox* skybox;
	Terrain* terrain;
	Player* player;
	std::vector<Player*> npcs;
	std::vector<rt3d::lightStruct> lights;
};

