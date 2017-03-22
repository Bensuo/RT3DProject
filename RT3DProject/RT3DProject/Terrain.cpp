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
	for ( int y = 0; y < h-1; y++)
	{
		for (int x = 0; x < w-1; x++)
		{
			pixel = pixels[y*w + x];
			SDL_GetRGB(pixel, format, &red, &red, &red);
			vertData[(x + y)*3] = x;
			vertData[(x + y) * 3 + 1] = (float)red;
			vertData[(x + y) * 3 + 2] = y;
		}
	}
	SDL_FreeSurface(tmpSurface);
}
