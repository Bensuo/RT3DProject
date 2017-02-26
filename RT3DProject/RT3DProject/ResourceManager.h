#pragma once
#include <map>
#include <memory>
#include <GL/glew.h>
#include <SDL.h>
class ResourceManager
{
public:
	ResourceManager();
	virtual ~ResourceManager();
	std::shared_ptr<GLuint> loadTexture(std::string filename);
private:
	std::map<std::string, std::shared_ptr<GLuint>> textureMap;
};

