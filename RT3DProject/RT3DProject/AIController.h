#pragma once
#include "Player.h"
class AIController
{
public:
	AIController();
	virtual ~AIController();
	void setActor(const std::shared_ptr<Player>& actor) { this->actor = actor; }
	void setTarget(Player* target) {
		this->target = target;
	}
	void update();
private:
	std::weak_ptr<Player> actor;
	Player* target;
	int actionCounter;
};