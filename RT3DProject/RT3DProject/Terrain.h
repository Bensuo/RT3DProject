#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
class Terrain
{
public:
	Terrain();
	virtual ~Terrain();
	void loadContent(const char* fname);
private:
	GLfloat* vertData;
	GLfloat* coordsData;
	int rows;
	int cols;
	glm::vec2 scale;
};

