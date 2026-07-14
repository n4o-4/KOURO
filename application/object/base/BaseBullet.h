#pragma once

#include "Kouro.h"
#include "BaseEntity.h"
#include "ObjectLine.h"

class BaseCollider;

// \brief BaseBullet 弾の基底クラス

class BaseBullet : public BaseEntity
{
public: // 公開メンバ関数
	
	/// \brief 仮想デストラクタ
	virtual ~BaseBullet() = default;

	/**
	 * \brief  Initialize 初期化
	 * \param  model モデル
	 */
	virtual void Initialize(Kouro::LineModel* model, const Kouro::Vector3& spawnPos);

	// \brief  更新
	virtual void Update();
	
	
	/**
	 * \brief  描画
	 * \param  directionalLight ディレクショナルライト
	 * \param  pointLight ポイントライト
	 * \param  spotLight スポットライト
	 */
	virtual void Draw();

	/**
	* \brief  存在の有効フラグを取得する
	* \return isValid_  true:有効 false:無効
	 */
	const bool IsValid() const { return isValid_; }

	/**
	 * \brief  ダメージ量取得
	 * \return damage ダメージ量
	 */
	uint32_t GetDamage() const { return damage_; }

	virtual BaseCollider* GetCollider() = 0;

	/**
	* \brief  速度を設定する
	* \param  velocity 速度
	*/
	void SetVelocity(const Kouro::Vector3& velocity) { velocity_ = velocity; }

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

