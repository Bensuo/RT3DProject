#ifndef CS_TIME_H
#define CS_TIME_H
#pragma once

#include <chrono>

class Timer
{
private:
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	std::chrono::time_point<std::chrono::steady_clock> lastTime;

	unsigned frames = 0;
	unsigned frameCap;
	const unsigned MAX_FRAMERATE = 60;

	std::chrono::duration<double> frameCounter;
	std::chrono::duration<double> deltaTime;
	std::chrono::duration<double> unprocessedTime;

	bool frameComplete;
public:
	Timer();
	double GetTime() const;
	static std::chrono::steady_clock::time_point GetTimePoint();
	const double& GetDeltaTime() const;

	void SetFrameCap(const unsigned& frameCap);
	void Initialize();
	void Reset();
	bool ProcessFrame() const;
	bool FrameComplete() const;
	void Update();
	void IncrementFrames();
	static void Sleep();
};
#endif
