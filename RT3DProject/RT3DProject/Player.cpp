#include "Player.h"
#include <iostream>

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
	if (fps)
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

void Player::setState(PlayerState state)
{
	model.setAnimation(state);
	weapon.setAnimation(state);
}