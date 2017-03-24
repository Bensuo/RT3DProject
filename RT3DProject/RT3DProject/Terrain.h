#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "ResourceManager.h"

class Terrain
{
public:
	Terrain();
	virtual ~Terrain();
	void loadContent(const char* fname, const char * normname, Utilities::ResourceManager& content);
	int getNumIndices() { return indexCount; }
	GLuint getVAO() { return vao; }
	Rendering::Shader& getShader() { return shader; }
	glm::vec3 getScale() { return scale; }
	void setScale(glm::vec3 s) { scale = s; }
	int getRows() { return rows; }
	int getColumns() { return cols; }
	GLuint* getTexture() { 
		return texture.lock().get();
	}
	Rendering::Shader shader;
private:
	int rows;
	int cols;
	int indexCount;
	glm::vec3 scale;
	GLuint vertVBO, normVBO, texVBO, indexEBO;
	//OpenGL buffers
	//GLuint vertVBO, texCoordVBO, indicesVBO, normalsVBO;
	GLuint vao;
	
	std::weak_ptr<GLuint> texture;
	rt3d::materialStruct material = {
		{ 0.4f, 0.4f, 1.0f, 1.0f }, // ambient
		{ 0.8f, 0.8f, 1.0f, 1.0f }, // diffuse
		{ 0.8f, 0.8f, 0.8f, 1.0f }, // specular
		1.0f  // shininess
	};
};

