#pragma once
#include "PlayerModel.h"
#include "AABB.h"

class StaticObject
{
public:
	StaticObject();
	virtual ~StaticObject();
	void loadContent(Utilities::ResourceManager &content, const std::string& meshPath, const std::string& texturePath);
	Rendering::PlayerModel* getModel() const { return model; }
	const AABB& getAABB() const { return boundingBox; }
	void setTransform(const Transform& t) {
		boundingBox.c = t.position; 
		boundingBox.c.y += boundingBox.r.y;
		model->setTransform(t);
	}
private:
	Rendering::PlayerModel* model;
	AABB boundingBox{ glm::vec3(0), glm::vec3(0) };
};

