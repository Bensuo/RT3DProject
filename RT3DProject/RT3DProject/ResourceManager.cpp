#include "ResourceManager.h"
#include <SDL.h>

namespace Utilities
{
	ResourceManager::ResourceManager()
	{
	}

	ResourceManager::~ResourceManager()
	{
	}

	const std::shared_ptr<GLuint>& ResourceManager::loadTexture(const std::string& filename)
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

	Mix_Music* ResourceManager::loadMusic(const std::string& filename)
	{
		auto iterator = musicMap.find(filename);
		Mix_Music* music = nullptr;
		//if the sound has been loaded, play it
		if (iterator != musicMap.end())
		{
			music = musicMap[filename];
		}
		//if the sound hasn't been loaded, load it and play it
		else
		{
			auto newMusic = Mix_LoadMUS(filename.c_str());
			if (newMusic == nullptr) {
				printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
				printf(filename.c_str(), "\n");
				printf("\n");
			}
			else {
				musicMap.emplace(filename, newMusic);
				music = newMusic;
			}
		}
		return music;
	}

	Mix_Chunk* ResourceManager::loadSound(const std::string& filename)
	{
		auto iterator = soundsMap.find(filename);
		Mix_Chunk* chunk = nullptr;
		//if the sound has been loaded, play it
		if (iterator != soundsMap.end())
		{
			chunk = soundsMap[filename];
		}
		//if the sound hasn't been loaded, load it and play it
		else
		{
			auto newSound = Mix_LoadWAV(filename.c_str());
			if (newSound == nullptr) {
				printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
				printf(filename.c_str(), "\n");
				printf("\n");
			}
			else {
				soundsMap.emplace(filename, newSound);
				chunk = newSound;
			}
		}
		return chunk;
	}

	void ResourceManager::unloadAll()
	{
		//free the music
		for (auto it = musicMap.begin(); it != musicMap.end(); ++it)
		{
			Mix_FreeMusic(it->second);
		}
		musicMap.clear();

		//free the sounds
		for (auto it = soundsMap.begin(); it != soundsMap.end(); ++it)
		{
			Mix_FreeChunk(it->second);
		}
		soundsMap.clear();

		for (auto it = textureMap.begin(); it != textureMap.end(); ++it)
		{
			glDeleteTextures(1, it->second.get());
		}
		textureMap.clear();
	}
}
