#include "Actor.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/type_ptr.hpp>

namespace Rendering
{
	Actor::Actor()
	{
		position = glm::vec3(0.0, 0.0, 20.0);
	}

	Actor::~Actor()
	{
	}

	void Actor::loadContent(Utilities::ResourceManager &content)
	{
		mesh = model.ReadMD2Model("test.md2");
		texture = content.loadTexture("test.bmp");
		vertexCount = model.getVertDataCount();
	}

	void Actor::update(const float& dt)
	{
		model.Animate(dt);
		rt3d::updateMesh(mesh, RT3D_VERTEX, model.getAnimVerts(), model.getVertDataSize());
	}

	void Actor::draw(std::stack<glm::mat4> &mvStack, const GLuint& shaderProgram)
	{
		glCullFace(GL_FRONT);
		glBindTexture(GL_TEXTURE_2D, *texture.get());

		mvStack.push(mvStack.top());
		float rotation = -90.0f * DEG_TO_RADIAN;

		mvStack.push(glm::rotate(mvStack.top(), rotation, glm::vec3(1.0f, 0.0f, 0.0f)));
		mvStack.top() = glm::translate(mvStack.top(), glm::vec3(position.x, position.z, position.y));

		rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
		rt3d::setMaterial(shaderProgram, material);
		rt3d::drawMesh(mesh, vertexCount, GL_TRIANGLES);
		mvStack.pop();
		mvStack.pop();

		glCullFace(GL_BACK);
	}
}