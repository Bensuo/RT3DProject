#pragma once

#include "md2model.h"
#include "Box.h"
#include "Skybox.h"
#include "Camera.h"
#include "Renderer.h"
#include "Player.h"
#include "AABB.h"
#include "Input.h"
#include "GameTime.h"
#include "Terrain.h"
#include "AudioManager.h"
#include "Scene.h"
#include "UI.h"
#include "AIController.h"


#define DEG_TO_RADIAN 0.017453293

class Timer
{
	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::duration<double> elapsed;
	int duration;

public:

	explicit Timer(int duration) : duration(duration) {}

	void startTimer()
	{
		start = std::chrono::system_clock::now();
	}

	void update()
	{
		if(elapsed.count() < duration) 
		{
			end = std::chrono::system_clock::now();
			elapsed = end - start;
		}
	}

	bool finished() const
	{
		return elapsed.count() >= duration;
	}

	std::string toString() const
	{
		std::string result;

		int minutes = (duration - elapsed.count()) / 60.0f;
		int seconds = (duration - elapsed.count()) - 60 * minutes;

		if (minutes < 10) {
			result += "0" + std::to_string(minutes) + ":";
		} else {
			result += std::to_string(minutes) + ":";
		}

		if (seconds < 10) {
			result += "0" + std::to_string(seconds);
		} else {
			result += std::to_string(seconds);
		}

		return result;
	}
};

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

	std::vector<IRenderable*> renderList;
	std::vector<IRenderable*> fpRenderList;
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
	void init(void);
	void DrawMinimap();
	void DrawScene();
	void DrawHud();
	void draw();
	bool Quit() const;
	void update();
	Game();
};