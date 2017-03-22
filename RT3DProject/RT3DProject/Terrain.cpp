#include "Terrain.h"
#include <SDL.h>


Terrain::Terrain()
{
}


Terrain::~Terrain()
{
}

void Terrain::loadContent(const char * fname)
{
	SDL_Surface* tmpSurface;
	tmpSurface = SDL_LoadBMP(fname);
	SDL_PixelFormat *format = tmpSurface->format;
	Uint32* pixels = (Uint32*)tmpSurface->pixels;
	Uint32 pixel;
	int w = tmpSurface->w;
	int h = tmpSurface->h;
	vertData = new GLfloat[w * h * 3];
	Uint8 red;
	for ( int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			pixel = pixels[y*w + x];
			SDL_GetRGB(pixel, format, &red, &red, &red);
			int index = (x + (y*h)) * 3;
			vertData[index] = x;
			vertData[index + 1] = (float)red;
			vertData[index + 2] = y;
		}
	}
	SDL_FreeSurface(tmpSurface);
}
