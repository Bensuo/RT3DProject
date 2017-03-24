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

	model.update(dt);
	model.setTransform(transform);
	weapon.update(dt);
	weapon.setTransform(transform);
	vpWeapon.update(dt);

	collider.c = transform.position;

	playerState = STAND;
	sprint = false;
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
	auto y = transform.position.y;

	if (!sprint) 
	{
		this->transform.position += this->movementNormal * (SPEED * deltaTime);
	}
	else
	{
		this->transform.position += this->movementNormal * (SPEED * 1.66f * deltaTime);
	}

	transform.position.y = y;
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

	playerState = RUN;
}

void Player::MoveBackward()
{
	movementNormal -= this->front;

	if (!fps && !aiming)
		transform.rotation.z = atan2f(movementNormal.x, movementNormal.z) * 57.2958;

	playerState = RUN;
}

void Player::MoveLeft()
{
	movementNormal -= this->right;

	if (!fps && !aiming)
		transform.rotation.z = atan2f(movementNormal.x, movementNormal.z) * 57.2958;

	playerState = RUN;
}

void Player::MoveRight()
{
	movementNormal += this->right;

	if (!fps && !aiming)
		transform.rotation.z = atan2f(movementNormal.x, movementNormal.z) * 57.2958;

	playerState = RUN;
}

void Player::Jump()
{
	playerState = JUMP;
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
