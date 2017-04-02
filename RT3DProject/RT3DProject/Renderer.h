#pragma once
#include <stack>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <SDL.h>
#include "rt3d.h"
#include "shader.h"
#include "skybox.h"
#include <map>
#include "IRenderable.h"
#include "Terrain.h"

namespace Rendering {
	class UI;
}

//The Renderer handles window creation and management as well as drawing to the screen using
//OpenGL
class Renderer
{
public:
	Renderer();
	virtual ~Renderer();
	void quit() const;
	void begin() const;
	void setView(const glm::mat4& view);
	void setProjection(const glm::mat4& projection);
	void swapBuffers() const;
	void draw(IRenderable* renderable);
	void drawSkybox(Rendering::Skybox* skybox) const;
	void drawTerrain(Terrain* terrain) const;
	void render(const std::vector<IRenderable*>& models);
	void renderFirstPerson(IRenderable* renderable);
	void setShader(std::string name);
	void renderUI(Rendering::UI * renderable, glm::vec3 position, glm::vec3 size);
	void renderUI(Rendering::UI * renderable, glm::vec3 position);
private:
	void init();
	std::stack<glm::mat4> mvStack;
	const unsigned SCREEN_HEIGHT = 720;
	const unsigned SCREEN_WIDTH = 1280;
	SDL_Window* setupRC(SDL_GLContext& context) const;
	SDL_Window* hWindow;
	SDL_GLContext glContext;

	std::map<std::string, Rendering::Shader> shaders;
	Rendering::Shader* currentShader;

	//TODO: Replace with maps?
	rt3d::lightStruct light0 = {
		{ 0.1f, 0.1f, 0.1f, 1.0f }, // ambient
		{ 0.2f, 0.4f, 0.5f, 1.0f }, // diffuse
		{ 0.15f, 0.34f, 0.7f, 0.25f }, // specular
		{ 0.0f, 0.0f, 0.0f, 0.0f }  // position
	};

	//rt3d::lightStruct light0 = {
	//	{ 0.1f, 0.1f, 0.1f, 1.0f }, // ambient
	//	{ 0.1f, 0.4f, 0.3f, 1.0f }, // diffuse
	//	{ 0.15f, 0.34f, 0.7f, 0.25f }, // specular
	//	{ 0.0f, 0.0f, 0.0f, 0.0f }  // position
	//};

	rt3d::lightStruct light1 = {
		{ 0.0f, 0.0f, 0.0f, 0.0f }, // ambient
		{ 0.0f, 0.0f, 0.0f, 0.0f }, // diffuse
		{ 0.0f, 0.0f, 0.0f, 0.0f }, // specular
		{ 20.0f, 20.0f, 0.0f, 0.0f }  // position
	};

	glm::mat4 view;
	glm::mat4 projection;

	glm::vec4 lightPos; //light position

};

