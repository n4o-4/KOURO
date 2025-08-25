﻿#pragma once
#include "BaseCharacter.h"

#include "PlayerBullet.h"

class Player : public BaseCharacter, public AABBCollider
{
public: // 公開メンバ関数

	// 初期化処理
	void Initialize(Model* model) override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) override;

	void SetColliderManager(ColliderManager* colliderManager) { colliderManager_ = colliderManager; }

	void SetParentTransform(WorldTransform* parent) { worldTransform_->SetParent(parent); }

private: // 非公開メンバ関数

	// \brief  Move 移動処理

	void Move();

	// \brief  Fire 弾を発射する

	void Fire();

	// \brief  OnCollisionEnter 衝突開始時の処理

	void OnCollisionEnter(BaseCollider* other) override;

	// \brief  OnCollisionStay 衝突中の処理

	void OnCollisionStay(BaseCollider* other) override;

	// \brief  OnCollisionExit 衝突終了時の処理

	void OnCollisionExit(BaseCollider* other) override;

private: // 非公開メンバ変数

	std::vector<std::shared_ptr<PlayerBullet>> bullets_; //!< 弾のリスト

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

	ColliderManager* colliderManager_ = nullptr; //!< コライダーマネージャ

private: 
	

};

