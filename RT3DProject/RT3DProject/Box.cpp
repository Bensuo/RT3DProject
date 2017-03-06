#include "Box.h"
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/type_ptr.hpp>

namespace Rendering
{
	Box::Box(const glm::vec3& bounds, const glm::vec3& position) : cubeVerts{ -0.5, -0.5f, -0.5f,
																			  -0.5,  0.5f, -0.5f,
																			   0.5,  0.5f, -0.5f,
																			   0.5, -0.5f, -0.5f,
																			  -0.5, -0.5f,  0.5f,
																			  -0.5,  0.5f,  0.5f,
																			   0.5,  0.5f,  0.5f,
																			   0.5, -0.5f,  0.5f },
		cubeIndices{ 0,1,2, 0,2,3, // back  
					 1,0,5, 0,4,5, // left					
					 6,3,2, 3,6,7, // right
					 1,5,6, 1,6,2, // top
					 0,3,4, 3,7,4, // bottom
					 6,5,4, 7,6,4 },
		width(bounds.x), height(bounds.y), depth(bounds.z)
	{
		for (int i = 0; i < cubeVertCount * 3; i += 3)
		{
			cubeVerts[i + 0] *= bounds.x;
			cubeVerts[i + 1] *= bounds.y;
			cubeVerts[i + 2] *= bounds.z;
		}
		mesh = rt3d::createMesh(cubeVertCount, cubeVerts, nullptr, cubeVerts, nullptr, cubeIndexCount, cubeIndices);
		this->transform.position = position;
	}

	Box::~Box()
	{
	}

	void Box::update()
	{
		//position.y -= 0.005f;
	}

	GLuint & Box::getMesh()
	{
		return mesh;
	}

	GLuint & Box::getTexture()
	{
		return *texture.get();
	}

	GLuint & Box::getCount()
	{
		return cubeIndexCount;
	}

	rt3d::materialStruct & Box::getMaterial()
	{
		return material;
	}

	Transform & Box::getTransform()
	{
		return transform;
	}

	bool Box::isIndexed()
	{
		return true;
	}

	void Box::loadContent(Utilities::ResourceManager& content)
	{
		texture = content.loadTexture("res/md2/rampage.bmp");
	}


}