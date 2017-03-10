#include "Game.h"



void Game::init()
{
	m_camera.Position = glm::vec3(0, 15.5f, 100);

	m_skybox = new Rendering::Skybox("res/textures/front.bmp",
		"res/textures/back.bmp",
		"res/textures/top.bmp",
		"res/textures/bottom.bmp",
		"res/textures/left.bmp",
		"res/textures/right.bmp",
		"res/shaders/skyboxVertex.vs",
		"res/shaders/skyboxFragment.fs");

	testBox = Rendering::Box(glm::vec3(1000, 1, 1000), glm::vec3(0, -23, 0));
	testBox.loadContent(content);

	testPlayer = new Player();
	testPlayer->loadContent(content);
	testPlayer2 = new Player();
	testPlayer2->loadContent(content);
	testPlayer2->setPosition(glm::vec3(100, 0, 0));

	rt3d::materialStruct material = {
		{ 0.4f, 0.4f, 1.0f, 0.2f }, // ambient
		{ 0.8f, 0.8f, 1.0f, 0.2f }, // diffuse
		{ 0.8f, 0.8f, 0.8f, 0.2f }, // specular
		0.2f  // shininess
	};
	glm::vec3 size(50, 100, 50);
	testBox1 = Rendering::Box(size, aabb1.c);
	testBox1.setMaterial(material);
	testBox2 = Rendering::Box(size, aabb2.c);
	testBox2.setMaterial(material);
	testBox1.loadContent(content);
	testBox2.loadContent(content);
}

bool TestAABBAABB(const AABB& A, const AABB& B)
{
	if (abs(A.c[0] - B.c[0]) > (A.r[0] + B.r[0])) return 0;
	if (abs(A.c[1] - B.c[1]) > (A.r[1] + B.r[1])) return 0;
	if (abs(A.c[2] - B.c[2]) > (A.r[2] + B.r[2])) return 0;
	return true;
}


void testAxis(glm::vec3 axis, float minA, float maxA, float minB, float maxB, glm::vec3& mtvAxis, float& mtvDistance)
{
	float axisLengthSquared = glm::dot(axis, axis);
	//Calculates overlap ranges and find out if the overlap is on the left or right
	float d0 = (maxB - minA);
	float d1 = (maxA - minB);
	float overlap = (d0 < d1) ? d0 : -d1;

	glm::vec3 sep = axis * (overlap / axisLengthSquared);

	float sepLengthSquared = glm::dot(sep, sep);
	if (sepLengthSquared < mtvDistance)
	{
		mtvDistance = sepLengthSquared;
		mtvAxis = sep;
	}
}

glm::vec3 getMTV(const AABB& A, const AABB& B)
{
	glm::vec3 mtv = glm::vec3(0);
	float mtvDistance = std::numeric_limits<float>::max();
	glm::vec3 minA(A.c - A.r);
	glm::vec3 maxA(A.c + A.r);
	glm::vec3 minB(B.c - B.r);
	glm::vec3 maxB(B.c + B.r);

	//Test the X, Y and Z axis to find the MTV
	testAxis(glm::vec3(1, 0, 0), minA.x, maxA.x, minB.x, maxB.x, mtv, mtvDistance);
	testAxis(glm::vec3(0, 1, 0), minA.y, maxA.y, minB.y, maxB.y, mtv, mtvDistance);
	testAxis(glm::vec3(0, 0, 1), minA.z, maxA.z, minB.z, maxB.z, mtv, mtvDistance);
	if (mtvDistance > 0)
	{
		mtv = glm::normalize(mtv);
		mtvDistance = (float)sqrt(mtvDistance) * 1.001f;
		mtv *= mtvDistance;
	}
	
	return mtv;
}



void Game::draw()
{
	renderer.begin(m_camera);
	renderer.drawSkybox(m_skybox);
	renderer.setShader("Phong");
	renderer.renderFirstPerson(&testPlayer->getVPWeapon());
	renderList.emplace_back(&testBox1);
	renderList.emplace_back(&testBox2);
	//renderList.emplace_back(&testBox);
	renderer.render(renderList);
	renderer.end();
	renderList.clear();
}

void Game::update()
{
	testBox.update();
	testPlayer->update(0.1f);
	testPlayer2->update(0.1f);
	m_camera.Update(1 / 60.0f);

	auto currentKeyStates = SDL_GetKeyboardState(nullptr);
	if (currentKeyStates[SDL_SCANCODE_W])
	{
		m_camera.Position.z -= 1.0f;
	}
	else if (currentKeyStates[SDL_SCANCODE_A])
	{
		m_camera.Position.x -= 1.0f;
	}
	else if (currentKeyStates[SDL_SCANCODE_D])
	{
		m_camera.Position.x += 1.0f;
	}
	else if (currentKeyStates[SDL_SCANCODE_S])
	{
		m_camera.Position.z += 1.0f;
	}
	else if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		aabb1.c.x -= 5.0f;
		testBox1.getTransform().position = aabb1.c;
	}
	else if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		aabb1.c.x += 5.0f;
		testBox1.getTransform().position = aabb1.c;
	}
	else if (currentKeyStates[SDL_SCANCODE_UP])
	{
		aabb1.c.z -= 5.0f;
		testBox1.getTransform().position = aabb1.c;
	}
	else if (currentKeyStates[SDL_SCANCODE_DOWN])
	{
		aabb1.c.z += 5.0f;
		testBox1.getTransform().position = aabb1.c;
	}
	bool result = TestAABBAABB(aabb1, aabb2);
	if (result == true)
	{
		aabb1.c += getMTV(aabb1, aabb2);
		testBox1.getTransform().position = aabb1.c;
	}
}

Game::Game()
{


	init();
	bool running = true; // set running to true
	SDL_Event sdlEvent; // variable to detect SDL events
	while (running)
	{ // the event loop
		while (SDL_PollEvent(&sdlEvent))
		{
			if (sdlEvent.type == SDL_QUIT)
				running = false;
		}
		update();
		draw(); // call the draw function
	}

	renderer.quit();
}