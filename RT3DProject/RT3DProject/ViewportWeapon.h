#pragma once
#include "Model.h"

namespace Rendering {
	class ViewportWeapon : public Model
	{
		enum WeaponState
		{
			ACTIVE,
			POW,
			IDLE1,
			IDLE2,
			PUTAWAY
		} weaponState;

		enum WeaponType
		{
			ASSAULT_RIFLE,
			BFG,
			ROCKET_LAUNCHER
		} weaponType;

	public:
		ViewportWeapon();
		explicit ViewportWeapon(const WeaponType& type, const glm::vec3& position);
		void SetUpAnimationData() override;
		~ViewportWeapon();
	};
}