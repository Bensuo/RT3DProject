#pragma once
#include "IRenderable.h"
#include <glm/glm.hpp>
#include "rt3d.h"
#include <SDL.h>
#include "rt3dObjLoader.h"
#include <string>

namespace Rendering 
{
	class UI : public IRenderable
	{
	public:
		UI(std::string object, bool isText);
		virtual ~UI();
		GLuint & getMesh() override;
		GLuint & getTexture() override;
		GLuint & getCount() override;
		rt3d::materialStruct & getMaterial() override;
		Transform & getTransform() override;
		bool isIndexed() override;
		void setText(std::string txt) { uiString = txt; }
	private:
		std::vector<GLfloat> verts;
		std::vector<GLfloat> norms;
		std::vector<GLfloat> tex_coords;
		std::vector<GLuint> indices;
		std::string uiString;
		GLuint label;
		GLuint mesh;
		GLuint size;
		bool isText = false;
		SDL_Surface* texture;
		rt3d::materialStruct material = {
			{ 1.0f, 1.0f, 1.0f, 1.0f }, // ambient
			{ 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse
			{ 1.0f, 1.0f, 1.0f, 1.0f }, // specular
			1.0f  // shininess
		};
		GLuint genTexture() const;
		void clean(GLuint textID) const;
		Transform transform{ glm::vec3(0), glm::vec3(0) };
		std::string objPath = "res/obj/cube.obj";
	};
}

