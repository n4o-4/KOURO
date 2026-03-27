#pragma once


#include "GpuResourceUtils.h"
#include "BaseScene.h"

namespace Kouro
{
	/// \brief BaseRenderer 描画の基底クラス
	class BaseRenderer
	{
	public:

		/// \brief デストラクタ
		virtual~BaseRenderer() = default;

		/**
		* \brief  初期化
		* \param  gpuResourceUtils : GpuResourceUtilsのポインタ
		*/
		virtual void Initialize(ID3D12GraphicsCommandList* cmdList, const GpuResourceUtils* gpuResourceUtils) = 0;

		/**
		* \brief  描画
		* \param  scene : 描画するシーンのポインタ
		*/
		virtual void Render(BaseScene* scene) = 0;
	};
}