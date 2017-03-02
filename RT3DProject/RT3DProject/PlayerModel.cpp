#include "PlayerModel.h"

PlayerModel::PlayerModel()
{
	PlayerModel::SetUpAnimationData();
	currentAnim = 0;
	currentFrame = 0;
	nextFrame = 1;
	interp = 0.0f;
}

PlayerModel::PlayerModel(const std::string& filename)
{
	PlayerModel::SetUpAnimationData();
	ReadMD2Model(filename);
	currentAnim = 0;
	currentFrame = 0;
	nextFrame = 1;
	interp = 0.0f;
}

PlayerModel::~PlayerModel()
{
	FreeModel();
	int i;
	for (i = 0; i<mdl.header.num_frames; ++i) {
		delete[] vertData[i];
	}
	delete[] animVerts;
}

void PlayerModel::SetUpAnimationData()
{
	//default Quake II player model states and frame counts
	animFrameList = {
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
}

void PlayerModel::SetUpAnimationData(const std::vector<int>& frameList)
{
	animFrameList = frameList;
}
