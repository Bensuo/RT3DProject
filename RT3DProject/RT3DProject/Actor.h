#pragma once
#include "md2model.h"
#include "ResourceManager.h"
#include <glm/glm.hpp>
#include "rt3d.h"
#include <stack>
class Actor
{
public:
	Actor();
	virtual ~Actor();
	virtual void loadContent(ResourceManager &content);
	virtual void update(float dt);
	virtual void draw(std::stack<glm::mat4> &mvStack, glm::mat4 projection, GLuint shaderProgram);
	glm::mat4 getModelView() { return modelView; };
private:
	GLuint md2VertCount;
	md2model model;
	GLuint mesh;
	std::shared_ptr<GLuint> texture;
	glm::vec3 position;
	glm::mat4 modelView;
	rt3d::materialStruct material = {
		{ 0.4f, 0.4f, 1.0f, 1.0f }, // ambient
		{ 0.8f, 0.8f, 1.0f, 1.0f }, // diffuse
		{ 0.8f, 0.8f, 0.8f, 1.0f }, // specular
		1.0f  // shininess
	};
};

