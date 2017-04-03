#include "Model.h"
#include <glm/gtc/matrix_transform.inl>

namespace Rendering
{
	Model::~Model() {}

	void Model::loadContent(Utilities::ResourceManager &content, const std::string& meshPath, const std::string& texturePath)
	{
		auto meshFile = meshPath + ".md2";
		auto textureFile = texturePath + ".bmp";
		mesh = model.readMD2Model(meshFile.c_str());
		texture = content.loadTexture(textureFile.c_str());
		vertexCount = model.getVertDataCount();	
	}

	void Model::update(const float& dt)
	{
		model.animate(dt * ANIMATION_SPEED);
		rt3d::updateMesh(mesh, RT3D_VERTEX, model.getCurrentVerts(), model.getVertDataSize());
		rt3d::updateMesh(mesh, RT3D_VERTEX_NEXT, model.getNextVerts(), model.getVertDataSize());
	}

	void Model::setTransform(const Transform & transform)
	{
		this->transform.position.x = transform.position.x;
		this->transform.position.y = transform.position.y;
		this->transform.position.z = transform.position.z;

		this->transform.rotation = transform.rotation;
		this->transform.rotation.x -= 90.0f;
	}

	const GLuint& Model::getMesh() const
	{
		return mesh;
	}

	const GLuint& Model::getCount() const
	{
		return vertexCount;
	}

	const rt3d::materialStruct& Model::getMaterial() const
	{
		return material;
	}

	const Transform& Model::getTransform() const
	{
		return transform;
	}

	bool Model::isIndexed() const
	{
		return false;
	}

	const GLuint& Model::getTexture() const
	{
		return *texture.get();
	}
}
