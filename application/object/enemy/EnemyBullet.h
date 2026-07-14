#pragma once
#include "BaseBullet.h"

class SphereCollider;

// \brief EnemyBullet 敵の弾クラス

class EnemyBullet : public BaseBullet, public SphereCollider
{
public: // 公開メンバ関数
	/**
	 * \brief  初期化
	 * \param  model モデル
	 */
	void Initialize(Kouro::LineModel* model, const Kouro::Vector3& spawnPos) override;

	// \brief  更新
	void Update() override;

	// \brief 描画
	void Draw() override;

	///================================================================================
	///                                        setter

	/**
	* \brief  回転を設定
	* \param  rotate 回転
	*/
	void SetRotate(const Kouro::Vector3& rotate) { worldTransform_->SetRotate(rotate); }

	BaseCollider* GetCollider() override { return this; }

private: // 非公開メンバ関数

	/**
	* \brief  衝突開始時
	* \param  BaseCollider 他のコライダー
	*/
	void OnCollisionEnter(BaseCollider* other) override;

	/**
	* \brief  衝突中
	* \param  BaseCollider 他のコライダー
	*/
	void OnCollisionStay(BaseCollider* other) override;

	/**
	* \brief  衝突終了時
	* \param  BaseCollider 他のコライダー
	*/
	void OnCollisionExit(BaseCollider* other) override;

private: // 非公開メンバ変数

	std::unique_ptr<Kouro::ParticleEmitter> emitter_; //!< パーティクルエミッター
};

