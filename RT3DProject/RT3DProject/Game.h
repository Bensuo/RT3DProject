#pragma once

#include "md2model.h"
#include "Box.h"
#include "Skybox.h"
#include "Camera.h"
#include "Renderer.h"
#include "Player.h"
#include "AABB.h"
#include "Input.h"
#include "Timer.h"
#include "Terrain.h"
#include "AudioManager.h"
#include "Scene.h"
#include "UI.h"


#define DEG_TO_RADIAN 0.017453293

class Game
{
	Utilities::ResourceManager content;
	Renderer renderer;
	AudioManager audioManager;
	Camera camera;
	Input input;
	Timer timer;
	Scene* scene;

	const unsigned SCREEN_HEIGHT = 720;
	const unsigned SCREEN_WIDTH = 1280;
	
	bool running = true;

	std::vector<IRenderable*> renderList;
	std::vector<IRenderable*> fpRenderList;
	Rendering::UI* uiTest;
	Rendering::UI* uiTest2;
	Rendering::Box testBox1;
	Rendering::Box testBox2;
	rt3d::materialStruct material = {
		{ 0.4f, 0.4f, 1.0f, 0.2f }, // ambient
		{ 0.8f, 0.8f, 1.0f, 0.2f }, // diffuse
		{ 0.8f, 0.8f, 0.8f, 0.2f }, // specular
		0.2f  // shininess
	};
	void checkCollisions();
public:
	void init(void);
	void draw();
	bool Quit() const;
	void update();
	Game();
};

