#pragma once

#include <SDL.h>
#include "camera.h"

class Input
{
	const uint8_t* currentKeyState;
	const uint8_t* previousKeyState;
	SDL_Event event;
	bool quit = false;
public:
	Input();
	void Update(Camera& camera);
	bool Quit() const;
	~Input();
};