#include "Terrain.h"
#include <SDL.h>
#include <vector>

Terrain::Terrain()
{
	scale = glm::vec2(1.0);
}


Terrain::~Terrain()
{
	delete vertData;
	delete coordsData;
}

void Terrain::loadContent(const char * fname)
{
	//Loading pixel data and setting vertex data (where the y component is from the pixel)
	//Also sets texture coordinates
	SDL_Surface* tmpSurface;
	tmpSurface = SDL_LoadBMP(fname);
	SDL_PixelFormat *format = tmpSurface->format;
	Uint32* pixels = (Uint32*)tmpSurface->pixels;
	Uint32 pixel;
	cols = tmpSurface->w;
	rows = tmpSurface->h;
	vertData = new GLfloat[cols * rows * 3];
	coordsData = new GLfloat[cols * rows * 2];
	std::vector<std::vector<glm::vec3>> vertices(rows, std::vector<glm::vec3>(cols));
	std::vector<std::vector<glm::vec2>> coords(rows, std::vector<glm::vec2>(cols));
	Uint8 red;
	float texU = float(cols) / 8.0f;
	float texV = float(rows) / 8.0f;
	for ( int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			pixel = pixels[y*rows + x];
			SDL_GetRGB(pixel, format, &red, &red, &red);
			//int index = (x + (y*rows)) * 3;
			float xVal = (float)x / float(cols - 1);
			float yVal = (float)y / float(rows - 1);
			float vHeight = float(red) / 255.0f;
			/*vertData[index] = -0.5f + xVal;
			vertData[index + 1] = (float)red;
			vertData[index + 2] = -0.5f +  + yVal;*/
			vertices[x][y] = glm::vec3(-0.5f*xVal, vHeight, -0.5f + yVal);
			coords[x][y] = glm::vec2(texU*xVal, texV*yVal);
			//int cIndex = (x + (y*rows)) * 2;
			//coordsData[index] = texU*xVal;
			//coordsData[index + 1] = texV*yVal;
		}
	}
	SDL_FreeSurface(tmpSurface);
	//calculate normals
	std::vector< std::vector<glm::vec3>> vNormals[2];
	for (int i = 0; i < 2; i++)
	{
		vNormals[i] = std::vector<std::vector<glm::vec3>>(rows - 1, std::vector<glm::vec3>(cols - 1));
	}
	for (int y = 0; y < rows-1; y++)
	{
		for (int x = 0; x < cols-1; x++)
		{
			glm::vec3 tri0[] =
			{
				vertices[x][y],
				vertices[x + 1][y],
				vertices[x + 1][y + 1]
			};
			glm::vec3 tri1[] =
			{
				vertices[x+1][y+1],
				vertices[x][y+1],
				vertices[x][y]
			};
			glm::vec3 triNorm0 = glm::cross(tri0[0] - tri0[1], tri0[1] - tri0[2]);
			glm::vec3 triNorm1 = glm::cross(tri1[0] - tri1[1], tri1[1] - tri1[2]);

			vNormals[0][x][y] = glm::normalize(triNorm0);
			vNormals[1][x][y] = glm::normalize(triNorm1);
		}
	}

	std::vector<std::vector<glm::vec3>>finalNormals = std::vector<std::vector<glm::vec3>>(rows, std::vector< glm::vec3>(cols));
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			glm::vec3 finalNormal = glm::vec3(0.0f, 0.0f, 0.0f);
			
			if (x != 0 && y != 0)
			{
				for (int i = 0; i < 2; i++)
				{
					finalNormal += vNormals[i][x - 1][y - 1];
				}
			}
			if (x != 0 && y != cols-1)
			{
				finalNormal += vNormals[0][x - 1][y];
			}
			if (x != rows-1 && y != cols-1)
			{
				for (int i = 0; i < 2; i++)
				{
					finalNormal += vNormals[i][x][y];
				}
			}
			if (x != rows-1 && y != 0)
			{
				finalNormal += vNormals[1][x][y - 1];
			}
			finalNormal = glm::normalize(finalNormal);
			finalNormals[x][y] = finalNormal;
		}
	}
	int butts = 1;
}
