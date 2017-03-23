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
	void setFPS(bool fps) { this->fps = fps; }

	const glm::vec3& normalise(glm::vec3& vector) const
	{
		if (vector != glm::vec3(0))
		{
			return vector = normalize(vector);
		}
		return vector;
	}

	void updatePosition(float deltaTime)
	{
		normalise(movementNormal);
		auto y = transform.position.y;
		
		if (!sprint) {
			this->transform.position += this->movementNormal * (SPEED * deltaTime);
		} else {
			this->transform.position += this->movementNormal * (SPEED * 1.66f * deltaTime);
		}

		transform.position.y = y;
		movementNormal = glm::vec3();
	}

	Rendering::PlayerModel& getPlayerModel() { return model; };
	Rendering::PlayerModel& getWeapon() { return weapon; };
	Rendering::ViewportWeapon& getVPWeapon() { return vpWeapon; };
	glm::vec3 getPosition() { return transform.position; };
	void setPosition(glm::vec3 pos) { transform.position = pos; };

	const glm::vec3& getPosition() const { return this->transform.position; }

	const AABB& getAABB() const { return collider; };
	void setState(PlayerState state);

	void UpdateVectors(const glm::vec3& front)
	{
		this->front = normalize(front);
		this->right = normalize(cross(this->front, this->worldUp));
		this->up = normalize(cross(this->right, this->front));
	}

	void MoveForward()
	{
		movementNormal += this->front;

		if(!fps)
			transform.rotation.z = atan2f(movementNormal.x, movementNormal.z) * 57.2958;

		playerState = RUN;
	}

	void MoveBackward()
	{
		movementNormal -= this->front;

		if (!fps)
			transform.rotation.z = atan2f(movementNormal.x, movementNormal.z) * 57.2958;

		playerState = RUN;
	}

	void MoveLeft()
	{
		movementNormal -= this->right;

		if (!fps)
			transform.rotation.z = atan2f(movementNormal.x, movementNormal.z) * 57.2958;

		playerState = RUN;
	}

	void MoveRight()
	{
		movementNormal += this->right;

		if (!fps)
			transform.rotation.z = atan2f(movementNormal.x, movementNormal.z) * 57.2958;

		playerState = RUN;
	}

	void Jump()
	{
		playerState = JUMP;
	}

	void Sprint()
	{
		sprint = true;
	}

	void ClampPosition(const glm::vec3& min, const glm::vec3& max)
	{
		transform.position = clamp(transform.position, min, max);
	}

private:
	Rendering::PlayerModel model;
	Rendering::PlayerModel weapon;
	Rendering::ViewportWeapon vpWeapon;
	Transform transform{ glm::vec3(0), glm::vec3(0) };

	glm::vec3 front;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = normalize(cross(up, front));
	glm::vec3 movementNormal;

	const float SPEED = 100.0;
	bool fps;
	bool sprint;

	AABB collider{ glm::vec3(0), glm::vec3(15, 35, 15) };
};

