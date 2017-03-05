#pragma once
#include "Model.h"

namespace Rendering {
	class PlayerModel : public Model
	{
		enum PlayerState
		{
			STAND,
			RUN,
			ATTACK,
			PAIN1,
			PAIN2,
			PAIN3,
			JUMP,
			FLIP,
			SALUTE,
			TAUNT,
			WAVE,
			POINT,
			CRSTND,
			CRWALK,
			CRATTAK,
			CRPAIN,
			CRDEATH,
			DEATH1,
			DEATH2,
			DEATH3
		};

	public:
		PlayerModel();
		explicit PlayerModel(const glm::vec3& position);
		void SetUpAnimationData() override;
		~PlayerModel();
	};
}