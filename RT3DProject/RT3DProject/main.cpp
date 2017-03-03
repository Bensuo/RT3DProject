#include <SDL.h>
#undef main
#include "rt3d.h"
#include <stack>
#include <stdio.h>
#include "md2model.h"
#include "Model.h"
#include "Box.h"
#include "Skybox.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include "clock.h"
#include "PlayerModel.h"
#include "ViewportWeapon.h"

#define DEG_TO_RADIAN 0.017453293

rt3d::lightStruct light0 = {
	{ 0.66f, 0.66f, 0.75f, 1.0f }, // ambient
	{ 0.08f, 0.13f, 0.26f, 1.0f }, // diffuse
	{ 0.15f, 0.34f, 1.0f, 1.0f }, // specular
	{ -10.0f, 10.0f, 10.0f, 1.0f }  // position
};
glm::vec4 lightPos(-10.0f, 10.0f, 10.0f, 1.0f); //light position

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

SDL_Window * setupRC(SDL_GLContext &context)
{
	SDL_Window * window;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialize video
		rt3d::exitFatalError("Unable to initialize SDL");

	// Request an OpenGL 3.0 context.
	// Not able to use SDL to choose profile (yet), should default to core profile on 3.2 or later
	// If you request a context not supported by your drivers, no OpenGL context will be created

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  // double buffering on
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // Turn on x4 multisampling anti-aliasing (MSAA)

													   // Create 800x600 window
	window = SDL_CreateWindow("RT3DProject", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window) // Check window was created OK
		rt3d::exitFatalError("Unable to create window");

	context = SDL_GL_CreateContext(window); // Create opengl context and attach to window
	SDL_GL_SetSwapInterval(1); // set swap buffers to sync with monitor's vertical refresh rate
	return window;
}

void init(void)
{
	m_camera.Position = glm::vec3(0,0,200);
	shader = Rendering::Shader("phong-tex.vert", "phong-tex.frag");
	shader.setLight(light0);
	m_skybox = new Rendering::Skybox("res/textures/front.bmp",
		"res/textures/back.bmp",
		"res/textures/top.bmp",
		"res/textures/bottom.bmp",
		"res/textures/left.bmp",
		"res/textures/right.bmp",
		"res/shaders/skyboxVertex.vs",
		"res/shaders/skyboxFragment.fs");

	playerCharacter.loadContent(content, "res/md2/rampage");
	playerCharacter.setAnimation(1);
	playerWeapon.loadContent(content, "res/md2/weapon");
	playerWeapon.setAnimation(1);
	viewportWeapon.loadContent(content, "res/md2/v_machn");
	viewportWeapon.setAnimation(2);

	testBox = Rendering::Box(glm::vec3(100,1,100), glm::vec3(0,-23,0));
	testBox.loadContent(content);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void draw(SDL_Window* window)
{
	//reset draw
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT);

	//render Skybox
	m_skybox->render(m_camera);

	//clear the depth to ensure Skybox is always drawn on top
	glClear(GL_DEPTH_BUFFER_BIT);

	//create projection from Camera data
	glm::mat4 projection(1.0);
	projection = glm::perspective(m_camera.Zoom, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 1.0f, 500.0f);

	//Use default phong Shader
	shader.use();
	shader.setUniformMatrix4fv("projection", value_ptr(projection));

	mvStack.push(inverse(m_camera.GetViewMatrix()));
		float rotation = 90.0f * DEG_TO_RADIAN;
		mvStack.top() = translate(mvStack.top(), -m_camera.Position);
		mvStack.top() = translate(mvStack.top(), glm::vec3(0,2,-2));
		mvStack.top() = rotate(mvStack.top(), rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		viewportWeapon.draw(mvStack, shader.getProgram());
	mvStack.pop();

	mvStack.push(m_camera.GetViewMatrix());
	{
		//Draw baddie
		playerWeapon.draw(mvStack, shader.getProgram());
		playerCharacter.draw(mvStack, shader.getProgram());
		//Draw floor
		testBox.draw(mvStack, shader.getProgram());
		shader.disable();
	}
	mvStack.pop();

	SDL_GL_SwapWindow(window); //Swap buffers
}

void update()
{
	playerWeapon.update(0.1f);
	playerCharacter.update(0.1f);
	viewportWeapon.update(0.1f);
	testBox.update();
	m_camera.Update(1 / 60.0f);
}

int main(int argc, char* argv[])
{
	SDL_Window* hWindow;
	SDL_GLContext glContext;
	hWindow = setupRC(glContext);
	
	// Required on Windows *only* init GLEW to access OpenGL beyond 1.1
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) { // glewInit failed, something is seriously wrong
		std::cout << "glewInit failed, aborting." << std::endl;
		exit(1);
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	init();

	bool running = true; // set running to true
	SDL_Event sdlEvent;  // variable to detect SDL events
	while (running) {	// the event loop
		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_QUIT)
				running = false;
		}
		update();
		draw(hWindow); // call the draw function
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(hWindow);
	SDL_Quit();
	return 0;
}