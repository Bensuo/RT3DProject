#include "StaticObject.h"



StaticObject::StaticObject()
{
}


StaticObject::~StaticObject()
{
}

void StaticObject::loadContent(Utilities::ResourceManager & content, const std::string & meshPath, const std::string & texturePath)
{
	model = new Rendering::PlayerModel();
	model->loadContent(content, meshPath, texturePath);
	float minX = 0, minY = 0, maxX = 0, maxY = 0, minZ = 0, maxZ = 0;
	GLfloat* verts = model->getVerts();
	int count = model->getNumVerts();
	for (int i = 0; i < count; i++)
	{
		float x = verts[i * 3];
		if (x < minX)
		{
			minX = x;
		}
		if (x>maxX)
		{
			maxX = x;
		}
		float y = verts[i * 3 + 1];
		if (y < minY)
		{
			minY = y;
		}
		if (y>maxY)
		{
			maxY = y;
		}
		float z = verts[i * 3 + 2];
		if (z < minZ)
		{
			minZ = z;
		}
		if (z>maxZ)
		{
			maxZ = z;
		}
	}
	//boundingBox.c = model->getTransform().position;
	boundingBox.r = glm::vec3((maxY - minY)/2,(maxZ - minZ) /2, (maxX - minX) / 2);
}