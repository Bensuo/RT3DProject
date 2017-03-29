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
		UI(const std::string& string, int size);
		virtual ~UI();
		int getWidth() const;
		int getHeight() const;
		void setString(const std::string& string);
		Shader& getShader();
		GLuint& getMesh() override;
		GLuint& getTexture() override;
		GLuint& getCount() override;
		rt3d::materialStruct& getMaterial() override;
		Transform& getTransform() override;
		bool isIndexed() override;
		void setText(const std::string& txt) { uiString = txt; }
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
		void genTexture();
		void clean(GLuint textID) const;
		Transform transform{ glm::vec3(0), glm::vec3(0) };
		std::string objPath = "res/obj/cube.obj";
	};
}

