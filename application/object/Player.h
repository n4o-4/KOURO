#pragma once
#include"Kouro.h"
#include "MyMath.h"
#include "FollowCamera.h"
#include "PlayerMissile.h"
#include <vector>
#include "BaseObject.h" // 当たり判定用
#include "LockOn.h" 
#include "PlayerMachineGun.h"// プレイヤーのマシンガン
#include "memory"

class Player : public BaseObject {
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
	//========================================
	// 移動入力の取得
	Vector3 GetMovementInput();
	// 移動処理を更新
	void UpdateMove(Vector3 direction);
	//========================================
	// ジャンプ入力の取得
	void IsJump();
	// ジャンプ処理を更新
	void UpdateJump();
	//========================================
	// 弾の処理と更新
	void UpdateBullets();
	// 射撃
	void Shoot();
	//========================================
	// ブースト処理
	bool HandleBoost();
	//========================================
	// マシンガンの射撃処理
	void ShootMachineGun();
	//反動処理
	void ApplyRecoil();
	//playerの揺れ処理
	void ApplyShake();
	//========================================

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
	void SetFollowCamera(FollowCamera* followCamera) { followCamera_ = followCamera; }

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
	 * \brief  GetWorldTransform ワールドトランスフォームの取得
	 * \param  return
	 */
	WorldTransform* GetWorldTransform() { return objectTransform_.get(); }

	/**----------------------------------------------------------------------------
	 * \brief  GetBullets 弾の取得
	 * \return
	 */
	std::vector<std::unique_ptr<PlayerMissile>>& GetBullets() { return bullets_; }

	/**----------------------------------------------------------------------------
	 * \brief  GetHp HPの取得
	 * \return
	 */
	int GetHp() { return hp_; }

	/**----------------------------------------------------------------------------
	 * \brief  SetHp HPの設定
	 * \param  hp
	 */
	void SetHp(int hp) { hp_ = hp; }

private:
	//========================================
	// 3Dオブジェクト
	std::unique_ptr<Object3d> object3d_;
	// ワールド変換
	std::unique_ptr<WorldTransform> objectTransform_;
	//========================================
	// 弾
	std::vector<std::unique_ptr<PlayerMissile>> bullets_;// 弾のリスト
	//========================================
	// ロックオン
	//std::unique_ptr<LockOn> lockOnSystem_ = nullptr;// ロックオンシステムのポインタを追加
	LockOn* lockOnSystem_ = nullptr;  // 🔹 `GameScene` に所有させるので `LockOn*` に戻す
	//========================================
	// Hp
	int hp_ = 10;
	//========================================
	// 移動関連
	Vector3 position_ = { 0.0f, 0.0f, 0.0f };     // 位置
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };     // 現在の速度ベクトル
	Vector3 acceleration_ = { 0.0f, 0.0f, 0.0f }; // 加速度ベクトル
	float maxSpeed_ = 0.35f;                      // 最大速度
	float accelerationRate_ = 0.03f;              // 加速度係数
	float deceleration_ = 0.04f;                  // 減速度
	float friction_ = 0.02f;                      // 摩擦
	// ブースト関連
	bool isBoosting_ = false;                     // ブースト中かどうか
	float boostFactor_ = 2.2f + 8.0f;             // ブースト時の速度倍率
	float boostCooldown_ = 0.0f;                  // ブーストのクールダウン
	float maxBoostTime_ = 30.0f;                  // 最大ブースト時間
	float currentBoostTime_ = 30.0f;              // 現在のブースト残量
	float boostRecoveryRate_ = 0.15f + 8.0f;      // ブースト回復速度
	// クイックブースト関連
	bool isQuickBoosting_ = false;                // クイックブースト中かどうか
	int quickBoostFrames_ = 0;                    // クイックブーストの残りフレーム
	int maxQuickBoostFrames_ = 10;                // クイックブーストの最大フレーム数
	float quickBoostCooldown_ = 0.0f;             // クイックブーストのクールダウン
	float maxQuickBoostCooldown_ = 16.0f;         // クイックブーストの最大クールダウン
	float quickBoostConsumption_ = 16.0f;
	// 操作感度
	float airControlFactor_ = 0.8f;               // 空中操作係数
	bool isQuickTurning_ = false;                 // クイックターン中か
	float quickTurnFactor_ = 2.5f;
	//========================================
	// ジャンプ関連
	bool isJumping_ = false;  	// ジャンプ中かどうか
	bool isFloating_ = false;  	// 浮遊中かどうか
	float jumpVelocity_ = 0.2f; // 上昇速度
	float fallSpeed_ = 0.0f;  	// 下降速度
	float gravity_ = 0.16f;  	// 下降加速度
	float initialY_ = 1.0f;  	// 初期Y座標
	float floatBoost_ = 0.2f; 	// 離した瞬間の追加上昇量
	float boostVelocity_ = 0.0f;// 追加上昇速度
	float boostDecay_ = 0.02f;  // 追加上昇の減衰量
	float maxFallSpeed_ = 0.15f;// 下降速度の最大値
	//========================================
	// マシンガン関連
	std::vector<std::unique_ptr<PlayerMachineGun>> machineGunBullets_;// マシンガンの弾のリスト
	bool isShootingMachineGun_ = false;// マシンガンを撃っているか
	int machineGunCooldown_ = 0;// マシンガンのクールダウン
	//========================================
	// 反動関連
	Vector3 recoilVelocity_ = { 0.0f, 0.0f, 0.0f }; // 反動による速度
	float recoilStrength_ = 0.05f; // 反動の強さ
	float recoilDecay_ = 0.9f; // 反動の減衰率
	float shakeIntensity_ = 0.0f; // 揺れの強さ
	float shakeDecay_ = 0.9f; // 揺れの減衰率
	//========================================	

	// カメラ
	FollowCamera* followCamera_ = nullptr;

	float distinationRotateY_;
};