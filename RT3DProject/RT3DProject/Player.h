#pragma once
#include "IRenderable.h"
#include "PlayerModel.h"
#include "ResourceManager.h"
#include "ViewportWeapon.h"
#include "AABB.h"

class Player
{
public:
	enum PlayerState
	{
		STAND,
		RUN,
		ATTACK,
		PAIN1,
		PAIN2,
		PAIN3,
		JUMP,
		FLIP,
		SALUTE,
		TAUNT,
		WAVE,
		POINT,
		CRSTND,
		CRWALK,
		CRATTAK,
		CRPAIN,
		CRDEATH,
		DEATH1,
		DEATH2,
		DEATH3
	} playerState;
	enum WeaponState
	{
		ACTIVE,
		POW,
		IDLE1,
		IDLE2,
		PUTAWAY
	} weaponState;

	Player();
	virtual ~Player();
	void loadContent(Utilities::ResourceManager content, const std::string& skin);
	void update(float dt);
	Rendering::PlayerModel& getPlayerModel() { return model; };
	Rendering::PlayerModel& getWeapon() { return weapon; };
	Rendering::ViewportWeapon& getVPWeapon() { return vpWeapon; };
	void setPosition(glm::vec3 pos) { transform.position = pos; };
<<<<<<< HEAD
	const AABB& getAABB() { return collider; };
=======
	void setState(PlayerState state);
>>>>>>> master
private:
	Rendering::PlayerModel model;
	Rendering::PlayerModel weapon;
	Rendering::ViewportWeapon vpWeapon;
	Transform transform{ glm::vec3(0), glm::vec3(0) };
	AABB collider{ glm::vec3(0), glm::vec3(50, 50, 50) };
};

