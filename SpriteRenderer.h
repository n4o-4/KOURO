#pragma once
#include "BaseRenderer.h"

namespace Kouro
{
	class SpriteRenderer : public BaseRenderer
	{
		void Initialize() override;

		void Render(BaseScene* scene) override;
	};


}


