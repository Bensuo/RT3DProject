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
			if(state == 6)
			{
				jumping = true;
				m_MD2.setCurrentAnim(state);
			}
			else
			{
				auto jumpEnd = m_MD2.GetAnimFrames()[6 * 2 + 1] ;
				if (jumping == true && getCurrentFrame() == jumpEnd)
				{
					jumping = false;
				}
				else if (jumping == true && getCurrentFrame() != jumpEnd)
				{
					jumping = true;
				}
				else
				{
					m_MD2.setCurrentAnim(state);
				}
			}
		}
	};
}
