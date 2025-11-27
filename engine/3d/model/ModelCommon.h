#pragma once
#include "DirectXCommon.h"

// \brief ModelCommon
// モデル共通情報を管理するクラス。
// DirectXCommonの参照を保持し、モデル描画時に共有リソースとして利用する。

class ModelCommon
{
public:
	// 初期化
	void Initialize(DirectXCommon* dxCommon);

	DirectXCommon* GetDxCommon() { return dxCommon_; }

private:
	DirectXCommon* dxCommon_;
};

