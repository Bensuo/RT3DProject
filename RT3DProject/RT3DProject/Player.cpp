#include "Player.h"
#include "AudioManager.h"

Player::Player(): playerState(), weaponState(), fps(false), isSprinting(false)
{
	model.setAnimation(playerState);
	weapon.setAnimation(playerState);
	vpWeapon.setAnimation(IDLE1);
}

Player::~Player()
{
}

void Player::loadContent(Utilities::ResourceManager& content, const std::string& skin)
{
	model.loadContent(content, "res/md2/player", "res/md2/" + skin);
	weapon.loadContent(content, "res/md2/weapon", "res/md2/weapon");
	vpWeapon.loadContent(content, "res/md2/v_machn", "res/md2/v_machn");
}

void Player::update(const float& dt)
{
	if (fps || isAiming)
		transform.rotation.z = atan2f(front.x, front.z) * 57.2958;
	if (playerState != DEATH1)
	{
		updatePosition(dt);
		weapon.setAnimation(playerState);
		weapon.update(dt);
	}
	

	model.setAnimation(playerState);
	
	vpWeapon.setAnimation(weaponState);
	model.update(dt);
	model.setTransform(transform);

	
	weapon.setTransform(transform);
	vpWeapon.update(dt);

	collider.c = transform.position;
	if (playerState == RUN)
	{
		if (stepCount == 40)
		{
			stepCount = 0;
			AudioManager::playSound("res/audio/sfx/PlayerMove1.wav", 0.4f);
		}
		stepCount++;
	}

	if (playerState != JUMP && playerState != ATTACK && playerState != DEATH1)
	{
		isSprinting = false;
		playerState = STAND;
	} 
	else if (playerState == ATTACK)
	{
		if(model.getCurrentFrame() == wepFinalFrame)
		{
			isSprinting = false;
			playerState = STAND;
		}
	} 

	if (weaponState != POW)
	{
		weaponState = IDLE2;
	}
	else
	{
		if (vpWeapon.getCurrentFrame() == vWepFinalFrame)
		{
			weaponState = IDLE2;
		}
	}
	if (playerState == DEATH1 && model.getCurrentFrame() == deathFinalFrame)
	{
		isDead = true;
	}
	shootTimer -= dt;
}

void Player::setFPS(const bool& fps)
{
	this->fps = fps;
}

void Player::setState(const PlayerState& state)
{
	model.setAnimation(state);
	weapon.setAnimation(state);
}

const glm::vec3& Player::normalise(glm::vec3& vector) const
{
	if (vector != glm::vec3(0))
	{
		return vector = normalize(vector);
	}
	return vector;
}

void Player::updatePosition(const float& deltaTime)
{
	normalise(movementNormal);
	//Remove uneeded y component of movement normal
	movementNormal.y = 0;
	auto y = transform.position.y;
	float maxSpeed;
	if (!isSprinting) {
		this->movementNormal *= (SPEED * deltaTime);
		maxSpeed = (SPEED * deltaTime);
	}
	else {
		this->movementNormal *=  (SPEED * 1.66f * deltaTime);
		maxSpeed = (SPEED * 1.66f *  deltaTime);
	}
	this->velocity.x = movementNormal.x;
	this->velocity.y -= 8.0f * deltaTime;
	this->velocity.z = movementNormal.z;
	
	velocity = glm::clamp(velocity, glm::vec3(-maxSpeed, -10, -maxSpeed), glm::vec3(maxSpeed, 40, maxSpeed));
	velocity.y *= 0.97f;
	this->transform.position += velocity;

	movementNormal = glm::vec3();
}

const IRenderable* Player::getPlayerModel() const
{
	return &model;
};

const IRenderable* Player::getWeapon() const
{
	return &weapon;
};

const IRenderable* Player::getVPWeapon() const
{
	return &vpWeapon;
};

const glm::vec3& Player::getPosition()
{
	return transform.position;
};

void Player::setPosition(const glm::vec3& pos)
{
	transform.position = pos;
};

const glm::vec3& Player::getPosition() const
{
	return this->transform.position;
}

const AABB& Player::getAABB() const
{
	return collider;
}

const float& Player::getAimDistance() const
{
	return AIM_DISTANCE;
}

void Player::updateVectors(const glm::vec3& cameraFront)
{
	this->front = normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
	this->right = normalize(cross(this->front, this->worldUp));
	this->up = normalize(cross(this->right, this->front));
}

void Player::moveForward()
{
	movementNormal += this->front;

	if (!fps && !isAiming)
		transform.rotation.z = atan2f(movementNormal.x, movementNormal.z) * 57.2958;
	if (playerState != JUMP)
	{
		playerState = RUN;
	}
}

void Player::moveBackward()
{
	movementNormal -= this->front;

	if (!fps && !isAiming)
		transform.rotation.z = atan2f(movementNormal.x, movementNormal.z) * 57.2958;

	if (playerState != JUMP)
	{
		playerState = RUN;
	}
}

void Player::moveLeft()
{
	movementNormal -= this->right;

	if (!fps && !isAiming)
		transform.rotation.z = atan2f(movementNormal.x, movementNormal.z) * 57.2958;

	if (playerState != JUMP)
	{
		playerState = RUN;
	}
}

void Player::moveRight()
{
	movementNormal += this->right;

	if (!fps && !isAiming)
		transform.rotation.z = atan2f(movementNormal.x, movementNormal.z) * 57.2958;

	if (playerState != JUMP)
	{
		playerState = RUN;
	}
}

void Player::jump()
{
	if (playerState != JUMP)
	{
		model.resetAnimation();
		weapon.resetAnimation();
		playerState = JUMP;
		velocity.y = 4.0f;
		AudioManager::playSound("res/audio/sfx/PlayerJump1.wav");
	}
}

void Player::land()
{
	velocity.y = 0;
	if (playerState == JUMP)
	{	
		playerState = STAND;
	}
	
}

void Player::sprint()
{
	isSprinting = true;
}

void Player::aim()
{
	if(!fps)
		isAiming = true;
}

void Player::clampPosition(const glm::vec3& min, const glm::vec3& max)
{
	transform.position = clamp(transform.position, min, max);
}

void Player::stopAim()
{
	if(!fps)
		isAiming = false;
}

void Player::shoot()
{
	if (shootTimer < 0 && canShoot == false && ammo > 0)
	{
		shootTimer = 0.2f;
		canShoot = true;
		AudioManager::playSound("res/audio/sfx/magnum.wav", 0.5f);
		weaponState = POW;
		playerState = ATTACK;
		ammo--;
	}
}

const bool& Player::aiming() const
{
	return fps || isAiming;
}