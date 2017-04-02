#include "Scene.h"

Scene::Scene(): skybox(nullptr), terrain(nullptr), player(nullptr)
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
	terrain->setScale(glm::vec3(4000.0f, 200.0f, 4000.0f));
	terrain->loadContent("res/textures/terrain/newhm.bmp", "res/textures/terrain/newhm-normal.bmp", content);

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

	for (auto i = 0; i < 30; i++)
	{
		auto p = new Player();
		std::shared_ptr<Player> shared(p);
		p->loadContent(content, "rampage");
		p->setState(Player::STAND);
		p->setPosition(glm::vec3(-500+ 15*i, 15, -500 + 15*i));
		npcs.push_back(shared);
	}

	auto m = new StaticObject();
	m->loadContent(content, "res/md2/triax_tracks", "res/md2/triax_tracks");
	m->setTransform(Transform{ glm::vec3(300, terrain->getHeightAtPosition(300, 0), 0), glm::vec3(0) });
	staticObjects.push_back(m);
	m = new StaticObject();
	m->loadContent(content, "res/md2/triax_wheels", "res/md2/triax_wheels");
	m->setTransform(Transform{ glm::vec3(450, terrain->getHeightAtPosition(450, -300), -300), glm::vec3(0) });
	staticObjects.push_back(m);
	m = new StaticObject();
	m->loadContent(content, "res/md2/triax_tracks", "res/md2/triax_tracks");
	m->setTransform(Transform{ glm::vec3(600, terrain->getHeightAtPosition(600, -700), -700), glm::vec3(0) });
	staticObjects.push_back(m);
}

void Scene::removePickup(const int& index)
{
	std::cout << "Pickup removed." << std::endl;
	pickups.erase(pickups.begin() + index);
}
