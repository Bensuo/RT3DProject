#pragma once
#include <glm\glm.hpp>

namespace Collisions
{
	struct Ray
	{
		glm::vec3 dir;
		glm::vec3 origin;
	};
}