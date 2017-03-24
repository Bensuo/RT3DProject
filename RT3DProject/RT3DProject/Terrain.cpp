#include "Terrain.h"
#include <SDL.h>
#include <vector>
#include "rt3d.h"

Terrain::Terrain()
{
	scale = glm::vec3(1.0);
}


Terrain::~Terrain()
{

}

void Terrain::loadContent(const char * fname, const char * normname, Utilities::ResourceManager& content)
{
	//Loading pixel data and setting vertex data (where the y component is from the pixel)
	//Also sets texture coordinates
	SDL_Surface* tmpSurface;
	SDL_Surface* normSurface;
	tmpSurface = SDL_LoadBMP(fname);
	normSurface = SDL_LoadBMP(normname);
	SDL_PixelFormat *format = tmpSurface->format;
	SDL_PixelFormat* normFormat = normSurface->format;
	Uint32* pixels = (Uint32*)tmpSurface->pixels;
	Uint32* norms = (Uint32*)normSurface->pixels;
	Uint32 pixel;
	cols = tmpSurface->w;
	rows = tmpSurface->h;
	std::vector<GLfloat> vertData;
	std::vector<GLfloat> normalData;
	std::vector<GLfloat> coordData;
	std::vector<GLuint> triIndexData;
	Uint8 red, blue ,green;
	//Determines how often we tile the texture
	float texU = float(cols) / 32.0f;
	float texV = float(rows) / 32.0f;

	int i = 0;
	for (int z = 0; z < rows; z++) for (int x = 0; x < cols; x++)
	{
		//Get a colour component of the pixel for the height map, image is grey scale so it doesn't matter which we get
		pixel = pixels[z*cols + x];
		SDL_GetRGB(pixel, format, &red, &red, &red);
		//Scale grid coordinates between 0 and 1
		float xVal = (float)x / float(cols - 1);
		float zVal = (float)z / float(rows - 1);
		//Add vertex and scale it
		vertData.push_back((-0.5f + xVal) * scale.x);
		vertData.push_back((float(red) / 255.0f) * scale.y);
		vertData.push_back((-0.5f + zVal) * scale.z);

		//Get data from normal map, rgb=xyz of the normal
		pixel = norms[z*cols + x];
		SDL_GetRGB(pixel, normFormat, &red, &green, &blue);

		//Add normal, scaling between 0 and 1
		auto test = glm::vec3(2.0f * ((float)red / 255.0f) - 1.0f, 2.0f * ((float)blue / 255.0f) - 1.0f, 2.0f * ((float)green / 255.0f) - 1.0f);

		normalData.push_back(test.x);
		normalData.push_back(test.y);
		normalData.push_back(test.z);

		coordData.push_back(texU*xVal);
		coordData.push_back(texV*zVal);
		//triangles/indices
		if (((i+1) % cols) != 0 && z+1 < rows)
		{
			triIndexData.push_back(i);
			triIndexData.push_back(i + cols);
			triIndexData.push_back(i + cols + 1);

			triIndexData.push_back(i);
			triIndexData.push_back(i + cols + 1);
			triIndexData.push_back(i + 1);
		}
		i++;
	}
	indexCount = triIndexData.size();
	
	//vao = rt3d::createMesh(vertData.size(), vertData.data(), nullptr, normalData.data(), coordData.data(), indexCount, triIndexData.data());
	//Setup OpenGL Buffers
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	GLuint vbo;
	glGenBuffers(1, &vertVBO);
	//Vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * rows*cols * sizeof(GLfloat), vertData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	//Normals
	glGenBuffers(1, &normVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * rows*cols * sizeof(GLfloat), normalData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Texture coords
	glGenBuffers(1, &texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * rows*cols * sizeof(GLfloat), coordData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);	

	//Indices
	glGenBuffers(1, &indexEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triIndexData.size() * sizeof(GLuint), triIndexData.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader = Rendering::Shader("terrain.vert", "phong-tex.frag");
	shader.setMaterial(material);
	texture = content.loadTexture("terrainTex.bmp");
	//Cleanup
	SDL_FreeSurface(tmpSurface);
	SDL_FreeSurface(normSurface);
}
