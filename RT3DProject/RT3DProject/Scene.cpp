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
	terrain->loadContent("newhm.bmp", "newhm-normal.bmp", content);
	player = new Player();
	player->loadContent(content, "rampage");
	player->setState(Player::STAND);
	player->setPosition(glm::vec3(100, 15, -200));
	for (int i = 0; i < 2; i++)
	{
		Player* p = new Player();
		std::shared_ptr<Player> shared(p);
		p->loadContent(content, "rampage");
		p->setState(Player::STAND);
		p->setPosition(glm::vec3(-500+ 50*i, 15, -200 + 50*i));
		npcs.push_back(shared);
	}
	StaticObject* m = new StaticObject();
	m->loadContent(content, "res/md2/triax_hoover", "res/md2/triax_hoover");
	m->setTransform(Transform{ glm::vec3(170, terrain->getHeightAtPosition(170, 0), 0), glm::vec3(0) });
	staticObjects.push_back(m);
}
