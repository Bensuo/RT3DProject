#include "Actor.h"



Actor::Actor()
{
	modelView = glm::mat4(1.0);
	position = glm::vec3(0.0);
}


Actor::~Actor()
{
}

void Actor::loadContent(ResourceManager &content)
{
	mesh = model.ReadMD2Model("yoshi.md2");
	texture = content.loadTexture("yoshi.bmp");
	md2VertCount = model.getVertDataCount();
}

void Actor::update(float dt)
{
	model.Animate(dt);
	rt3d::updateMesh(mesh, RT3D_VERTEX, model.getAnimVerts(), model.getVertDataSize());
}

void Actor::draw(std::stack<glm::mat4> &mvStack, glm::mat4 projection, GLuint shaderProgram)
{
	glCullFace(GL_FRONT);
	glBindTexture(GL_TEXTURE_2D, *texture.get());
	mvStack.push(mvStack.top());
	mvStack.top() = mvStack.top() * modelView;
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(0.0f,-50.0f, 0.0f));
	
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::setMaterial(shaderProgram, material);
	rt3d::drawMesh(mesh, md2VertCount, GL_TRIANGLES);
	mvStack.pop();
	glCullFace(GL_BACK);
}
