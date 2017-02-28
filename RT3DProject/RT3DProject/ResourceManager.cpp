#include "ResourceManager.h"



ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}

std::shared_ptr<GLuint> ResourceManager::loadTexture(std::string filename)
{
	if (textureMap.find(filename) == textureMap.end())
	{
		std::shared_ptr<GLuint> texID(new GLuint());
		glGenTextures(1, texID.get());
		
		SDL_Surface *tmpSurface;
		tmpSurface = SDL_LoadBMP(filename.c_str());
		if (!tmpSurface)
		{
			//Error
		}

		glBindTexture(GL_TEXTURE_2D, *texID.get());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		SDL_PixelFormat *format = tmpSurface->format;

		GLuint externalFormat, internalFormat;
		if (format->Amask)
		{
			internalFormat = GL_RGBA;
			externalFormat = (format->Rmask < format->Bmask) ? GL_RGBA : GL_BGRA;
		}
		else {
			internalFormat = GL_RGB;
			externalFormat = (format->Rmask < format->Bmask) ? GL_RGB : GL_BGR;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, tmpSurface->w, tmpSurface->h, 0, externalFormat, GL_UNSIGNED_BYTE, tmpSurface->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		SDL_FreeSurface(tmpSurface);
		textureMap.insert(std::make_pair(filename, texID));
		
	}
	return textureMap[filename];
}
