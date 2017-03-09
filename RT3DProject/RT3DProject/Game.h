#pragma once

#include <SDL.h>
#include <stack>
#include "md2model.h"
#include "Box.h"
#include "Skybox.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include "clock.h"
#include "PlayerModel.h"
#include "ViewportWeapon.h"
#include "Renderer.h"
#include "Player.h"
#include "AABB.h"

#define DEG_TO_RADIAN 0.017453293

class Game
{
	Utilities::ResourceManager content;
	Utilities::Clock m_clock;
	Renderer renderer;
	Camera m_camera;
	Rendering::Box testBox;
	Rendering::Skybox* m_skybox;

	Player* testPlayer, *testPlayer2;

	const unsigned SCREEN_HEIGHT = 720;
	const unsigned SCREEN_WIDTH = 1280;
	

	std::vector<IRenderable*> renderList;
	std::vector<IRenderable*> fpRenderList;

	//Some AABB testing stuff
	AABB aabb1{ glm::vec3(0, 0, -20), glm::vec3(25, 50, 25) };
	Rendering::Box testBox1;
	AABB aabb2{ glm::vec3(100, 0, -20), glm::vec3(25, 50, 25) };
	Rendering::Box testBox2;
public:
	void init(void);
	void draw();
	void update();
	Game();
};

