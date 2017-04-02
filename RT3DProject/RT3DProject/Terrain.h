#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "ResourceManager.h"
#include <vector>
#include "Transform.h"

class Terrain
{
public:
	Terrain();
	virtual ~Terrain();
	void loadContent(const char* fname, const char* normname, Utilities::ResourceManager& content);
	const int& getNumIndices() const { return indexCount; }
	const GLuint& getVAO() const { return vao; }
	const Rendering::Shader& getShader() const { return shader; }
	const glm::vec3& getScale() const { return scale; }
	void setScale(const glm::vec3& scale) { this->scale = scale; }
	const int& getRows() const { return rows; }
	const int& getColumns() const { return cols; }
	const GLuint& getTexture(const int& index) const
	{ 
		return texture[index];
	}

	const float& getHeightAtPosition(const float& x, const float& z) const;
	Rendering::Shader shader;
private:
	float getBarycentricHeight(glm::vec2 pos, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) const;
	int rows;
	int cols;
	int indexCount;
	glm::vec3 scale;
	GLuint vertVBO, normVBO, texVBO, indexEBO;
	GLuint vao;
	std::vector<std::vector<float>> heights;
	GLuint texture[5];
	rt3d::materialStruct material = {
		{ 0.4f, 0.4f, 1.0f, 1.0f }, // ambient
		{ 0.8f, 0.8f, 1.0f, 1.0f }, // diffuse
		{ 0.8f, 0.8f, 0.8f, 1.0f }, // specular
		1.0f  // shininess
	};
	Transform transform{ glm::vec3(0), glm::vec3(0) };
};

