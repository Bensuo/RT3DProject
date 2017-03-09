#pragma once
#include "IRenderable.h"
#include "PlayerModel.h"
#include "ResourceManager.h"
#include "ViewportWeapon.h"
class Player
{
public:
	Player();
	virtual ~Player();
	void loadContent(Utilities::ResourceManager content);
	void update(float dt);
	Rendering::PlayerModel& getPlayerModel() { return model; };
	Rendering::PlayerModel& getWeapon() { return weapon; };
	Rendering::ViewportWeapon& getVPWeapon() { return vpWeapon; };
	void setPosition(glm::vec3 pos) { transform.position = pos; };
private:
	Rendering::PlayerModel model;
	Rendering::PlayerModel weapon;
	Rendering::ViewportWeapon vpWeapon;
	Transform transform{ glm::vec3(0), glm::vec3(0) };
};
