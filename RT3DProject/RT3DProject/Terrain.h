#pragma once
#include <gl/glew.h>
class Terrain
{
public:
	Terrain();
	virtual ~Terrain();
	void loadContent(const char* fname);
private:
	GLfloat* vertData;
};

