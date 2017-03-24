#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>


Renderer::Renderer()
{
	hWindow = setupRC(glContext);
	glewExperimental = GL_TRUE;
	auto err = glewInit();
	if (GLEW_OK != err)
	{ // glewInit failed, something is seriously wrong
		std::cout << "glewInit failed, aborting." << std::endl;
		exit(1);
	}
	std::cout << glGetString(GL_VERSION) << std::endl;
		lightPos = glm::vec4(-10.0f, 10.0f, 10.0f, 1.0f); //light position
	init();
}


Renderer::~Renderer()
{
}

SDL_Window* Renderer::setupRC(SDL_GLContext& context) const
{
	SDL_Window* window;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialize video
		rt3d::exitFatalError("Unable to initialize SDL");

	// Request an OpenGL 3.0 context.
	// Not able to use SDL to choose profile (yet), should default to core profile on 3.2 or later
	// If you request a context not supported by your drivers, no OpenGL context will be created

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // double buffering on
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // Turn on x4 multisampling anti-aliasing (MSAA)
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); // 8 bit alpha buffering
													   // Create 800x600 window
	window = SDL_CreateWindow("RT3DProject", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window) // Check window was created OK
		rt3d::exitFatalError("Unable to create window");

	context = SDL_GL_CreateContext(window); // Create opengl context and attach to window
	SDL_GL_SetSwapInterval(1); // set swap buffers to sync with monitor's vertical refresh rate
	return window;
}

void Renderer::init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Set up shaders
	shaders.insert(std::make_pair("Phong", Rendering::Shader("phong-tex.vert", "phong-tex.frag")));
	shaders["Phong"].setLight(light0);
	shaders.insert(std::make_pair("Phong2", Rendering::Shader("phong-tex.vert", "phong-tex.frag")));
	shaders["Phong2"].setLight(light1);

}

//Set the shader to be used for rendering
void Renderer::setShader(std::string name)
{
	if (shaders.find(name) != shaders.end())
	{
		currentShader = &shaders[name];
		//currentShader->use();
	}
}

void Renderer::quit() const
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(hWindow);
	SDL_Quit();
}

//Clears buffers to begin rendering
void Renderer::begin(Camera& camera)
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	this->camera = &camera;
}

//Ends rendering and pushes the result to the screen
void Renderer::end() const
{
	SDL_GL_SwapWindow(hWindow); //Swap buffers
}

//Draws an object implementing the IRenderable interface
void Renderer::draw(IRenderable* renderable)
{
	glCullFace(GL_FRONT);
	glBindTexture(GL_TEXTURE_2D, renderable->getTexture());

	mvStack.push(mvStack.top());

	float rotation = renderable->getTransform().rotation.x * DEG_TO_RADIAN;
	
	
	mvStack.top() = translate(mvStack.top(), renderable->getTransform().position);
	mvStack.push(rotate(mvStack.top(), rotation, glm::vec3(1.0f, 0.0f, 0.0f)));
	currentShader->setUniformMatrix4fv("modelview", glm::value_ptr(mvStack.top()));
	currentShader->setMaterial(renderable->getMaterial());
	if (renderable->isIndexed())
	{
		rt3d::drawIndexedMesh(renderable->getMesh(), renderable->getCount(), GL_TRIANGLES);
	}
	else
	{
		rt3d::drawMesh(renderable->getMesh(), renderable->getCount(), GL_TRIANGLES);
	}
	mvStack.pop();
	mvStack.pop();

	glCullFace(GL_BACK);
}


void Renderer::drawSkybox(Rendering::Skybox* skybox) const
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	skybox->shader.use();
	auto projection = glm::perspective(static_cast<float>(1), 1600.0f / 900, 0.1f, 1000.0f);
	glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(skybox->shader.getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(skybox->shader.getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));

	glBindVertexArray(skybox->vao);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	//clear the depth to ensure Skybox is always drawn on bottom
	glClear(GL_DEPTH_BUFFER_BIT);
	skybox->shader.disable();
}

void Renderer::drawTerrain(Terrain * terrain) const
{
	glCullFace(GL_BACK);
	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
	terrain->shader.use();
	terrain->shader.setLight(light0);
	//terrain->shader.setLight(light0);

	glm::mat4 projection(1.0);
	projection = glm::perspective(camera->currentZoom, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 1.0f, 500.0f);
	glm::mat4 view = camera->GetViewMatrix();
	//view = glm::translate(view, glm::vec3(0, -50, 0));
	terrain->shader.setUniformMatrix4fv("projection", glm::value_ptr(projection));
	terrain->shader.setUniformMatrix4fv("modelview", glm::value_ptr(view));
	//glUniformMatrix4fv(glGetUniformLocation(terrain->shader.getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	//glUniformMatrix4fv(glGetUniformLocation(terrain->shader.getProgram(), "modelview"), 1, GL_FALSE, glm::value_ptr(view));
	glBindTexture(GL_TEXTURE_2D, *terrain->getTexture());
	glBindVertexArray(terrain->getVAO());
	//glEnable(GL_PRIMITIVE_RESTART);
	
	//glPrimitiveRestartIndex(terrain->getRows() * terrain->getColumns());
	//rt3d::drawIndexedMesh(terrain->getVAO(), terrain->getNumIndices(), GL_TRIANGLES);
	glDrawElements(GL_TRIANGLES, terrain->getNumIndices(), GL_UNSIGNED_INT, 0);
	//glDisable(GL_PRIMITIVE_RESTART);
	glBindVertexArray(0);

	
	glCullFace(GL_BACK);
	//glClear(GL_DEPTH_BUFFER_BIT);
}

//Renders all objects in the provided list based on the camera provided in begin() and the currently
//active shader
void Renderer::render(std::vector<IRenderable*>& models)
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	//create projection from Camera data
	glm::mat4 projection(1.0);
	projection = glm::perspective(camera->currentZoom, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 1.0f, 500.0f);
	currentShader->use();
	currentShader->setUniformMatrix4fv("projection", value_ptr(projection));
	mvStack.push(camera->GetViewMatrix());
	for (auto const& m : models)
	{
		draw(m);
	}
	mvStack.pop();
}

//Renders "first person" renderables such as viewport weapon models
void Renderer::renderFirstPerson(IRenderable * renderable)
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	//create projection from Camera data
	glm::mat4 projection(1.0);
	projection = glm::perspective(camera->currentZoom, static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 1.0f, 500.0f);
	currentShader->use();
	currentShader->setUniformMatrix4fv("projection", value_ptr(projection));
	mvStack.push(glm::mat4(1));
	float rotation = 90.0f * DEG_TO_RADIAN;
	mvStack.top() = translate(mvStack.top(), glm::vec3(0, 2, -2));
	mvStack.top() = rotate(mvStack.top(), rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	glClear(GL_DEPTH_BUFFER_BIT);
	draw(renderable);
	mvStack.pop();
}
