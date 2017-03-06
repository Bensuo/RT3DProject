#pragma once
#include <stack>
#include <glm/glm.hpp>
#include <SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "rt3d.h"
#include "shader.h"
#include "Model.h"
#include "skybox.h"
#include "ViewportWeapon.h"
#include <map>
class Renderer
{
public:
	Renderer();
	virtual ~Renderer();
	void quit();
	void begin(Camera camera);
	void end();
	void draw(Rendering::Model* model);
	void drawSkybox(Rendering::Skybox* skybox, Camera& camera);
	void render(std::vector<Rendering::Model*>& fpModels, std::vector<Rendering::Model*>& models, Camera camera);

	void setShader(std::string name);
private:
	std::stack<glm::mat4> mvStack;
	const unsigned SCREEN_HEIGHT = 720;
	const unsigned SCREEN_WIDTH = 1280;
	SDL_Window* setupRC(SDL_GLContext& context);
	void init();

	SDL_Window* hWindow;
	SDL_GLContext glContext;

	//TODO: Replace with maps?
	std::map<std::string, Rendering::Shader> shaders;
	Rendering::Shader* currentShader;
	rt3d::lightStruct light0 = {
		{ 0.66f, 0.66f, 0.75f, 1.0f }, // ambient
		{ 0.08f, 0.13f, 0.26f, 1.0f }, // diffuse
		{ 0.15f, 0.34f, 1.0f, 1.0f }, // specular
		{ -10.0f, 10.0f, 10.0f, 1.0f }  // position
	};

	glm::vec4 lightPos; //light position

};

