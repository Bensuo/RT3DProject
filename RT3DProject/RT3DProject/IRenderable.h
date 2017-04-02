#pragma once
#include <GL\glew.h>
#include "rt3d.h"
#include "Transform.h"
//An interface for rendering 3D objects using OpenGL and the RT3D libraries
class IRenderable
{
public:
	virtual ~IRenderable() = default;
	virtual const GLuint& getMesh() = 0;
	virtual const GLuint& getTexture() = 0;
	virtual const GLuint& getCount() = 0;
	virtual const rt3d::materialStruct& getMaterial() = 0;
	virtual const Transform& getTransform() = 0;
	virtual const bool& isIndexed() = 0;
	virtual const float& getInterp() = 0;
};

