#pragma once
#include "rt3d.h"
#include <glm/glm.hpp>
#include <memory>
#include "ResourceManager.h"
#include "IRenderable.h"

namespace Rendering
{
	class Box : public IRenderable
	{
	public:
		Box(): width(0), height(0), depth(0), mesh(0) {}
		Box(const glm::vec3& bounds, const glm::vec3& position);
		void loadContent(Utilities::ResourceManager& content);
		virtual ~Box();
		// Inherited via IRenderable
		const GLuint& getMesh() const override;
		const GLuint& getTexture() const override;
		const GLuint& getCount() const override;
		const rt3d::materialStruct& getMaterial() const override;
		const Transform& getTransform() const override;
		bool isIndexed() const override;
		void setMaterial(const rt3d::materialStruct& mat) { material = mat; };
		float getInterp() const override { return 0.0f; }
	private:
		GLfloat cubeVerts[24];
		GLfloat cubeColours[24];
		GLuint cubeVertCount = 8;
		GLuint cubeIndexCount = 36;
		GLuint cubeIndices[36];
		int width, height, depth;
		GLuint mesh;

		std::shared_ptr<GLuint> texture;
		rt3d::materialStruct material = {
			{ 0.4f, 0.4f, 1.0f, 1.0f }, // ambient
			{ 0.8f, 0.8f, 1.0f, 1.0f }, // diffuse
			{ 0.8f, 0.8f, 0.8f, 1.0f }, // specular
			1.0f  // shininess
		};
		Transform transform{ glm::vec3(0), glm::vec3(0) };
	};
}

