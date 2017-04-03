#include <thread>
#include <iostream>
#include "GameTime.h"
#include <chrono>

GameTime::GameTime()
{
	startTime = std::chrono::high_resolution_clock::now();
	isFrameComplete = false;
	frames = 0;
	frameCap = MAX_FRAMERATE;
	frameCounter = std::chrono::duration<float>(0);
	deltaTime = std::chrono::duration<float>(1.0 / frameCap);
	unprocessedTime = std::chrono::duration<float>(0);
}

const double& GameTime::getTime() const
{
	//find current time in miliseconds, return this data in terms of seconds
	auto current_time = std::chrono::high_resolution_clock::now();
	auto value = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - startTime).count();
	return value / 1000.0;
}

const std::chrono::steady_clock::time_point& GameTime::getTimePoint()
{
	return std::chrono::steady_clock::now();
}

const double& GameTime::getDeltaTime() const
{
	return deltaTime.count();
}

void GameTime::setFrameCap(const unsigned& frameCap)
{
	this->frameCap = frameCap;
}

void GameTime::initialize()
{
	lastTime = getTimePoint();
}

void GameTime::reset()
{
	isFrameComplete = false;

	//calculate current frame duration
	auto startTime = getTimePoint();
	auto passedTime = startTime - lastTime;
	lastTime = startTime;
	unprocessedTime += passedTime;
	frameCounter += passedTime;

#if _DEBUG
	//display FPS m_Counter
	if (frameCounter >= std::chrono::duration<double>(1))
	{
		std::cout << frames << "\n";
		frames = 0;
		frameCounter = std::chrono::duration<double>(0);
	}
#endif
}

const bool& GameTime::processFrame() const
{
	return unprocessedTime > deltaTime;
}

const bool& GameTime::frameComplete() const
{
	return isFrameComplete;
}

void GameTime::update()
{
	isFrameComplete = true;
	unprocessedTime -= deltaTime;
}

void GameTime::incrementFrames()
{
	frames++;
}

void GameTime::sleep()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
