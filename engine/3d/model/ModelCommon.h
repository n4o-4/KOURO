#pragma once
#include "DirectXCommon.h"

// \brief ModelCommon 3dモデル汎用クラス 汎用の処理を行う

class ModelCommon
{
public:
	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	DirectXCommon* GetDxCommon() { return dxCommon_; }

private:
	DirectXCommon* dxCommon_;
};

