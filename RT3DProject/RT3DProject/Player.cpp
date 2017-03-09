#include "Player.h"



Player::Player()
{
	model.setAnimation(1);
	weapon.setAnimation(1);
	vpWeapon.setAnimation(2);
}


Player::~Player()
{
}

void Player::loadContent(Utilities::ResourceManager content)
{
	model.loadContent(content, "res/md2/rampage");
	weapon.loadContent(content, "res/md2/weapon");
	vpWeapon.loadContent(content, "res/md2/v_machn");
}

void Player::update(float dt)
{
	model.update(dt);
	model.setTransform(transform);
	weapon.update(dt);
	weapon.setTransform(transform);
	vpWeapon.update(dt);

	collider.c = transform.position;
}


