#pragma once
// Engine
#include "Kouro.h"


class BaseCharacter
{
public: // 公開メンバ関数

	// 初期化処理
	virtual void Initialize(Model* model);

	// 更新処理
	virtual void Update();

	// 描画処理
	virtual void Draw(DirectionalLight directionalLight,PointLight pointLight,SpotLight spotLight);

	// カメラを設定
	void SetCamera(BaseCamera* camera) { camera_ = camera; }

	WorldTransform* GetWorldTransform() { return worldTransform_.get(); }

protected: // 派生用メンバ関数

	void DrawImGui();

protected: // 派生用メンバ変数

	// 3Dモデル
	std::unique_ptr<Object3d> object3d_;

	// ワールドトランスフォーム
	std::unique_ptr<WorldTransform> worldTransform_;

	// 速度
	Vector3 velocity_ = { 0.0f,0.0f,0.0f };

	// 有効フラグ
	bool isActive_ = true;

	BaseCamera* camera_ = nullptr;

};

