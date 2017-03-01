#include <SDL.h>
#undef main
#include "rt3d.h"
#include <stack>
#include <stdio.h>
#include "md2model.h"
#include "Actor.h"
#include "Box.h"
#include "skybox.h"
#include "camera.h"
#include <glm/gtc/type_ptr.hpp>

#define DEG_TO_RADIAN 0.017453293

rt3d::lightStruct light0 = {
	{ 0.3f, 0.3f, 0.3f, 1.0f }, // ambient
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse
	{ 1.0f, 1.0f, 1.0f, 1.0f }, // specular
	{ -10.0f, 10.0f, 10.0f, 1.0f }  // position
};
glm::vec4 lightPos(-10.0f, 10.0f, 10.0f, 1.0f); //light position

GLuint meshObjects[1];
std::stack<glm::mat4> mvStack;
md2model testModel;
GLuint md2VertCount = 0;
Actor testActor;
Box testBox;
ResourceManager content;
rendering::shader shader;
camera m_camera;
rendering::skybox* m_skybox;

const int screenHeight = 720;
const int screenWidth = 1280;

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
		screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window) // Check window was created OK
		rt3d::exitFatalError("Unable to create window");

	context = SDL_GL_CreateContext(window); // Create opengl context and attach to window
	SDL_GL_SetSwapInterval(1); // set swap buffers to sync with monitor's vertical refresh rate
	return window;
}

void init(void)
{
	m_camera.Position = glm::vec3(0,0,50);
	shader = rendering::shader("phong-tex.vert", "phong-tex.frag");
	shader.setLight(light0);
	m_skybox = new rendering::skybox("res/textures/front.bmp",
		"res/textures/back.bmp",
		"res/textures/top.bmp",
		"res/textures/bottom.bmp",
		"res/textures/left.bmp",
		"res/textures/right.bmp",
		"res/shaders/skyboxVertex.vs",
		"res/shaders/skyboxFragment.fs");

	testActor.loadContent(content);
	meshObjects[0] = testModel.ReadMD2Model("yoshi.md2");
	md2VertCount = testModel.getVertDataCount();
	testBox = Box(glm::vec3(100,1,100), glm::vec3(0,-20,0));
	testBox.loadContent(content);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void draw(SDL_Window* window)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT);

	m_skybox->render(m_camera);

	//Clear the screen
	glClear(GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection(1.0);
	projection = glm::perspective(m_camera.Zoom, static_cast<float>(screenWidth) / screenHeight, 1.0f, 500.0f);
	shader.use();
	shader.setUniformMatrix4fv("projection", value_ptr(projection));
	mvStack.push(m_camera.GetViewMatrix());

	//Draw yoshi
	testActor.draw(mvStack, projection, shader.getProgram());
	testBox.draw(mvStack, projection, shader.getProgram());

	mvStack.pop();
	SDL_GL_SwapWindow(window); // swap buffers
}

void update()
{
	testActor.update(0.1f);
	testBox.update();
}
int main(int argc, char* argv[])
{
	SDL_Window * hWindow;
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