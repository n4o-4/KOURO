#pragma once
#include "DirectXCommon.h"

// \brief ModelCommon
// モデル共通情報を管理するクラス。
// DirectXCommonの参照を保持し、モデル描画時に共有リソースとして利用する。

namespace Kouro
{
	class ModelCommon
	{
	public:

		/**
		* \brief  初期化
		* \param  dxCommon DirectX共通クラスへのポインタ
		*/
		void Initialize(DirectXCommon* dxCommon);

		/**
		* \brief  directXCommon取得
		* \return directXCommon
		*/
		DirectXCommon* GetDxCommon() { return dxCommon_; }

	private:
		DirectXCommon* dxCommon_;
	};
}