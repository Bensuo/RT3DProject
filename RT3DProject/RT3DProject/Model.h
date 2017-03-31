#pragma once
#include "ResourceManager.h"
#include <glm/glm.hpp>
#include "rt3d.h"
#include <stack>
#include "MD2.h"
#include "IRenderable.h"

#define DEG_TO_RADIAN 0.017453293f

namespace Rendering
{
	class Model : public IRenderable
	{
	public:
		virtual ~Model();
		void loadContent(Utilities::ResourceManager &content, const std::string& meshPath, const std::string& texturePath);
		void update(const float& dt);	
		int getCurrentFrame() const { return m_MD2.getCurrentFrame(); }
		virtual void setAnimation(const int& state) {}
		virtual void SetUpAnimationData() {}
		virtual void setTransform(const Transform& transform);
		GLuint& getMesh() override;
		GLuint& getTexture() override;
		GLuint& getCount() override;
		rt3d::materialStruct& getMaterial() override;
		Transform& getTransform() override;
		bool isIndexed() override;
		float getInterp() override { return m_MD2.getInterp(); }
	protected:
		const float ANIMATION_SPEED = 5.0f; //multiples of deltaTime
		std::shared_ptr<GLuint> texture;
		rt3d::materialStruct material = {
			{ 0.4f, 0.4f, 1.0f, 1.0f }, // ambient
			{ 0.8f, 0.8f, 1.0f, 1.0f }, // diffuse
			{ 0.8f, 0.8f, 0.8f, 1.0f }, // specular
			1.0f  // shininess
		};
		GLuint vertexCount;
		MD2 m_MD2;
		GLuint mesh;
		Transform transform{ glm::vec3(0), glm::vec3(-90.0f, 0, 0) };
	};
}