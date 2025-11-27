#pragma once
#include "BaseBullet.h"

// \brief PlayerBullet プレイヤーの弾クラス

class PlayerBullet : public BaseBullet, public SphereCollider
{
public: // 公開メンバ関数
	/**
	 * \brief  Initialize 初期化
	 * \param  model モデル
	 */

	void Initialize(LineModel* model,Vector3 spawnPos) override;

	// \brief  Update 更新

	void Update() override;

	/**
	 * \brief  Draw 描画
	 * \param  directionalLight ディレクショナルライト
	 * \param  pointLight ポイントライト
	 * \param  spotLight スポットライト
	 */

	void Draw() override;

	 ///================================================================================
	 ///                                        setter

	/**
	 * \brief  SetCamera カメラを設定
	 * \param  camera カメラ
	*/

	void SetCameraManager(CameraManager* cameraManager) { cameraManager_ = cameraManager; }

	/**
	 * \brief  SetVelocity 速度を設定
	 * \param  velocity 速度
	*/

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

private: // 非公開メンバ関数

	// \brief  OnCollisionEnter 衝突開始時の処理

	void OnCollisionEnter(BaseCollider* other) override;

	// \brief  OnCollisionStay 衝突中の処理

	void OnCollisionStay(BaseCollider* other) override;

	// \brief  OnCollisionExit 衝突終了時の処理

	void OnCollisionExit(BaseCollider* other) override;

private: // 非公開メンバ変数

	std::unique_ptr<ParticleEmitter> emitter_; //!< パーティクルエミッター

};

