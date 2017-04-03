#include "Terrain.h"
#include <SDL.h>

Terrain::Terrain(): rows(0), cols(0), indexCount(0), vertVBO(0), normVBO(0), texVBO(0), indexEBO(0), vao(0)
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
	auto format = tmpSurface->format;
	auto normFormat = normSurface->format;
	auto pixels = static_cast<Uint32*>(tmpSurface->pixels);
	auto norms = static_cast<Uint32*>(normSurface->pixels);
	Uint32 pixel;
	cols = tmpSurface->w;
	rows = tmpSurface->h;
	heights = std::vector<std::vector<float>>(cols, std::vector<float>(rows));
	std::vector<GLfloat> vertData;
	std::vector<GLfloat> normalData;
	std::vector<GLfloat> coordData;
	std::vector<GLuint> triIndexData;
	Uint8 red, blue ,green;
	//Determines how often we tile the texture
	auto texU = float(cols) / 32.0f;
	auto texV = float(rows) / 32.0f;

	auto i = 0;
	for (auto z = 0; z < rows; z++) for (auto x = 0; x < cols; x++)
	{
		//Get a colour component of the pixel for the height map, image is grey scale so it doesn't matter which we get
		pixel = pixels[z*cols + x];
		SDL_GetRGB(pixel, format, &red, &red, &red);
		//Scale grid coordinates between 0 and 1
		auto xVal = static_cast<float>(x) / float(cols - 1);
		auto zVal = static_cast<float>(z) / float(rows - 1);
		//Add vertex and scale it
		vertData.push_back((-0.5f + xVal) * scale.x);
		auto h = (float(red) / 255.0f) * scale.y;
		heights[x][z] = h;
		vertData.push_back(h);
		vertData.push_back((-0.5f + zVal) * scale.z);

		//Get data from normal map, rgb=xyz of the normal
		pixel = norms[z*cols + x];
		SDL_GetRGB(pixel, normFormat, &red, &green, &blue);

		//Add normal, scaling between 0 and 1
		auto test = glm::vec3(2.0f * (static_cast<float>(red) / 255.0f) - 1.0f, 2.0f * (static_cast<float>(blue) / 255.0f) - 1.0f, 2.0f * (static_cast<float>(green) / 255.0f) - 1.0f);

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
	

	//Setup OpenGL Buffers
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vertVBO);
	//Vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * rows*cols * sizeof(GLfloat), vertData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	//Normals
	glGenBuffers(1, &normVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * rows*cols * sizeof(GLfloat), normalData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//Texture coords
	glGenBuffers(1, &texVBO);
	glBindBuffer(GL_ARRAY_BUFFER, texVBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * rows*cols * sizeof(GLfloat), coordData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr);	

	//Indices
	glGenBuffers(1, &indexEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triIndexData.size() * sizeof(GLuint), triIndexData.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader = Rendering::Shader("res/shaders/phong-tex.vert", "res/shaders/terrain.frag");
	shader.setMaterial(material);
	texture[0] = *content.loadTexture("res/textures/terrain/background.bmp").get();
	texture[1] = *content.loadTexture("res/textures/terrain/red.bmp").get();
	texture[2] = *content.loadTexture("res/textures/terrain/green.bmp").get();
	texture[3] = *content.loadTexture("res/textures/terrain/blue.bmp").get();
	texture[4] = *content.loadTexture("res/textures/terrain/blendmap.bmp").get();
	//Cleanup
	SDL_FreeSurface(tmpSurface);
	SDL_FreeSurface(normSurface);
}

//Returns the height at a given world x, z using barycentric coordinates to get the interpolated height on a triangle surface
const float& Terrain::getHeightAtPosition(const float& x, const float& z) const
{
	auto tX = x - transform.position.x + (scale.x/2);
	auto tZ = z - transform.position.z + (scale.z / 2);
	auto gridSquareSize = scale.x / (cols - 1);
	auto gridX = static_cast<int>(tX / gridSquareSize);
	auto gridZ = static_cast<int>(tZ / gridSquareSize);

	auto xCoord = fmod(tX, gridSquareSize) / gridSquareSize;
	auto zCoord = fmod(tZ ,gridSquareSize) / gridSquareSize;

	//Determine which side of the quad (aka which triangle) we're on and call the method with the appropriate heights/coordinates
	if (xCoord <= zCoord)
	{
		return getBarycentricHeight(glm::vec2(xCoord, zCoord), glm::vec3(0, heights[gridX][gridZ], 0), glm::vec3(1, heights[gridX + 1][gridZ + 1], 1),
		                            glm::vec3(0, heights[gridX][gridZ + 1], 1));
	}
	else
	{
		return getBarycentricHeight(glm::vec2(xCoord, zCoord), glm::vec3(0, heights[gridX][gridZ], 0), glm::vec3(1, heights[gridX + 1][gridZ + 1], 1),
		                            glm::vec3(1, heights[gridX + 1][gridZ], 0));
	}
	
}

const float& Terrain::getBarycentricHeight(const glm::vec2& pos, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) const
{
	auto det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	auto l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	auto l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	auto l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}
