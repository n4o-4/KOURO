#pragma once
#include "Kouro.h"
#include "BaseObject.h"
#include <iostream>

///=============================================================================
///						エネミークラス
class Enemy : public BaseObject {
	///--------------------------------------------------------------
	///						 メンバ関数 
public:
	// @brief 初期化
	void Initialize();
	// @brief 更新
	void Update();
	/**----------------------------------------------------------------------------
	 * \brief  Draw 描画
	 * \param  viewProjection ビュー射影行列
	 * \param  directionalLight ディレクショナルライト
	 * \param  pointLight ポイントライト
	 * \param  spotLight スポットライト
	 */
	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

	///--------------------------------------------------------------
	///						 当たり判定
private:
	/// \brief 衝突開始時の処理
	void OnCollisionEnter(BaseObject* other) override;

	/// \brief 衝突継続時の処理
	void OnCollisionStay(BaseObject* other) override;

	/// \brief 衝突終了時の処理
	void OnCollisionExit(BaseObject* other) override;

	///--------------------------------------------------------------
	///						 入出力関数
public:
	/**----------------------------------------------------------------------------
	 * \brief  SetPosition 位置を設定
	 * \param  position 位置
	 */
	void SetPosition(const Vector3& position) { worldTransform_->transform.translate = position; }

	/**----------------------------------------------------------------------------
	 * \brief  GetPosition 位置を取得
	 * \return 位置
	 */
	const Vector3& GetPosition() const {
		std::cout << "Enemy Position Retrieved: x=" << worldTransform_->transform.translate.x
			<< ", y=" << worldTransform_->transform.translate.y
			<< ", z=" << worldTransform_->transform.translate.z << std::endl;
		return worldTransform_->transform.translate;
	}

	///--------------------------------------------------------------
	/// メンバ変数
private:
	//========================================
	// モデル
	std::unique_ptr<Object3d> model_ = nullptr;
	//ワールド変換
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	//========================================
	// 移動関連
	Vector3 position_ = { 0.0f, 0.0f, 0.0f };     // 位置
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };     // 現在の速度ベクトル
	Vector3 acceleration_ = { 0.0f, 0.0f, 0.0f }; // 加速度ベクトル
	float maxSpeed_ = 0.35f;                      // 最大速度
	float accelerationRate_ = 0.03f;              // 加速度係数
	float deceleration_ = 0.04f;                  // 減速度
	float friction_ = 0.02f;                      // 摩擦

};

