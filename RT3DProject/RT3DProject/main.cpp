#include <SDL.h>
#undef main
#include "rt3d.h"
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include <stdio.h>
#include "md2model.h"

#define DEG_TO_RADIAN 0.017453293

GLuint shaderProgram;
GLuint meshObjects[1];
std::stack<glm::mat4> mvStack;
md2model testModel;
GLuint md2VertCount = 0;
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
		800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window) // Check window was created OK
		rt3d::exitFatalError("Unable to create window");

	context = SDL_GL_CreateContext(window); // Create opengl context and attach to window
	SDL_GL_SetSwapInterval(1); // set swap buffers to sync with monitor's vertical refresh rate
	return window;
}

void init(void)
{
	glEnable(GL_DEPTH_TEST);
	shaderProgram = rt3d::initShaders("mvp.vert", "minimal.frag");
	meshObjects[0] = testModel.ReadMD2Model("yoshi.md2");
	md2VertCount = testModel.getVertDataCount();
}

void draw(SDL_Window* window)
{
	//Clear the screen
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection(1.0);
	projection = glm::perspective(float(60.0f*DEG_TO_RADIAN), 800.0f / 600.0f, 1.0f, 500.0f);

	glm::mat4 modelview(1.0);
	mvStack.push(modelview);

	testModel.Animate(0.016f);
	rt3d::updateMesh(meshObjects[0], RT3D_VERTEX, testModel.getAnimVerts(), testModel.getVertDataSize());
	//Draw yoshi
	mvStack.push(mvStack.top());
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f, 0.0f, -50.0f));
	glm::mat4 mvp = projection * mvStack.top();
	rt3d::setUniformMatrix4fv(shaderProgram, "MVP", glm::value_ptr(mvp));
	rt3d::drawMesh(meshObjects[0], md2VertCount, GL_TRIANGLES);
	mvStack.pop();
	mvStack.pop();
	SDL_GL_SwapWindow(window); // swap buffers
}

void update()
{

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