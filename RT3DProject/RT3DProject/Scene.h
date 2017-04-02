#pragma once
#include "Player.h"
#include "Terrain.h"
#include "ResourceManager.h"
#include "Skybox.h"
#include "StaticObject.h"
#include "Pickup.h"

class Scene
{
public:
	Scene();
	virtual ~Scene();
	void loadContent(Utilities::ResourceManager& content);
	Player* getPlayer() const { return player; }
	Rendering::Skybox* getSkybox() const { return skybox; }
	const Terrain* getTerrain() const { return terrain; }
	std::vector<std::shared_ptr<Player>>& getNPCs() { return npcs; }
	std::vector<StaticObject*>& getStaticObjects() { return staticObjects; }
	std::vector<Pickup*> getPickups() const { return pickups; }
	void removePickup(const int& index);
private:
	Rendering::Skybox* skybox;
	Terrain* terrain;
	Player* player;
	std::vector<Pickup*> pickups;
	std::vector<std::shared_ptr<Player>> npcs;
	std::vector<rt3d::lightStruct> lights;
	std::vector<StaticObject*> staticObjects;
};

