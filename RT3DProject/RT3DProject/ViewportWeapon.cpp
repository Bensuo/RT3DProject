#include "ViewportWeapon.h"

namespace Rendering {

	ViewportWeapon::ViewportWeapon()
	{
		weaponType = ASSAULT_RIFLE;

		ViewportWeapon::setUpAnimationData();
	}

	ViewportWeapon::ViewportWeapon(const WeaponType& type, const glm::vec3& position)
	{
		weaponType = type;
		this->transform.position = position;
		ViewportWeapon::setUpAnimationData();
	}

	void ViewportWeapon::setUpAnimationData()
	{
		std::vector<int>animFrameList;

		switch (weaponType)
		{
		case ASSAULT_RIFLE:
			animFrameList = {
			   0, 3,   //active
			   4, 5,   //pow
			   6, 45,  //idle
			   6, 45,  //idle 2
			   46, 49, //put away
			};
			break;
		case BFG: 
			break;
		case ROCKET_LAUNCHER: 
			break;
		default: 
			break;
		}

		model.setUpAnimationData(animFrameList);
	}

	ViewportWeapon::~ViewportWeapon()
	{
	}
	void ViewportWeapon::setTransform(const Transform & transform)
	{
		this->transform.position = transform.position;
		this->transform.rotation = transform.rotation;
	}
}