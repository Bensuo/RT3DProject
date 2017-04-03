#pragma once
#include <GL\glew.h>
#include "rt3d.h"
#include "Transform.h"
//An interface for rendering 3D objects using OpenGL and the RT3D libraries
class IRenderable
{
public:
	virtual ~IRenderable() = default;
	virtual const GLuint& getMesh() const = 0;
	virtual const GLuint& getTexture() const = 0;
	virtual const GLuint& getCount() const = 0;
	virtual const rt3d::materialStruct& getMaterial() const = 0;
	virtual const Transform& getTransform() const = 0;
	virtual const bool& isIndexed() const = 0;
	virtual const float& getInterp() const = 0;
};

