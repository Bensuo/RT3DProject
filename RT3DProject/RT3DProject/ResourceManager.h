#pragma once
#include <map>
#include <memory>
#include <GL/glew.h>

namespace Utilities
{
	class ResourceManager
	{
	public:
		ResourceManager();
		virtual ~ResourceManager();
		std::shared_ptr<GLuint> loadTexture(std::string filename);
	private:
		std::map<std::string, std::shared_ptr<GLuint>> textureMap;
	};
}