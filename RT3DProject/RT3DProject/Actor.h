#pragma once
#include "md2model.h"
#include "ResourceManager.h"
#include <glm/glm.hpp>
#include "rt3d.h"
#include <stack>

#define DEG_TO_RADIAN 0.017453293

namespace Rendering
{
	class Actor
	{
	public:
		Actor();
		virtual ~Actor();
		virtual void loadContent(Utilities::ResourceManager &content);
		virtual void update(const float& dt);
		virtual void draw(std::stack<glm::mat4>& mvStack, const GLuint& shaderProgram);
	private:
		GLuint vertexCount;
		md2model model;
		GLuint mesh;
		std::shared_ptr<GLuint> texture;
		glm::vec3 position;
		rt3d::materialStruct material = {
			{ 0.4f, 0.4f, 1.0f, 1.0f }, // ambient
			{ 0.8f, 0.8f, 1.0f, 1.0f }, // diffuse
			{ 0.8f, 0.8f, 0.8f, 1.0f }, // specular
			1.0f  // shininess
		};
	};
}