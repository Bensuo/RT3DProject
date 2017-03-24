#include "Input.h"

Input::Input()
{
	currentKeyState = SDL_GetKeyboardState(nullptr);

}

void Input::Update(Player* player, Camera& camera)
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_MOUSEMOTION:
			camera.ProcessMouseMovement(glm::vec2(event.motion.xrel, -event.motion.yrel));
			break;
		case SDL_MOUSEWHEEL:
			camera.ProcessMouseScroll(-event.wheel.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				camera.SnapDistance(player->getAimDistance());
				player->Aim();
			}
			if (event.button.button == SDL_BUTTON_MIDDLE)
			{
				camera.SnapToMaxDistance();
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				player->StopAim();
			}
			if (event.button.button == SDL_BUTTON_MIDDLE)
			{
				camera.SnapToMaxDistance();
			}
			break;
		case SDL_QUIT:
			quit = true;
			break;
		default: 
			break;
		}
	}

	currentKeyState = SDL_GetKeyboardState(nullptr);
	if (currentKeyState[SDL_SCANCODE_W])
	{
		player->MoveForward();
	}
	if (currentKeyState[SDL_SCANCODE_S])
	{
		player->MoveBackward();
	}
	if (currentKeyState[SDL_SCANCODE_A])
	{
		player->MoveLeft();
	}
	if (currentKeyState[SDL_SCANCODE_D])
	{
		player->MoveRight();
	}
	if (currentKeyState[SDL_SCANCODE_SPACE])
	{
		player->Jump();
	}
	if (currentKeyState[SDL_SCANCODE_LSHIFT])
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


}

bool Input::Quit() const
{
	return quit;
}


Input::~Input()
{
}
