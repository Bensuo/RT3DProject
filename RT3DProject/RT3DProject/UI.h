#pragma once
#include "IRenderable.h"
#include <glm/glm.hpp>
#include "rt3d.h"
#include <SDL.h>
#include "rt3dObjLoader.h"
#include "Shader.h"

namespace Rendering 
{
	class UI : public IRenderable
	{
	public:
		UI(const std::string& path);
		UI(const std::string& string, int size, const glm::vec4& color = glm::vec4(1.0f));
		virtual ~UI();
		const int& getWidth() const;
		const int& getHeight() const;
		void setString(const std::string& string);
		const Shader& getShader();
		const GLuint& getMesh() override;
		const GLuint& getTexture() override;
		const GLuint& getCount() override;
		const rt3d::materialStruct& getMaterial() override;
		const Transform& getTransform() override;
		const bool& isIndexed() override;
		void setText(const std::string& txt) { uiString = txt; }
		const float& getInterp() override { return 0.0f; }
	private:
		Shader shader;
		std::vector<GLfloat> verts;
		std::vector<GLfloat> norms;
		std::vector<GLfloat> tex_coords;
		std::vector<GLuint> indices;
		std::string uiString;
		GLuint label;
		GLuint mesh;
		GLuint size;
		SDL_Surface* texture;
		rt3d::materialStruct material = {
			{ 1.0f, 1.0f, 1.0f, 1.0f }, // ambient
			{ 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse
			{ 1.0f, 1.0f, 1.0f, 1.0f }, // specular
			1.0f  // shininess
		};
		glm::vec4 color;
		void genTexture();
		void clean(GLuint textID) const;
		Transform transform{ glm::vec3(0), glm::vec3(0) };
		std::string objPath = "res/obj/cube.obj";
	};
}

