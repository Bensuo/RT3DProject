#pragma once

#include <SDL.h>
#include "camera.h"
#include "Player.h"

class Input
{
	const uint8_t* currentKeyState;
	const uint8_t* previousKeyState;
	SDL_Event event;
	bool quit = false;
public:
	Input();
	void Update(Player* player, Camera& camera);
	bool Quit() const;
	~Input();
};