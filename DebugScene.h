#pragma once
#include "BaseScene.h"

/// \brief DebugScene デバッグ用のシーン。ゲームプレイに関係ない描画や処理を行うためのシーン。

namespace Kouro
{
	class DebugScene : public BaseScene
	{
	public:

		/**
		* \brief  初期化
		\ \param  context : 
		*/
		void Initialize(EngineContext& context) override;
		
		/**
		* \brief  更新
		*/
		void Update() override;
	};
}


