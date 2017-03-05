#include "PlayerModel.h"

namespace Rendering {

	PlayerModel::PlayerModel()
	{
		this->position = glm::vec3();
		PlayerModel::SetUpAnimationData();
	}

	PlayerModel::PlayerModel(const glm::vec3& position)
	{
		this->position = position;
		PlayerModel::SetUpAnimationData();
	}

	void PlayerModel::SetUpAnimationData()
	{
		std::vector<int> animFrameList = {
			0, 39,    //stand
			40, 45,   //run
			46, 53,   //attack
			54, 57,   //pain1
			58, 61,   //pain2
			62, 65,   //pain3
			66, 71,   //jump
			72, 83,   //flip
			84, 94,   //salute
			95, 111,  //taunt
			112, 122, //wave
			123, 134, //point
			135, 153, //crstnd
			154, 159, //crwalk
			160, 168, //crattak
			169, 172, //crpain
			173, 177, //crdeath
			178, 183, //death1
			184, 189, //death2
			190, 196  //death3
		};

		m_MD2.SetUpAnimationData(animFrameList);
	}

	PlayerModel::~PlayerModel()
	{
	}
}