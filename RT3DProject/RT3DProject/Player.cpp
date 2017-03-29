#include "Player.h"

Player::Player(): playerState(), weaponState(), fps(false), sprint(false)
{
	model.setAnimation(playerState);
	weapon.setAnimation(playerState);
	vpWeapon.setAnimation(IDLE1);
}

Player::~Player()
{
}

void Player::loadContent(Utilities::ResourceManager content, const std::string& skin)
{
	model.loadContent(content, "res/md2/player", "res/md2/" + skin);
	weapon.loadContent(content, "res/md2/weapon", "res/md2/weapon");
	vpWeapon.loadContent(content, "res/md2/v_machn", "res/md2/v_machn");
}

void Player::update(float dt)
{
	if (fps || aiming)
		transform.rotation.z = atan2f(front.x, front.z) * 57.2958;

	updatePosition(dt);

	model.setAnimation(playerState);
	weapon.setAnimation(playerState);
	vpWeapon.setAnimation(weaponState);
	model.update(dt);
	model.setTransform(transform);

	weapon.update(dt);
	weapon.setTransform(transform);
	vpWeapon.update(dt);

	collider.c = transform.position;
	if (playerState == RUN)
	{
		if (stepCount == 40)
		{
			stepCount = 0;
			AudioManager::PlaySound("res/audio/sfx/PlayerMove1.wav", 0.4f);
		}
		stepCount++;
	}

	if (playerState != JUMP && playerState != ATTACK)
	{
		sprint = false;
		playerState = STAND;
	} 
	else if (playerState == ATTACK)
	{
		if(model.getCurrentFrame() == wepFinalFrame)
		{
			sprint = false;
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

	shootTimer -= dt;
}

void Player::setFPS(bool fps)
{
	this->fps = fps;
}

void Player::setState(PlayerState state)
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

void Player::updatePosition(float deltaTime)
{
	normalise(movementNormal);
	//Remove uneeded y component of movement normal
	movementNormal.y = 0;
	auto y = transform.position.y;
	float maxSpeed;
	if (!sprint) {
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

Rendering::PlayerModel& Player::getPlayerModel()
{
	return model;
};

Rendering::PlayerModel& Player::getWeapon()
{
	return weapon;
};

Rendering::ViewportWeapon& Player::getVPWeapon()
{
	return vpWeapon;
};

const glm::vec3& Player::getPosition()
{
	return transform.position;
};

void Player::setPosition(glm::vec3 pos)
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

void Player::UpdateVectors(const glm::vec3& cameraFront)
{
	this->front = normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
	this->right = normalize(cross(this->front, this->worldUp));
	this->up = normalize(cross(this->right, this->front));
}

void Player::MoveForward()
{
	movementNormal += this->front;

	if (!fps && !aiming)
		transform.rotation.z = atan2f(movementNormal.x, movementNormal.z) * 57.2958;
	if (playerState != JUMP)
	{
		playerState = RUN;
	}
}

void Player::MoveBackward()
{
	movementNormal -= this->front;

	if (!fps && !aiming)
		transform.rotation.z = atan2f(movementNormal.x, movementNormal.z) * 57.2958;

	if (playerState != JUMP)
	{
		playerState = RUN;
	}
}

void Player::MoveLeft()
{
	movementNormal -= this->right;

	if (!fps && !aiming)
		transform.rotation.z = atan2f(movementNormal.x, movementNormal.z) * 57.2958;

	if (playerState != JUMP)
	{
		playerState = RUN;
	}
}

void Player::MoveRight()
{
	movementNormal += this->right;

	if (!fps && !aiming)
		transform.rotation.z = atan2f(movementNormal.x, movementNormal.z) * 57.2958;

	if (playerState != JUMP)
	{
		playerState = RUN;
	}
}

void Player::Jump()
{
	if (playerState != JUMP)
	{
		model.resetAnimation();
		weapon.resetAnimation();
		playerState = JUMP;
		velocity.y = 4.0f;
		AudioManager::PlaySound("res/audio/sfx/PlayerJump1.wav");
	}
}

void Player::Land()
{
	velocity.y = 0;
	if (playerState == JUMP)
	{
		
		playerState = STAND;
	}
	
}

void Player::Sprint()
{
	sprint = true;
}

void Player::Aim()
{
	if(!fps)
		aiming = true;
}

void Player::ClampPosition(const glm::vec3& min, const glm::vec3& max)
{
	transform.position = clamp(transform.position, min, max);
}

void Player::StopAim()
{
	if(!fps)
		aiming = false;
}

void Player::shoot()
{
	if (shootTimer < 0 && canShoot == false && ammo > 0)
	{
		shootTimer = 0.2f;
		canShoot = true;
		AudioManager::PlaySound("res/audio/sfx/magnum.wav", 0.5f);
		weaponState = POW;
		playerState = ATTACK;
		ammo--;
	}
}

bool Player::Aiming() const
{
	return fps || aiming;
}