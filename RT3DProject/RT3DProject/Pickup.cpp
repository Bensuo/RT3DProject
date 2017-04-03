#include "Pickup.h"
#include <glm/detail/type_mat.hpp>

Pickup::Pickup()
{
}

Pickup::~Pickup()
{
}

void Pickup::loadContent(Utilities::ResourceManager content, const std::string& skin)
{
	model.loadContent(content, "res/md2/pickups/" + skin, "res/md2/pickups/" + skin);
}

void Pickup::update(const float& dt)
{
	transform.rotation.z += dt * rotationSpeed;

	model.setTransform(transform);

	collider.c = transform.position;
}

Rendering::Model& Pickup::getPlayerModel()
{
	return model;
};

const glm::vec3& Pickup::getPosition() const
{
	return transform.position;
};

void Pickup::setPosition(const glm::vec3& pos)
{
	transform.position = pos;
};

const AABB& Pickup::getAABB() const
{
	return collider;
}