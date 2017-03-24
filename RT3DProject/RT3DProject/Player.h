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
	void setFPS(bool fps);
	const glm::vec3& normalise(glm::vec3& vector) const;
	void updatePosition(float deltaTime);
	Rendering::PlayerModel& getPlayerModel();
	Rendering::PlayerModel& getWeapon();
	Rendering::ViewportWeapon& getVPWeapon();
	const glm::vec3& getPosition();
	void setPosition(glm::vec3 pos);
	const glm::vec3& getPosition() const;
	const AABB& getAABB() const;
	const float& getAimDistance() const;
	void setState(PlayerState state);
	void UpdateVectors(const glm::vec3& front);
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
	void Jump();
	void Sprint();
	void Aim();
	void ClampPosition(const glm::vec3& min, const glm::vec3& max);
	void StopAim();
private:
	Rendering::PlayerModel model;
	Rendering::PlayerModel weapon;
	Rendering::ViewportWeapon vpWeapon;

	Transform transform
	{ 
		glm::vec3(0), glm::vec3(0) 
	};

	AABB collider
	{
		glm::vec3(0), glm::vec3(15, 35, 15)
	};

	glm::vec3 front;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = normalize(cross(up, front));
	glm::vec3 movementNormal, targetNormal;
	glm::vec3 velocity;

	const float SPEED = 100.0f;
	const float AIM_DISTANCE = 75.0f;
	bool fps = false;
	bool sprint = false;
	bool aiming = false;
};

