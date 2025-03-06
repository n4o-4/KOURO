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
	 * \brief  SetHp HPを設定
	 * \param  hp HP
	 */
	void SetHp(const int hp) { hp_ = hp; }

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

	/**----------------------------------------------------------------------------
	 * \brief  GetHp HPを取得
	 * \return HP
	 */
	const int GetHp() const { return hp_; }

	///--------------------------------------------------------------
	/// メンバ変数
private:
	//========================================
	// モデル
	std::unique_ptr<Object3d> model_ = nullptr;
	//ワールド変換
	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	//HP
	int hp_ = 1;

	float speed_ = 0.05f;   // 移動速度
	float minX_ = -100.0f;    // 左の限界
	float maxX_ = 100.0f;     // 右の限界
	int direction_ = 1;     // 移動方向 (1:右, -1:左)
};

