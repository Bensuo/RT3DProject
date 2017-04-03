#pragma once
#include "Model.h"

namespace Rendering {
	class ViewportWeapon : public Model
	{
		enum WeaponType
		{
			ASSAULT_RIFLE,
			BFG,
			ROCKET_LAUNCHER
		} weaponType;

	public:
		ViewportWeapon();
		explicit ViewportWeapon(const WeaponType& type, const glm::vec3& position);
		void setUpAnimationData() override;
		~ViewportWeapon();
		void setTransform(const Transform& transform) override;
		void setAnimation(const int& state) override
		{
			model.setCurrentAnim(state);
		}
	};
}