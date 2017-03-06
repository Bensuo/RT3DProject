#pragma once
#include <glm/glm.hpp>
class Transform
{
public:
	Transform();
	virtual ~Transform();
	glm::vec3 position;
	float rotation;

};

