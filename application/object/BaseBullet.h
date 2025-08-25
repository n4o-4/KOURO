#pragma once

#include "Kouro.h"
#include "BaseEntity.h"

class BaseBullet : public BaseEntity
{
public: // 公開メンバ関数
	
	/**
	 * \brief  Initialize 初期化
	 * \param  model モデル
	 */

	virtual void Initialize(Model* model, Vector3 spawnPos);

	// \brief  Update 更新

	virtual void Update();
	
	
	/**
	 * \brief  Draw 描画
	 * \param  directionalLight ディレクショナルライト
	 * \param  pointLight ポイントライト
	 * \param  spotLight スポットライト
	 */

	virtual void Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

	/**
	 * \brief  GetIsAlive
	 * \return	isAlive 生存フラグ
	 */

	bool GetIsAlive() const { return isAlive_; }

protected: // 派生用メンバ変数

	// 3Dモデル
	std::unique_ptr<Object3d> object3d_;

	// 速度
	Vector3 velocity_ = { 0.0f,0.0f,0.0f };

	BaseCamera* camera_ = nullptr;

	// 寿命定数
	const float kLifeTime_ = 5.0f; //!< 弾の寿命

	float lifeTimer_ = 0.0f; //!< 弾の寿命タイマー
};

