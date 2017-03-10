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
			camera.ProcessMouseMovement(event.motion.xrel, -event.motion.yrel);
			break;
		case SDL_QUIT:
			quit = true;
			break;
		default: 
			break;
		}
	}

	currentKeyState = SDL_GetKeyboardState(nullptr);
	if (currentKeyState[SDL_SCANCODE_W] && previousKeyState[SDL_SCANCODE_W])
	{
		camera.MoveForward();
	}
	if (currentKeyState[SDL_SCANCODE_S] && previousKeyState[SDL_SCANCODE_S])
	{
		camera.MoveBackward();
	}
	if (currentKeyState[SDL_SCANCODE_A] && previousKeyState[SDL_SCANCODE_A])
	{
		camera.MoveLeft();
	}
	if (currentKeyState[SDL_SCANCODE_D] && previousKeyState[SDL_SCANCODE_D])
	{
		camera.MoveRight();
	}

	if (currentKeyState[SDL_SCANCODE_ESCAPE])
	{
		quit = true;
	}

	previousKeyState = currentKeyState;
}

bool Input::Quit() const
{
	return quit;
}


Input::~Input()
{
}
