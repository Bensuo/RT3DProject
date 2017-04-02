#pragma once

#include <SDL.h>
#include "camera.h"
#include "Player.h"

class Input
{
	const uint8_t* currentKeyState;
	const uint8_t* previousKeyState;
	SDL_Event event;
	bool isQuitting = false;
public:
	Input();
	void update(Player* player, Camera& camera, const bool& freezeControls);
	bool quit() const;
	~Input();
};