#pragma once
// Engine
#include "Kouro.h"
#include "ObjectLine.h"
#include "BaseEntity.h"

// \brief BaseCharacter キャラクター基底クラス

class BaseCharacter : protected BaseEntity
{
public: // 公開メンバ関数

	// 初期化処理
	virtual void Initialize(LineModel* model);

	// 更新処理
	virtual void Update();

	// 描画処理
	virtual void Draw();

	WorldTransform* GetWorldTransform() { return worldTransform_.get(); }

protected: // 派生用メンバ関数

	void DrawImGui();

protected: // 派生用メンバ変数

	// ワイヤーモデル
	std::unique_ptr<ObjectLine> objectLine_ = nullptr;

	// 速度
	Vector3 velocity_ = { 0.0f,0.0f,0.0f };

	uint32_t hp_;

};

