#include "Input.h"

Input::Input()
{
	currentKeyState = SDL_GetKeyboardState(nullptr);
	previousKeyState = currentKeyState;
}

void Input::Update(Camera& camera)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_MOUSEMOTION:
			SDL_SetRelativeMouseMode(SDL_TRUE);
			camera.ProcessMouseMovement(event.motion.xrel, -event.motion.yrel);
			break;
		case SDL_QUIT:
			break;
		default: 
			break;
		}
	}

	currentKeyState = SDL_GetKeyboardState(nullptr);
	if (currentKeyState[SDL_SCANCODE_W] && previousKeyState[SDL_SCANCODE_W])
	{
		camera.ProcessKeyboard(FORWARD);
	}
	else if (currentKeyState[SDL_SCANCODE_S] && previousKeyState[SDL_SCANCODE_S])
	{
		camera.ProcessKeyboard(BACKWARD);
	}
	else if (currentKeyState[SDL_SCANCODE_A] && previousKeyState[SDL_SCANCODE_A])
	{
		camera.ProcessKeyboard(LEFT);
	}
	else if (currentKeyState[SDL_SCANCODE_D] && previousKeyState[SDL_SCANCODE_D])
	{
		camera.ProcessKeyboard(RIGHT);
	}

	previousKeyState = currentKeyState;
}


Input::~Input()
{
}