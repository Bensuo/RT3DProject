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

glm::vec3 getMTV(const AABB& A, const AABB& B)
{
	float xp, yp, zp;
	glm::vec3 mtv(0);
	//X axis
	xp = abs((A.c.x + A.r.x) - (B.c.x - B.r.x));
	//Y axis
	yp = abs((A.c.y + A.r.y) - (B.c.y - B.r.y));
	//Z axis
	zp = abs((A.c.z + A.r.z) - (B.c.z - B.r.z));
	if (xp < yp && xp < zp)
	{
		if (A.c.x < B.c.x)
		{
			mtv += glm::vec3(-xp, 0, 0);
		}
		else
		{
			mtv += glm::vec3(xp, 0, 0);
		}
	}
	else if (yp < zp && yp < xp)
	{
		if (A.c.y < B.c.y)
		{
			mtv += glm::vec3(0, -yp, 0);
		}
		else
		{
			mtv += glm::vec3(0, yp, 0);
		}
	}
	else if (zp < yp && zp < xp)
	{
		if (A.c.z < B.c.z)
		{
			mtv += glm::vec3(0, 0, -zp);
		}
		else
		{
			mtv += glm::vec3(0, 0, zp);
		}
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
		aabb2.c.x -= 1.0f;
		testBox2.getTransform().position = aabb2.c;
	}
	bool result = TestAABBAABB(aabb1, aabb2);
	if (result == true)
	{
		aabb2.c += getMTV(aabb1, aabb2);
		testBox2.getTransform().position = aabb2.c;
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