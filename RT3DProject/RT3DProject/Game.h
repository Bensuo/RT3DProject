#pragma once

#include "md2model.h"
#include "Box.h"
#include "camera.h"
#include "Renderer.h"
#include "Player.h"
#include "Input.h"
#include "GameTime.h"
#include "Terrain.h"
#include "AudioManager.h"
#include "Scene.h"
#include "UI.h"
#include "AIController.h"
#include "Timer.h"


#define DEG_TO_RADIAN 0.017453293

class Game
{
	int score = 0;

	Utilities::ResourceManager content;
	Renderer renderer;
	static AudioManager audioManager;
	Camera camera;
	Input input;
	GameTime gameTime;
	Scene* scene;
	Timer countdown;

	const unsigned SCREEN_HEIGHT = 720;
	const unsigned SCREEN_WIDTH = 1280;
	
	bool running = true;
	bool endSoundPlayed = false;
	std::vector<const IRenderable*> renderList;
	std::vector<const IRenderable*> fpRenderList;
	Rendering::UI* failure;
	Rendering::UI* victory;

	Rendering::UI* scoreLabel;
	Rendering::UI* healthLabel;
	Rendering::UI* ammoLabel;
	Rendering::UI* timeLabel;
	Rendering::UI* HUD;
	Rendering::UI* crosshair;
	Rendering::Box testBox1;
	Rendering::Box testBox2;
	rt3d::materialStruct material = {
		{ 0.4f, 0.4f, 1.0f, 0.2f }, // ambient
		{ 0.8f, 0.8f, 1.0f, 0.2f }, // diffuse
		{ 0.8f, 0.8f, 0.8f, 0.2f }, // specular
		0.2f  // shininess
	};
	void checkCollisions();
	std::vector<AIController> npcControllers;
public:
	void init();
	void drawMinimap();
	void drawScene();
	void drawHud();
	void draw();
	const bool& quit() const;
	void update();
	Game();
};