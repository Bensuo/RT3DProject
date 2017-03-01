#ifndef TIME_H
#define TIME_H
#pragma once

#include <chrono>

namespace utilities
{
	class clock
	{
	private:
		std::chrono::duration<double> m_Delta;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
	public:
		clock();
		double currentTimeSeconds() const;
		std::chrono::steady_clock::time_point getTime() const;
		const std::chrono::duration<double>& getDelta() const;
		void setDelta(const std::chrono::duration<double> delta);
	};
}
#endif
