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
	void loadContent(Utilities::ResourceManager& content, const std::string& skin);
	void update(const float& dt);
	void setFPS(const bool& fps);
	const glm::vec3& normalise(glm::vec3& vector) const;
	void updatePosition(const float& deltaTime);
	const IRenderable* getPlayerModel() const;
	const IRenderable* getWeapon() const;
	const IRenderable* getVPWeapon() const;
	const glm::vec3& getPosition();
	void setPosition(const glm::vec3& pos);
	const glm::vec3& getPosition() const;
	const AABB& getAABB() const;
	const float& getAimDistance() const;
	void setState(const PlayerState& state);
	void updateVectors(const glm::vec3& cameraFront);
	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();
	void jump();
	void land();
	void sprint();
	void aim();
	void clampPosition(const glm::vec3& min, const glm::vec3& max);
	void stopAim();
	void shoot();
	const bool& getCanShoot() const { return canShoot; }
	void giveAmmo(const int& ammo)
	{
		this->ammo += ammo;
	}
	void hasShot() { canShoot = false; }
	void takeDamage(const int& amount)
	{
		health -= amount; 
		health = glm::clamp(health, 0, 100);
		if (health <= 0 && playerState != DEATH1) playerState = DEATH1;
	}
	const int& getHealth() const { return health; }
	const int& getAmmo() const { return ammo; }
	const bool& getIsDead() const { return isDead; }
	bool aiming() const;
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
		glm::vec3(0), glm::vec3(15, 25, 15)
	};

	glm::vec3 front;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = normalize(cross(up, front));
	glm::vec3 movementNormal, targetNormal;
	glm::vec3 velocity;

	const float SPEED = 150.0f;
	const float AIM_DISTANCE = 75.0f;
	bool fps = false;
	bool isSprinting = false;
	bool isAiming = false;
	bool canShoot = false;
	bool isDead = false;
	float shootTimer = 0.0f;
	int stepCount = 40;
	int health = 100;
	int ammo = 20;

	int vWepFinalFrame = 5;
	int wepFinalFrame = 53;
	int deathFinalFrame = 183;
};