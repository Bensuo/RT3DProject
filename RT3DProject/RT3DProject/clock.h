#ifndef TIME_H
#define TIME_H
#pragma once

#include <chrono>

namespace Utilities
{
	class Clock
	{
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
	public:
		Clock();
		double currentTimeSeconds() const;
		std::chrono::steady_clock::time_point getTime() const;
	};
}
#endif
