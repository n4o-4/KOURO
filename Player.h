#pragma once
#include"Kouro.h"
#include "MyMath.h"

class Player
{
public:
	void Initialize();
	void Update();
	// 描画
	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);
	void Finalize();

	// 移動
	void Move(Vector3 direction);

private:
	// オブジェクト
	std::unique_ptr<Object3d> object3d_;
	// ワールド行列
	std::unique_ptr<WorldTransform> objectTransform_;

	Vector3 position_ = { 0.0f, 0.0f, 0.0f }; // 現在の座標
	float speed_ = 0.1f; // 移動速度
};

