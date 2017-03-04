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

#define DEG_TO_RADIAN 0.017453293

class CoreEngine
{
	rt3d::lightStruct light0 = {
		{ 0.66f, 0.66f, 0.75f, 1.0f }, // ambient
		{ 0.08f, 0.13f, 0.26f, 1.0f }, // diffuse
		{ 0.15f, 0.34f, 1.0f, 1.0f }, // specular
		{ -10.0f, 10.0f, 10.0f, 1.0f }  // position
	};

	glm::vec4 lightPos; //light position

	std::stack<glm::mat4> mvStack;

	Utilities::ResourceManager content;
	Utilities::Clock m_clock;

	Camera m_camera;
	Rendering::Shader shader;
	Rendering::PlayerModel playerCharacter;
	Rendering::PlayerModel playerWeapon;
	Rendering::ViewportWeapon viewportWeapon;

	Rendering::Box testBox;
	Rendering::Skybox* m_skybox;

	const unsigned SCREEN_HEIGHT = 720;
	const unsigned SCREEN_WIDTH = 1280;
public:
	SDL_Window* setupRC(SDL_GLContext& context);
	void init(void);
	void draw(SDL_Window* window);
	void update();
	CoreEngine();
};

