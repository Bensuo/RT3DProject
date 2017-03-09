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
#include "IRenderable.h"

//The Renderer handles window creation and management as well as drawing to the screen using
//OpenGL
class Renderer
{
public:
	Renderer();
	virtual ~Renderer();
	void quit();
	void begin(Camera& camera);
	void end();
	void draw(IRenderable* renderable);
	void drawSkybox(Rendering::Skybox* skybox);
	void render(std::vector<IRenderable*>& models);
	void renderFirstPerson(IRenderable* renderable);
	void setShader(std::string name);
private:
	void init();
	std::stack<glm::mat4> mvStack;
	const unsigned SCREEN_HEIGHT = 720;
	const unsigned SCREEN_WIDTH = 1280;
	SDL_Window* setupRC(SDL_GLContext& context);
	SDL_Window* hWindow;
	SDL_GLContext glContext;

	std::map<std::string, Rendering::Shader> shaders;
	Rendering::Shader* currentShader;

	//TODO: Replace with maps?
	rt3d::lightStruct light0 = {
		{ 0.66f, 0.66f, 0.75f, 1.0f }, // ambient
		{ 0.08f, 0.13f, 0.26f, 1.0f }, // diffuse
		{ 0.15f, 0.34f, 1.0f, 1.0f }, // specular
		{ -10.0f, 10.0f, 10.0f, 1.0f }  // position
	};
	rt3d::lightStruct light1 = {
		{ 0.66f, 0.66f, 0.75f, 1.0f }, // ambient
		{ 0.08f, 0.13f, 0.26f, 1.0f }, // diffuse
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // specular
		{ -10.0f, 10.0f, 10.0f, 1.0f }  // position
	};
	Camera* camera;
	glm::vec4 lightPos; //light position

};
