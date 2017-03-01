#include "clock.h"

namespace utilities
{
	clock::clock()
	{
		m_StartTime = std::chrono::high_resolution_clock::now();
	}

	double clock::currentTimeSeconds() const
	{
		//find current time in miliseconds, return this data in terms of seconds
		auto current_time = std::chrono::high_resolution_clock::now();
		auto value = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - m_StartTime).count();
		return value / 1000.0;
	}

	std::chrono::steady_clock::time_point clock::getTime() const
	{
		return std::chrono::steady_clock::now();
	}

	const std::chrono::duration<double>& clock::getDelta() const
	{
		return m_Delta;
	}

	void clock::setDelta(std::chrono::duration<double> newDelta)
	{
		m_Delta = newDelta;
	}
}
