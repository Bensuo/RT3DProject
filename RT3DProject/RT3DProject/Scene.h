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
	const Rendering::Skybox* getSkybox() const { return skybox; }
	const Terrain* getTerrain() const { return terrain; }
	const std::vector<std::shared_ptr<Player>>& getNPCs() const { return npcs; }
	const std::vector<StaticObject*>& getStaticObjects() const { return staticObjects; }
	const std::vector<Pickup*>& getPickups() const { return pickups; }
	void removePickup(const int& index);
	void removeNPC(const int& index);
private:
	Rendering::Skybox* skybox;
	Terrain* terrain;
	Player* player;
	std::vector<Pickup*> pickups;
	std::vector<std::shared_ptr<Player>> npcs;
	std::vector<rt3d::lightStruct> lights;
	std::vector<StaticObject*> staticObjects;
};