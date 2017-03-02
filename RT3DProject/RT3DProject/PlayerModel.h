#pragma once
#include "MD2.h"

class PlayerModel : public MD2
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
		CRSTAND,
		CRWALK,
		CRATTACK,
		CRPAIN,
		CRDEATH,
		DEATH1,
		DEATH2,
		DEATH3,
	};

	void SetUpAnimationData(const std::vector<int>& frameList) override;
public:
	PlayerModel();
	explicit PlayerModel(const std::string& filename);
	~PlayerModel();
	void SetUpAnimationData() override;
};

