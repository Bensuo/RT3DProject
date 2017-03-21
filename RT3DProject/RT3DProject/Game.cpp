#include "Game.h"

void Game::init()
{
	SDL_SetRelativeMouseMode(SDL_TRUE);

	camera.Position = glm::vec3(0, 18.0f, 100);

	skybox = new Rendering::Skybox("res/textures/front.bmp",
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
	testPlayer->loadContent(content, "rampage");
	testPlayer->setState(Player::STAND);
	timer.Initialize();	//always init last for accurate game loop startup

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
	renderer.begin(camera);
	renderer.drawSkybox(skybox);
	renderer.setShader("Phong");
	renderList.emplace_back(&testPlayer->getPlayerModel());
	renderList.emplace_back(&testPlayer->getWeapon());
	renderList.emplace_back(&testBox);
	renderer.render(renderList);
	renderer.renderFirstPerson(&testPlayer->getVPWeapon());
	renderer.end();
	renderList.clear();
	auto test = 0;
}

bool Game::Quit() const
{
	return input.Quit();
}

void Game::update()
{
	running = !Quit();
	input.Update(camera);
	testBox.update();
	testPlayer->update(timer.GetDeltaTime());
	camera.Update(timer.GetDeltaTime());
}

Game::Game()
{
	init();
	while (running)
	{
		timer.Reset();

		//process individual frame's worth of updates
		while (timer.ProcessFrame())
		{
			timer.Update();
			update();
		}

		//render processed frame
		if (timer.FrameComplete())
		{
			timer.IncrementFrames();
			draw();
		}
		else
		{
			timer.Sleep();
		}
	}
	renderer.quit();
}