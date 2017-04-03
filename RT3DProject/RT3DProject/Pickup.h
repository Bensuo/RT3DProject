#pragma once
#include "IRenderable.h"
#include "ResourceManager.h"
#include "AABB.h"
#include "Model.h"

class Pickup
{
public:
	Pickup();
	virtual ~Pickup();
	void loadContent(Utilities::ResourceManager content, const std::string& skin);
	void update(const float& dt);
	Rendering::Model& getPlayerModel();
	void setPosition(const glm::vec3& pos);
	const glm::vec3& getPosition() const;
	const AABB& getAABB() const;
private:
	float rotationSpeed = 30;

	Rendering::Model model;

	Transform transform
	{
		glm::vec3(0), glm::vec3(0)
	};

	AABB collider
	{
		glm::vec3(0), glm::vec3(10, 10, 10)
	};
};

