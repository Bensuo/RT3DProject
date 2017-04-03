#include "AIController.h"
#define uniform() (rand()/(RAND_MAX + 1.0f))


AIController::AIController(): target(nullptr)
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
		auto toTarget = target->getPosition() - self->getPosition();
		if (self->playerState != Player::PlayerState::DEATH1)
		{
			if (length(toTarget) < 300)
			{
				self->updateVectors(toTarget);
				self->moveForward();
			}
			else
			{
				if (actionCounter > 180)
				{
					actionCounter = 0;
					self->updateVectors(glm::vec3(-uniform() + uniform(), 0, -uniform() + uniform()));
				}
				if (actionCounter < 60)
				{
					self->moveForward();
				}

			}
			actionCounter++;
		}
	}
}
