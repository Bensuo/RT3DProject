#pragma once
#include <GL\glew.h>
#include "rt3d.h"
#include "Transform.h"
class IRenderable
{
public:

	virtual GLuint& getMesh() = 0;
	virtual GLuint& getTexture() = 0;
	virtual GLuint& getCount() = 0;
	virtual rt3d::materialStruct& getMaterial() = 0;
	virtual Transform& getTransform() = 0;
	virtual bool isIndexed() = 0;
};

