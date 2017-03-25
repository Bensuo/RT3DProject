#include "Scene.h"



Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::loadContent(Utilities::ResourceManager& content)
{
	skybox = new Rendering::Skybox("res/textures/front.bmp",
		"res/textures/back.bmp",
		"res/textures/top.bmp",
		"res/textures/bottom.bmp",
		"res/textures/left.bmp",
		"res/textures/right.bmp",
		"res/shaders/skyboxVertex.vs",
		"res/shaders/skyboxFragment.fs");
	terrain = new Terrain();
	terrain->setScale(glm::vec3(2000.0f, 100.0f, 2000.0f));
	terrain->loadContent("heightmap.bmp", "heightmap-norm.bmp", content);
	player = new Player();
	player->loadContent(content, "rampage");
	player->setState(Player::STAND);
	player->setPosition(glm::vec3(100, 15, -200));
	for (int i = 0; i < 2; i++)
	{
		Player* p = new Player();
		p->loadContent(content, "rampage");
		p->setState(Player::STAND);
		p->setPosition(glm::vec3(100+ 200*i, 15, -200));
		npcs.push_back(p);
	}
}
