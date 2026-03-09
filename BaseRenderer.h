#pragma once

#include "BaseScene.h"

namespace Kouro
{
	class BaseRenderer
	{
	public:
		virtual void Initialize() = 0;

		/**
		* \brief  Render 描画
		* \param  scene : 描画するシーンのポインタ
		*/
		virtual void Render(BaseScene* scene) = 0;
	};
}