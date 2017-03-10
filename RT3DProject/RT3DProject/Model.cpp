#include "Model.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/type_ptr.hpp>

namespace Rendering
{
	Model::~Model() {}

	void Model::loadContent(Utilities::ResourceManager &content, const std::string& meshPath, const std::string& texturePath)
	{
		auto meshFile = meshPath + ".md2";
		auto textureFile = texturePath + ".bmp";
		mesh = m_MD2.ReadMD2Model(meshFile.c_str());
		texture = content.loadTexture(textureFile.c_str());
		vertexCount = m_MD2.getVertDataCount();
	}

	void Model::update(const float& dt)
	{
		m_MD2.Animate(dt * ANIMATION_SPEED);
		rt3d::updateMesh(mesh, RT3D_VERTEX, m_MD2.getAnimVerts(), m_MD2.getVertDataSize());
	}

	void Model::setAnimation(const int& state)
	{
		m_MD2.setCurrentAnim(state);
	}

	void Model::setTransform(const Transform & transform)
	{
		this->transform.position.x = transform.position.x;
		this->transform.position.y = transform.position.z;
		this->transform.position.z = transform.position.y;

		this->transform.rotation = transform.rotation;
		this->transform.rotation.x -= 90.0f;
	}

	GLuint& Model::getMesh()
	{
		return mesh;
	}
	GLuint& Model::getCount()
	{
		return vertexCount;
	}
	rt3d::materialStruct& Model::getMaterial()
	{
		return material;
	}
	Transform& Model::getTransform()
	{
		return transform;
	}
	bool Model::isIndexed()
	{
		return false;
	}
	GLuint& Model::getTexture()
	{
		return *texture.get();
	}
}
