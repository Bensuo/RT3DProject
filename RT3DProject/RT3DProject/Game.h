#pragma once

#include "md2model.h"
#include "Box.h"
#include "Skybox.h"
#include "Camera.h"
#include "Renderer.h"
#include "Player.h"
#include "Input.h"
#include "Timer.h"

#define DEG_TO_RADIAN 0.017453293

class Game
{
	Utilities::ResourceManager content;
	Renderer renderer;
	Camera camera;
	Input input;
	Timer timer;
	Rendering::Box testBox;
	Rendering::Skybox* skybox;

	Player* testPlayer;

	const unsigned SCREEN_HEIGHT = 720;
	const unsigned SCREEN_WIDTH = 1280;
	
	bool running = true;

	std::vector<IRenderable*> renderList;
	std::vector<IRenderable*> fpRenderList;
public:
	void init(void);
	void draw();
	bool Quit() const;
	void update();
	Game();
};

