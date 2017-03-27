#include "UI.h"


namespace Rendering 
{
	UI::UI(std::string object, bool isText)
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
			mesh[0] = rt3d::createMesh(verts.size(), verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
			if (TTF_Init() == -1)
				std::cout << "TTF failed to initialise." << std::endl;

			textFont = TTF_OpenFont("MavenPro-Regular.ttf", 48);
			if (textFont == NULL)
				std::cout << "Failed to open font." << std::endl;
			transform.rotation.y = 90;
		}
		else
		{
			label = 0;
			size = indices.size();
			mesh[0] = rt3d::createMesh(verts.size(), verts.data(), nullptr, norms.data(), tex_coords.data(), size, indices.data());
			texture = IMG_Load(object.c_str());
		}
	}

	UI::~UI(){}

	GLuint & UI::getTexture() {
		if (isText == true)
		{
			return label = (textToTexture(uiString.c_str(), label));//needs to be moved

		}
		else if (isText == false)
		{
			return label = imageTexture(texture);
		}
		
	}
	GLuint UI::imageTexture(SDL_Surface* tex) {//in progress
		GLuint itexture = 0;
		glGenTextures(1, &itexture);
		glBindTexture(GL_TEXTURE_2D, itexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->w, tex->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex->pixels);
		glBindTexture(GL_TEXTURE_2D, NULL);
		return itexture;
	}
	GLuint UI::textToTexture(const char * str, GLuint textID) {//should only be called for text
		GLuint itexture = textID;
		TTF_Font * font = textFont;

		SDL_Surface * stringImage = TTF_RenderText_Blended(font, str, { 255, 255, 255 });

		if (stringImage == NULL) {
			std::cout << "String surface not created." << std::endl;
		}

		if (texture == 0) {
			glGenTextures(1, &itexture);
		}

		glBindTexture(GL_TEXTURE_2D, itexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, stringImage->w, stringImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, stringImage->pixels);
		glBindTexture(GL_TEXTURE_2D, NULL);

		SDL_FreeSurface(stringImage);
		return itexture;
	}

	void UI::clearTextTexture(GLuint textID) {
		if (textID != NULL) {
			glDeleteTextures(1, &textID);
		}
	}

	GLuint & UI::getMesh() {return mesh[0];}
	GLuint & UI::getCount() { return size; }
	rt3d::materialStruct & UI::getMaterial() { return material; }
	Transform & UI::getTransform() { return transform; }
	bool UI::isIndexed() { return true; }
}

