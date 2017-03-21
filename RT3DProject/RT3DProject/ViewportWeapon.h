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
		void SetUpAnimationData() override;
		~ViewportWeapon();
		virtual void setTransform(const Transform& transform) override;
	};
}