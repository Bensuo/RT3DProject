#ifndef CS_TIME_H
#define CS_TIME_H
#pragma once

#include <chrono>

class GameTime
{
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	std::chrono::time_point<std::chrono::steady_clock> lastTime;

	unsigned frames = 0;
	unsigned frameCap;
	const unsigned MAX_FRAMERATE = 60;

	std::chrono::duration<double> frameCounter;
	std::chrono::duration<double> deltaTime;
	std::chrono::duration<double> unprocessedTime;

	bool isFrameComplete;
public:
	GameTime();
	const double& getTime() const;
	const std::chrono::steady_clock::time_point& getTimePoint();
	const double& getDeltaTime() const;
	void setFrameCap(const unsigned& frameCap);
	void initialize();
	void reset();
	const bool& processFrame() const;
	const bool& frameComplete() const;
	void update();
	void incrementFrames();
	void sleep();
};
#endif
