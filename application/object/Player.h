#pragma once
#include"Kouro.h"
#include "MyMath.h"
#include "FollowCamera.h"
#include "PlayerBullet.h"
#include <vector>
#include "BaseObject.h" // 当たり判定用
#include "LockOn.h" 
#include "memory"

class Player : public BaseObject
{
public:
	///--------------------------------------------------------------
	///							メンバ関数
	/// @brief 初期化
	void Initialize();
	/// @brief 更新
	void Update();
	/**----------------------------------------------------------------------------
	 * \brief  Draw 描画
	 * \param  viewProjection ビュー射影行列
	 * \param  directionalLight ディレクショナルライト
	 * \param  pointLight ポイントライト
	 * \param  spotLight スポットライト
	 */
	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);
	/// @brief 終了処理
	void Finalize();
	// ImGui描画
	void DrawImGui();

	///--------------------------------------------------------------
	///							静的メンバ関数
private:
	/**----------------------------------------------------------------------------
	 * \brief  Move 移動
	 * \param  direction 移動方向
	 */
	void Move(Vector3 direction);
	/**----------------------------------------------------------------------------
	 * \brief  Jump ジャンプ
	 */
	void Jump();
	/**----------------------------------------------------------------------------
	 * \brief  Shoot 弾を撃つ
	 */
	void Shoot();

	///--------------------------------------------------------------
	///						 当たり判定
	/// \brief 衝突開始時の処理
	void OnCollisionEnter(BaseObject* other) override;

	/// \brief 衝突継続時の処理
	void OnCollisionStay(BaseObject* other) override;

	/// \brief 衝突終了時の処理
	void OnCollisionExit(BaseObject* other) override;

	///--------------------------------------------------------------
	///							入出力関数
public:
	/**----------------------------------------------------------------------------
	 * \brief  SetCamera カメラをセット
	 * \param  camera カメラ
	 */
	void SetCamera(Camera* camera) { object3d_->SetCamera(camera); }
	/**----------------------------------------------------------------------------
	 * \brief  SetFollowCamera 追従カメラをセット
	 * \param  camera カメラのセット
	 */
	void SetFollowCamera(FollowCamera* camera) { followCamera_ = camera; }

	/**----------------------------------------------------------------------------
	 * \brief  SetLockOnSystem ロックオンシステムをセット
	 * \param  lockOn ロックオンシステム
	 */
	void SetLockOnSystem(LockOn* lockOnSystem) {
		lockOnSystem_ = lockOnSystem;  // 🔹 `std::move()` を使わずポインタとして設定
	}

	/**----------------------------------------------------------------------------
	 * \brief  GetPosition 位置の取得
	 * \return
	 */
	Vector3 GetPosition() { return objectTransform_->transform.translate; }



	/**----------------------------------------------------------------------------
	 * \brief  GetBullets 弾の取得
	 * \return
	 */
	std::vector<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }


private:
	//========================================
	// 3Dオブジェクト
	std::unique_ptr<Object3d> object3d_;
	// ワールド変換
	std::unique_ptr<WorldTransform> objectTransform_;
	//========================================
	// 弾
	std::vector<std::unique_ptr<PlayerBullet>> bullets_;// 弾のリスト
	//========================================
	// ロックオン
	//std::unique_ptr<LockOn> lockOnSystem_ = nullptr;// ロックオンシステムのポインタを追加
	LockOn* lockOnSystem_ = nullptr;  // 🔹 `GameScene` に所有させるので `LockOn*` に戻す
	//========================================
	// 移動関連
	Vector3 position_ = { 0.0f, 0.0f, 0.0f };
	// 移動速度
	float speed_ = 0.1f;
	//========================================
	// ジャンプ関連
	bool isJumping_ = false;  	// ジャンプ中かどうか
	bool isFloating_ = false;  	// 浮遊中かどうか
	float jumpVelocity_ = 0.2f; // 上昇速度
	float fallSpeed_ = 0.0f;  	// 下降速度
	float gravity_ = 0.005f;  	// 下降加速度
	float initialY_ = 1.0f;  	// 初期Y座標
	float floatBoost_ = 0.2f; 	// 離した瞬間の追加上昇量
	float boostVelocity_ = 0.0f;// 追加上昇速度
	float boostDecay_ = 0.02f;  // 追加上昇の減衰量
	float maxFallSpeed_ = 0.15f;// 下降速度の最大値
	//========================================
	// カメラ
	FollowCamera* followCamera_ = nullptr;
};