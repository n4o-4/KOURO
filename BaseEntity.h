#pragma once
// Engine
#include "Kouro.h"

class BaseEntity
{
public: // 公開メンバ関数

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	WorldTransform* GetWorldTransform() { return worldTransform_.get(); }

protected: // 派生用メンバ関数

	void DrawImGui();

public: // 派生用メンバ変数

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_;

	// 有効フラグ
	bool isAlive_ = true;
};

