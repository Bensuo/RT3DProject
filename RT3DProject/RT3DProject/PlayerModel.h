#pragma once
#include "Model.h"

namespace Rendering {
	class PlayerModel : public Model
	{
		bool jumping;
	public:
		PlayerModel();
		explicit PlayerModel(const glm::vec3& position);
		void setUpAnimationData() override;
		~PlayerModel();
		void setAnimation(const int& state) override
		{
			model.setCurrentAnim(state);
		}
		void resetAnimation() { model.resetAnimation(); }
		const GLfloat* getVerts() const { return model.getAnimVerts(); }
		int getNumVerts() const { return model.getVertDataCount(); }
	};
}