#include "Model.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/type_ptr.hpp>

namespace Rendering
{
	Model::~Model() {}

	void Model::loadContent(Utilities::ResourceManager &content, const std::string& path)
	{
		auto meshPath = path + ".md2";
		auto texturePath = path + ".bmp";
		mesh = m_MD2.ReadMD2Model(meshPath.c_str());
		texture = content.loadTexture(texturePath.c_str());
		vertexCount = m_MD2.getVertDataCount();
	}

	void Model::update(const float& dt)
	{
		m_MD2.Animate(dt);
		rt3d::updateMesh(mesh, RT3D_VERTEX, m_MD2.getAnimVerts(), m_MD2.getVertDataSize());
	}

	void Model::draw(std::stack<glm::mat4> &mvStack, const GLuint& shaderProgram) const
	{
		glCullFace(GL_FRONT);
		glBindTexture(GL_TEXTURE_2D, *texture.get());

		mvStack.push(mvStack.top());
		float rotation = -90.0f * DEG_TO_RADIAN;
		mvStack.push(rotate(mvStack.top(), rotation, glm::vec3(1.0f, 0.0f, 0.0f)));
		
		mvStack.top() = translate(mvStack.top(), glm::vec3(position.x, position.y, position.z));

		rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
		rt3d::setMaterial(shaderProgram, material);
		rt3d::drawMesh(mesh, vertexCount, GL_TRIANGLES);
		mvStack.pop();
		mvStack.pop();

		glCullFace(GL_BACK);
	}

	void Model::setPosition(const glm::vec3 & position)
	{
		this->position = position;
	}

	void Model::setAnimation(const int& state)
	{
		m_MD2.setCurrentAnim(state);
	}
}
