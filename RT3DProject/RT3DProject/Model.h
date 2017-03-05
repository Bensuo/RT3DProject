#pragma once
#include "ResourceManager.h"
#include <glm/glm.hpp>
#include "rt3d.h"
#include <stack>
#include "MD2.h"

#define DEG_TO_RADIAN 0.017453293

namespace Rendering
{
	class Model
	{
	public:
		virtual ~Model();
		void loadContent(Utilities::ResourceManager &content, const std::string& path);
		void update(const float& dt);
		void draw(std::stack<glm::mat4>& mvStack, const GLuint& shaderProgram) const;
		void setPosition(const glm::vec3& position);
		void setAnimation(const int& state);
		virtual void SetUpAnimationData() = 0;
	protected:
		GLuint vertexCount;
		MD2 m_MD2;
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