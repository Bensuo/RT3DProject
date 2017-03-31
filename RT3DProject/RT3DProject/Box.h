#pragma once
#include "rt3d.h"
#include <glm/glm.hpp>
#include <stack>
#include <memory>
#include "ResourceManager.h"
#include "IRenderable.h"
namespace Rendering
{
	class Box : public IRenderable
	{
	public:
		Box() {};
		Box(const glm::vec3& bounds, const glm::vec3& position);
		void loadContent(Utilities::ResourceManager& content);
		virtual ~Box();

		void update();

		// Inherited via IRenderable
		virtual GLuint & getMesh() override;
		virtual GLuint & getTexture() override;
		virtual GLuint & getCount() override;
		virtual rt3d::materialStruct & getMaterial() override;
		virtual Transform & getTransform() override;
		virtual bool isIndexed() override;
		void setMaterial(rt3d::materialStruct mat) { material = mat; };
		float getInterp() override { return 0.0f; }
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

