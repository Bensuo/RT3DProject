#include "AIController.h"
#define uniform() (rand()/(RAND_MAX + 1.0f))


AIController::AIController()
{
	actionCounter = 180;
}


AIController::~AIController()
{
}

void AIController::update()
{
	if (!actor.expired())
	{
		auto self = actor.lock();
		glm::vec3 toTarget = target->getPosition() - self->getPosition();
		if (self->playerState != Player::PlayerState::DEATH1)
		{
			if (glm::length(toTarget) < 300)
			{
				self->UpdateVectors(toTarget);
				self->MoveForward();
			}
			else
			{
				if (actionCounter > 180)
				{
					actionCounter = 0;
					self->UpdateVectors(glm::vec3(-uniform() + uniform(), 0, -uniform() + uniform()));
				}
				if (actionCounter < 60)
				{
					self->MoveForward();
				}

			}
			actionCounter++;
		}
	}
}
