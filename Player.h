#pragma once
#include "BaseCharacter.h"
class Player : public BaseCharacter
{
public: // 公開メンバ関数

	// 初期化処理
	void Initialize(Model* model) override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) override;

	// カメラを設定
	void SetCamera(BaseCamera* camera) { camera_ = camera; }

private: // 非公開メンバ関数

	// 移動
	void Move();
};

