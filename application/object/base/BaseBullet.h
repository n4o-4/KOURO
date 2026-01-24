#pragma once

#include "Kouro.h"
#include "BaseEntity.h"
#include "ObjectLine.h"

// \brief BaseBullet 弾の基底クラス

class BaseBullet : public BaseEntity
{
public: // 公開メンバ関数
	
	/**
	 * \brief  Initialize 初期化
	 * \param  model モデル
	 */
	virtual void Initialize(Kouro::LineModel* model, Kouro::Vector3 spawnPos);

	// \brief  Update 更新
	virtual void Update();
	
	
	/**
	 * \brief  Draw 描画
	 * \param  directionalLight ディレクショナルライト
	 * \param  pointLight ポイントライト
	 * \param  spotLight スポットライト
	 */
	virtual void Draw();

	/**
	 * \brief  GetIsAlive
	 * \return isAlive 生存フラグ
	 */
	bool GetIsAlive() const { return isAlive_; }

	/**
	 * \brief  GetDamage ダメージ量取得
	 * \return damage ダメージ量
	 */
	uint32_t GetDamage() const { return damage_; }

protected: // 派生用メンバ変数

	// 3Dモデル
	std::unique_ptr<Kouro::ObjectLine> objectLine_;

	// 速度
	Kouro::Vector3 velocity_ = { 0.0f,0.0f,0.0f };

	Kouro::CameraManager* cameraManager_ = nullptr;

	// 寿命定数
	const float kLifeTime_ = 5.0f; //!< 弾の寿命

	float lifeTimer_ = 0.0f; //!< 弾の寿命タイマー

	uint32_t damage_ = 1; //!< ダメージ量
};

