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

	const float kMoveTime = 10.0f;

	float moveTimer_ = 0.0f; //!< カメラの移動時間

	std::vector<Vector3> controlPoints_ =
	{
		{0.0f,0.0f,0.0f},
		{10.0f,0.0f,20.0f},
		{0.0f,0.0f,40.0f},
		{-10.0f,0.0f,60.0f},
		{0.0f,0.0f,80.0f},
		{10.0f,0.0f,100.0f},
		{0.0f,0.0f,120.0f},
		{-10.0f,0.0f,140.0f},
		{0.0f,0.0f,160.0f},
		{10.0f,0.0f,180.0f},
		{0.0f,0.0f,200.0f}
	};
};

