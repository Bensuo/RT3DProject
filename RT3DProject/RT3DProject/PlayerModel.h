#pragma once
#include "Model.h"

namespace Rendering {
	class PlayerModel : public Model
	{
		bool jumping;
	public:
		PlayerModel();
		explicit PlayerModel(const glm::vec3& position);
		void SetUpAnimationData() override;
		~PlayerModel();
		void setAnimation(const int& state) override
		{
			m_MD2.setCurrentAnim(state);
		}
		void resetAnimation() { m_MD2.ResetAnimation(); }
		GLfloat* getVerts() { return m_MD2.getAnimVerts(); }
		int getNumVerts() { return m_MD2.getVertDataCount(); }
	};
}
