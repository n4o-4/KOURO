#pragma once
#include "BaseCharacter.h"
#include "EnemyBullet.h"

class Enemy : public BaseCharacter, public AABBCollider
{
public: // 公開メンバ関数
	// 初期化処理
	void Initialize(Model* model) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) override;

	void SetPosition(const Vector3& position);

	void SetTarget(BaseCharacter* target) { target_ = target; }

	void SetColliderManager(ColliderManager* colliderManager) { colliderManager_ = colliderManager; }

private: // 非公開メンバ関数

	void Fire();

	// \brief  OnCollisionEnter 衝突開始時の処理

	void OnCollisionEnter(BaseCollider* other) override;

	// \brief  OnCollisionStay 衝突中の処理

	void OnCollisionStay(BaseCollider* other) override;

	// \brief  OnCollisionExit 衝突終了時の処理

	void OnCollisionExit(BaseCollider* other) override;

private: // 非公開メンバ変数

	ColliderManager* colliderManager_ = nullptr; //!< コライダーマネージャ
	
	BaseCharacter* target_ = nullptr; //!< ターゲットキャラクター

	std::vector<std::unique_ptr<EnemyBullet>> bullets_; //!< 弾のリスト

	const float kFireInterval = 2.0f; //!< 弾の発射間隔

	float fireTimer_ = 0.0f; //!< 弾の発射タイマー
};

