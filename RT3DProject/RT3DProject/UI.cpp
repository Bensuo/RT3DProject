#include "UI.h"
#include <SDL_ttf.h>
#include <iostream>
#include <SDL_image.h>

namespace Rendering 
{
	UI::UI(const std::string& object, bool isText) : shader("res/shaders/UI.vert", "res/shaders/UI.frag")
	{
		this->isText = isText;
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))//lazyfoo
		{
			std::cout << "image failed to initialise." << std::endl;
		}

		rt3d::loadObj(objPath.c_str(), verts, norms, tex_coords, indices);

		if (isText) {
			label = 0;
			size = indices.size();
			uiString = object;
			mesh = rt3d::createMesh(verts.size(), verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
			if (TTF_Init() == -1)
				std::cout << "TTF failed to initialise." << std::endl;

			auto textFont = TTF_OpenFont("MavenPro-Regular.ttf", 48);
			if (textFont == nullptr)
				std::cout << "Failed to open font." << std::endl;
			SDL_Color sdl_Color = { 255, 255, 255, 255 };
			texture = TTF_RenderText_Blended(textFont, object.c_str(), sdl_Color);
		}
		else
		{
			label = 0;
			size = indices.size();
			mesh = rt3d::createMesh(verts.size(), verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
			texture = IMG_Load(object.c_str());
		}
	}

	UI::~UI(){}

	Shader& UI::getShader()
	{
		return shader;
	}

	GLuint & UI::getTexture() {
		return label = genTexture();
	}

	GLuint UI::genTexture() const
	{
		GLuint itexture = 0;
		glGenTextures(1, &itexture);
		glBindTexture(GL_TEXTURE_2D, itexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
		glBindTexture(GL_TEXTURE_2D, NULL);
		return itexture;
	}

	void UI::clean(GLuint textID) const
	{
		if (textID != NULL) {
			glDeleteTextures(1, &textID);
			SDL_FreeSurface(texture);
		}
	}

	GLuint & UI::getMesh() { return mesh; }
	GLuint & UI::getCount() { return size; }
	rt3d::materialStruct & UI::getMaterial() { return material; }
	Transform & UI::getTransform() { return transform; }
	bool UI::isIndexed() { return true; }
}

