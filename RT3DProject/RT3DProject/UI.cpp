#include "UI.h"
#include <SDL_ttf.h>
#include <iostream>
#include <SDL_image.h>

namespace Rendering 
{
	UI::UI(const std::string& path) : shader("res/shaders/UI.vert", "res/shaders/UI.frag")
	{
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))//lazyfoo
		{
			std::cout << "image failed to initialise." << std::endl;
		}

		rt3d::loadObj(objPath.c_str(), verts, norms, tex_coords, indices);

		label = 0;
		size = indices.size();
		mesh = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
		texture = IMG_Load(path.c_str());
		genTexture();
	}

	UI::UI(const std::string& string, int textSize, const glm::vec4& color) : shader("res/shaders/UI.vert", "res/shaders/UI.frag"), color(color)
	{
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))//lazyfoo
		{
			std::cout << "image failed to initialise." << std::endl;
		}

		rt3d::loadObj(objPath.c_str(), verts, norms, tex_coords, indices);

		label = 0;
		size = indices.size();
		uiString = string;
		mesh = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
		if (TTF_Init() == -1)
			std::cout << "TTF failed to initialise." << std::endl;

		auto textFont = TTF_OpenFont("res/fonts/MavenPro-Regular.ttf", textSize);
		if (textFont == nullptr)
			std::cout << "Failed to open font." << std::endl;
		SDL_Color sdl_Color = { color.b, color.g, color.r, color.a };
		texture = TTF_RenderText_Blended(textFont, string.c_str(), sdl_Color);
		genTexture();
	}

	UI::~UI(){}

	int UI::getWidth() const
	{
		return texture->w;
	}

	int UI::getHeight() const
	{
		return texture->h;
	}

	void UI::setString(const std::string& string)
	{
		if(string == uiString)
			return;

		this->uiString = string;
		auto textFont = TTF_OpenFont("res/fonts/MavenPro-Regular.ttf", 24);
		if (textFont == nullptr)
			std::cout << "Failed to open font." << std::endl;
		SDL_Color sdl_Color = { color.b, color.g, color.r, color.a };
		texture = TTF_RenderText_Blended(textFont, string.c_str(), sdl_Color);
		genTexture();
	}

	Shader& UI::getShader()
	{
		return shader;
	}

	GLuint & UI::getTexture() {
		return label;
	}

	void UI::genTexture()
	{
		glGenTextures(1, &label);
		glBindTexture(GL_TEXTURE_2D, label);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->w, texture->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->pixels);
		glBindTexture(GL_TEXTURE_2D, NULL);
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

