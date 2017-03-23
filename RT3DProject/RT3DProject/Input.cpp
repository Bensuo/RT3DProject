#include "Input.h"

Input::Input()
{
	currentKeyState = SDL_GetKeyboardState(nullptr);
	previousKeyState = currentKeyState;
}

void Input::Update(Player* player, Camera& camera)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_MOUSEMOTION:
			camera.ProcessMouseMovement(event.motion.xrel, -event.motion.yrel);
			break;
		case SDL_MOUSEWHEEL:
			camera.ProcessMouseScroll(-event.wheel.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
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
		player->MoveForward();
	}
	if (currentKeyState[SDL_SCANCODE_S] && previousKeyState[SDL_SCANCODE_S])
	{
		player->MoveBackward();
	}
	if (currentKeyState[SDL_SCANCODE_A] && previousKeyState[SDL_SCANCODE_A])
	{
		player->MoveLeft();
	}
	if (currentKeyState[SDL_SCANCODE_D] && previousKeyState[SDL_SCANCODE_D])
	{
		player->MoveRight();
	}
	if (currentKeyState[SDL_SCANCODE_SPACE] && previousKeyState[SDL_SCANCODE_SPACE])
	{
		player->Jump();
	}
	if (currentKeyState[SDL_SCANCODE_LSHIFT] && previousKeyState[SDL_SCANCODE_LSHIFT])
	{
		player->Sprint();
	}
	if (!currentKeyState[SDL_SCANCODE_W] &&
		!currentKeyState[SDL_SCANCODE_S] &&
		!currentKeyState[SDL_SCANCODE_A] &&
		!currentKeyState[SDL_SCANCODE_D])
	{
		//player->Idle();
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
