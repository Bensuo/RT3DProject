#pragma once
#include "IRenderable.h"
#include <glm/glm.hpp>
#include "rt3d.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "rt3dObjLoader.h"
#include <string>
#include <SDL_image.h>
namespace Rendering 
{
	class UI : public IRenderable
	{
	public:
		UI(std::string object, bool isText);
		virtual ~UI();
		// Inherited via IRenderable
		virtual GLuint & getMesh() override;
		virtual GLuint & getTexture() override;
		virtual GLuint & getCount() override;
		virtual rt3d::materialStruct & getMaterial() override;
		virtual Transform & getTransform() override;
		virtual bool isIndexed() override;
		void setText(std::string txt) { uiString = txt; }
	private:
		std::vector<GLfloat> verts;
		std::vector<GLfloat> norms;
		std::vector<GLfloat> tex_coords;
		std::vector<GLuint> indices;
		std::string uiString;
		GLuint health = 0;
		GLuint label;
		GLuint mesh[2];
		GLuint size;
		bool isText = false;
		SDL_Surface* texture;
		rt3d::materialStruct material = {
			{ 1.0f, 1.0f, 1.0f, 1.0f }, // ambient
			{ 1.0f, 1.0f, 1.0f, 1.0f }, // diffuse
			{ 1.0f, 1.0f, 1.0f, 1.0f }, // specular
			1.0f  // shininess
		};
		GLuint imageTexture(SDL_Surface* tex);
		GLuint textToTexture(const char * str, GLuint textID);
		void clearTextTexture(GLuint textID);
		TTF_Font * textFont;
		Transform transform{ glm::vec3(0), glm::vec3(0) };
		std::string objPath = "res/obj/cube.obj";
	};
}

