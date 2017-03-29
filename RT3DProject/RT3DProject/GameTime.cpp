#include <thread>
#include <iostream>
#include "GameTime.h"

GameTime::GameTime()
{
	startTime = std::chrono::high_resolution_clock::now();

	frames = 0;
	frameCap = MAX_FRAMERATE;
	frameCounter = std::chrono::duration<float>(0);
	deltaTime = std::chrono::duration<float>(1.0 / frameCap);
	unprocessedTime = std::chrono::duration<float>(0);
}

double GameTime::GetTime() const
{
	//find current time in miliseconds, return this data in terms of seconds
	auto current_time = std::chrono::high_resolution_clock::now();
	auto value = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - startTime).count();
	return value / 1000.0;
}

std::chrono::steady_clock::time_point GameTime::GetTimePoint()
{
	return std::chrono::steady_clock::now();
}

const double& GameTime::GetDeltaTime() const
{
	return deltaTime.count();
}

void GameTime::SetFrameCap(const unsigned& frameCap)
{
	this->frameCap = frameCap;
}

void GameTime::Initialize()
{
	lastTime = GetTimePoint();
}

void GameTime::Reset()
{
	frameComplete = false;

	//calculate current frame duration
	auto startTime = GetTimePoint();
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

bool GameTime::ProcessFrame() const
{
	return unprocessedTime > deltaTime;
}

bool GameTime::FrameComplete() const
{
	return frameComplete;
}

void GameTime::Update()
{
	frameComplete = true;
	unprocessedTime -= deltaTime;
}

void GameTime::IncrementFrames()
{
	frames++;
}

void GameTime::Sleep()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
