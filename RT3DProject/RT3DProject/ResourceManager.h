#pragma once
#include <map>
#include <memory>
#include <GL/glew.h>
#include <SDL_mixer.h>

namespace Utilities
{
	class ResourceManager
	{
	public:
		ResourceManager();
		virtual ~ResourceManager();
		std::shared_ptr<GLuint> loadTexture(std::string filename);
		Mix_Music* loadMusic(std::string filename);
		Mix_Chunk* loadSound(std::string filename);
		void unloadAll();
	private:
		std::map<std::string, std::shared_ptr<GLuint>> textureMap;
		std::map<std::string, Mix_Music*> musicMap;
		std::map<std::string, Mix_Chunk*> soundsMap;
	};
}