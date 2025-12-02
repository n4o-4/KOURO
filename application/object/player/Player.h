#pragma once
#include "BaseCharacter.h"

#include "PlayerBullet.h"

#include "LineModelManager.h"

#include "Rail.h"

// \brief Player プレイヤークラス

class Player : public BaseCharacter, public AABBCollider
{
public: // 公開メンバ関数

	/**
	* \brief  Initialzie 初期化
	* \param  LineModel 線モデル
	*/
	void Initialize(LineModel* model) override;

	// \brief Update 更新
	void Update() override;

	// \brief  Draw 描画
	void Draw() override;

	/**
	* \brief SetColliderManager コライダーマネージャーを設定する
	* \param ColliderManager コライダーマネージャーのポインタ
	*/
	void SetColliderManager(ColliderManager* colliderManager) { colliderManager_ = colliderManager; }

	/**
	* \brief  SetParentTransform 座標の親を設定する
	* \param  WorldTransform ワールドトランスフォームのポインタ
	*/
	void SetParentTransform(WorldTransform* parent) { worldTransform_->SetParent(parent); }

	/**
	* \brief  SetLineModelManager ラインモデルマネージャーを設定する
	* \param  LineModelManager ラインモデルマネージャーのポインタ
	*/
	void SetLineModelManager(LineModelManager* lineModelManager) { lineModelManager_ = lineModelManager; }

	/**
	* \brief  GetIsAlive 生存フラグを取得する
	* \return bool 生存フラグ
	*/
	bool GetIsAlive();

private: // 非公開メンバ関数

	// \brief  Move 移動処理
	void Move();

	// \brief  Fire 弾を発射する
	void Fire();

	/**
	* \brief  OnCollisionEnter 衝突開始時
	* \param  BaseCollider 他のコライダー
	*/
	void OnCollisionEnter(BaseCollider* other) override;

	/**
	* \brief  OnCollisionStay 衝突中
	* \param  BaseCollider 他のコライダー
	*/
	void OnCollisionStay(BaseCollider* other) override;

	/**
	* \brief  OnCollisionExit 衝突終了時
	* \param  BaseCollider 他のコライダー
	*/
	void OnCollisionExit(BaseCollider* other) override;

private: // 非公開メンバ変数

	std::vector<std::shared_ptr<PlayerBullet>> bullets_; //!< 弾のリスト

	const float kMoveTime = 10.0f;

	float moveTimer_ = 0.0f; //!< カメラの移動時間

	std::vector<Vector3> controlPoints_ =
	{
		{0.0f,0.0f,-20.0f},

		{0.0f,0.0f,20.0f},

		{0.0f,0.0f,480.0f},
		{15.0f,0.0f,495.0f},
		{20.0f,0.0,500.0f},

		{480.0f,0.0f,500.0f},
		{495.0f,0.0f,495.0f},
		{500.0f,0.0f,480.0f},

		{500.0f,0.0f,20.0f},
		{495.0f,0.0f,5.0f},
		{480.0f,0.0f,0.0f},

		{20.0f,0.0f,0.0f},
		{5.0f,0.0f,5.0f},
		{0.0f,0.0f,20.0f},

		{0.0f,0.0f,20.1f}, // 終了点
	};

	ColliderManager* colliderManager_ = nullptr; //!< コライダーマネージャ

	LineModelManager* lineModelManager_ = nullptr;

	Rail rail;

	const float kFireInterval_ = 0.2f; //!< 発射間隔

	float fireTimer_ = 0.0f; //!< 発射タイマー

	const float kHitInterval_ = 0.1f; //!< 被弾アクション間隔

	float hitintervalTimer_ = 0.0f; //!< 被弾アクションタイマー

private: 
	

};

