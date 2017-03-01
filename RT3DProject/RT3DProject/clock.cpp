#include "Clock.h"

namespace Utilities
{
	Clock::Clock()
	{
		m_StartTime = std::chrono::high_resolution_clock::now();
	}

	double Clock::currentTimeSeconds() const
	{
		//find current time in miliseconds, return this data in terms of seconds
		auto current_time = std::chrono::high_resolution_clock::now();
		auto value = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - m_StartTime).count();
		return value / 1000.0;
	}

	std::chrono::steady_clock::time_point Clock::getTime() const
	{
		return std::chrono::steady_clock::now();
	}
}
