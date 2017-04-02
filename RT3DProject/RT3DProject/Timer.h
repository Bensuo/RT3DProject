#pragma once
#include <chrono>
#include <string>

class Timer
{
	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::duration<double> elapsed;
	int duration;

public:

	explicit Timer(int duration) : duration(duration) {}

	void startTimer()
	{
		start = std::chrono::system_clock::now();
	}

	void update()
	{
		if (elapsed.count() < duration)
		{
			end = std::chrono::system_clock::now();
			elapsed = end - start;
		}
	}

	bool finished() const
	{
		return elapsed.count() >= duration;
	}

	std::string toString() const
	{
		std::string result;

		int minutes = (duration - elapsed.count()) / 60.0f;
		int seconds = (duration - elapsed.count()) - 60 * minutes;

		if (minutes < 10) {
			result += "0" + std::to_string(minutes) + ":";
		}
		else {
			result += std::to_string(minutes) + ":";
		}

		if (seconds < 10) {
			result += "0" + std::to_string(seconds);
		}
		else {
			result += std::to_string(seconds);
		}

		return result;
	}
};
