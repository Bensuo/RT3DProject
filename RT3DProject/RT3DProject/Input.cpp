#include "Input.h"

Input::Input(): previousKeyState(nullptr)
{
	currentKeyState = SDL_GetKeyboardState(nullptr);
}

void Input::update(Player* player, camera& camera, bool freezeControls)
{
	if (!freezeControls)
	{
		while (SDL_PollEvent(&event))
		{
			auto motion = glm::vec2();
			switch (event.type)
			{
			case SDL_MOUSEMOTION:
				motion = glm::vec2(event.motion.xrel, -event.motion.yrel);
				camera.processMouseMovement(motion);
				break;
			case SDL_MOUSEWHEEL:
				camera.processMouseScroll(-event.wheel.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_RIGHT)
				{
					camera.snapDistance(player->getAimDistance());
					player->aim();
				}
				if (event.button.button == SDL_BUTTON_MIDDLE)
				{
					camera.snapToMaxDistance();
				}
				if (event.button.button == SDL_BUTTON_LEFT && (player->aiming() || camera.isFPS()))
				{
					player->shoot();
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_RIGHT)
				{
					player->stopAim();
				}
				break;
			case SDL_QUIT:
				isQuitting = true;
				break;
			default:
				break;
			}
		}

		currentKeyState = SDL_GetKeyboardState(nullptr);
		if (currentKeyState[SDL_SCANCODE_W])
		{
			player->moveForward();
		}
		if (currentKeyState[SDL_SCANCODE_S])
		{
			player->moveBackward();
		}
		if (currentKeyState[SDL_SCANCODE_A])
		{
			player->moveLeft();
		}
		if (currentKeyState[SDL_SCANCODE_D])
		{
			player->moveRight();
		}
		if (currentKeyState[SDL_SCANCODE_SPACE])
		{
			player->jump();
		}
		if (currentKeyState[SDL_SCANCODE_LSHIFT])
		{
			player->sprint();
		}
		if (currentKeyState[SDL_SCANCODE_ESCAPE])
		{
			isQuitting = true;
		}
	}
	else
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				isQuitting = true;
				break;
			default:
				break;
			}
		}

		if (currentKeyState[SDL_SCANCODE_ESCAPE])
		{
			isQuitting = true;
		}
	}
}

bool Input::quit() const
{
	return isQuitting;
}


Input::~Input()
{
}
