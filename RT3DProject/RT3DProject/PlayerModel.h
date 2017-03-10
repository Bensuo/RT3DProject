#pragma once
#include "Model.h"

namespace Rendering {
	class PlayerModel : public Model
	{
	public:
		PlayerModel();
		explicit PlayerModel(const glm::vec3& position);
		void SetUpAnimationData() override;
		~PlayerModel();
	};
}