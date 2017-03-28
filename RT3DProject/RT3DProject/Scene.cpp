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

	auto pickup = new Pickup();
	pickup->loadContent(content, "g_hyperb");
	pickup->setPosition(glm::vec3(500, 40, -400));
	pickups.push_back(pickup);

	pickup = new Pickup();
	pickup->loadContent(content, "g_machn");
	pickup->setPosition(glm::vec3(200, 40, -100));
	pickups.push_back(pickup);

	pickup = new Pickup();
	pickup->loadContent(content, "g_launch");
	pickup->setPosition(glm::vec3(0, 40, 0));
	pickups.push_back(pickup);

	pickup = new Pickup();
	pickup->loadContent(content, "g_chain");
	pickup->setPosition(glm::vec3(300, 40, -300));
	pickups.push_back(pickup);

	for (int i = 0; i < 2; i++)
	{
		Player* p = new Player();
		std::shared_ptr<Player> shared(p);
		p->loadContent(content, "rampage");
		p->setState(Player::STAND);
		p->setPosition(glm::vec3(-500+ 50*i, 15, -200 + 50*i));
		npcs.push_back(shared);
	}
}

void Scene::removePickup(const int& index)
{
	std::cout << "Pickup removed." << std::endl;
	pickups.erase(pickups.begin() + index);
}
