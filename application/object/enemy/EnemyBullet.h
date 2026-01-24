#pragma once
#include "BaseBullet.h"

// \brief EnemyBullet 敵の弾クラス

class EnemyBullet : public BaseBullet, public SphereCollider
{
public: // 公開メンバ関数
	/**
	 * \brief  Initialize 初期化
	 * \param  model モデル
	 */
	void Initialize(Kouro::LineModel* model, Kouro::Vector3 spawnPos) override;

	// \brief  Update 更新
	void Update() override;

	// \brief Draw 描画
	void Draw() override;

	///================================================================================
	///                                        setter

	/**
	 * \brief  SetVelocity 速度を設定
	 * \param  velocity 速度
	*/
	void SetVelocity(const Kouro::Vector3& velocity) { velocity_ = velocity; }

private: // 非公開メンバ関数

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

	std::unique_ptr<Kouro::ParticleEmitter> emitter_; //!< パーティクルエミッター
};

