#include "ViewportWeapon.h"

namespace Rendering {

	ViewportWeapon::ViewportWeapon()
	{
		weaponType = ASSAULT_RIFLE;

		ViewportWeapon::SetUpAnimationData();
	}

	ViewportWeapon::ViewportWeapon(const WeaponType& type, const glm::vec3& position)
	{
		weaponType = type;
		this->transform.position = position;
		ViewportWeapon::SetUpAnimationData();
	}

	void ViewportWeapon::SetUpAnimationData()
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
		}

		m_MD2.SetUpAnimationData(animFrameList);
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