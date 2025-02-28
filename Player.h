#pragma once
#include"Kouro.h"
#include "MyMath.h"

class Player
{
public:
	void Initialize();
	void Update();
	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);
	void Finalize();

	void Move(Vector3 direction);// 移動処理
	void Jump();// ジャンプ処理
	//追従カメラ
	void SetCamera(Camera* camera) { object3d_->SetCamera(camera); }

private:
	std::unique_ptr<Object3d> object3d_;// 3Dオブジェクト
	std::unique_ptr<WorldTransform> objectTransform_;// 3Dオブジェクトの変換情報

	Vector3 position_ = { 0.0f, 0.0f, 0.0f };
	float speed_ = 0.1f;

	// ジャンプ関連
	bool isJumping_ = false;  // ジャンプ中かどうか
	bool isFloating_ = false;  // 浮遊中かどうか
	float jumpVelocity_ = 0.2f;  // 上昇速度
	float fallSpeed_ = 0.0f;  // 下降速度
	float gravity_ = 0.005f;  // 下降加速度
	float initialY_ = -2.0f;  // 初期Y座標
	float floatBoost_ = 0.1f; // 離した瞬間の追加上昇量
	float maxFallSpeed_ = 0.15f; // 下降速度の最大値
};
