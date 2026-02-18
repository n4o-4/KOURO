#pragma once
#include "BaseCharacter.h"

#include "PlayerBullet.h"

#include "LineModelManager.h"

#include "Rail.h"

#include "PostEffect.h"

/**
* \brief  QuickMoveData
* \param  bool  isQuickMoving 現在QuickMove状態かのフラグ
* \param  float duration      QuickMoveの動作時間
* \param  float coolTime      クールタイム
* \param  float actionTimer   時間測定用
*/
struct QuickMoveData
{
	bool isQuickMoving = false; //!< 現在 QuickMove 中かどうか
	float duration = 0.0f;      //!< QuickMove の継続時間
	float coolTime = 0.0f;      //!< クールタイム（再使用までの待機時間）
	float actionTimer = 0.0f;   //!< 経過時間の計測用タイマー
};

// \brief Player プレイヤークラス
class Player : public BaseCharacter, public AABBCollider
{
public: // 公開メンバ関数

	/**
	* \brief  初期化
	* \param  LineModel 線モデル
	*/
	void Initialize(Kouro::LineModel* model) override;

	// \brief Update 更新
	void Update() override;

	// \brief  Draw 描画
	void Draw() override;

	/**
	* \brief SetColliderManager コライダーマネージャーを設定する
	* \param ColliderManager コライダーマネージャーのポインタ
	*/
	void SetColliderManager(ColliderManager* colliderManager) { colliderManager_ = colliderManager; }

	/**
	* \brief  SetParentTransform 座標の親を設定する
	* \param  WorldTransform ワールドトランスフォームのポインタ
	*/
	void SetParentTransform(const Kouro::WorldTransform* parent) { worldTransform_->SetParent(parent); }

	/**
	* \brief  SetLineModelManager ラインモデルマネージャーを設定する
	* \param  LineModelManager ラインモデルマネージャーのポインタ
	*/
	void SetLineModelManager(Kouro::LineModelManager* lineModelManager) { lineModelManager_ = lineModelManager; }

	/**
	* \brief  GetIsAlive 生存フラグを取得する
	* \return bool 生存フラグ
	*/
	bool GetIsAlive();

	/**
	* \brief  QuickMoveのデータを取得する
	* \return QuickMoveDataのポインタ
	*/
	const QuickMoveData* GetQuickMoveData() { return quickMoveData_.get(); }

	/**
	* \brief  hitフラグを取得する
	* \return hitフラグ
	*/
	const bool GetIsHit() const { return isHit_; }


	const float GetFireInterval()const { return kFireInterval_; }
	const float GetFireIntervalTimer() const { return fireTimer_; }

private: // 非公開メンバ関数

	// \brief  Move 移動処理
	void Move();

	// \brief  Fire 弾を発射する
	void Fire();

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

	std::vector<std::shared_ptr<PlayerBullet>> bullets_; //!< 弾のリスト

	const float kMoveTime = 10.0f;

	float moveTimer_ = 0.0f; //!< カメラの移動時間

	std::vector<Kouro::Vector3> controlPoints_ =
	{
		{0.0f,0.0f,-20.0f},

		{0.0f,0.0f,20.0f},

		{0.0f,0.0f,480.0f},
		{15.0f,0.0f,495.0f},
		{20.0f,0.0,500.0f},

		{480.0f,0.0f,500.0f},
		{495.0f,0.0f,495.0f},
		{500.0f,0.0f,480.0f},

		{500.0f,0.0f,20.0f},
		{495.0f,0.0f,5.0f},
		{480.0f,0.0f,0.0f},

		{20.0f,0.0f,0.0f},
		{5.0f,0.0f,5.0f},
		{0.0f,0.0f,20.0f},

		{0.0f,0.0f,20.1f}, // 終了点
	};

	ColliderManager* colliderManager_ = nullptr; //!< コライダーマネージャ

	Kouro::LineModelManager* lineModelManager_ = nullptr;

	Rail rail;

	const float kFireInterval_ = 0.05f; //!< 発射間隔

	float fireTimer_ = 0.0f; //!< 発射タイマー

	std::unique_ptr<QuickMoveData> quickMoveData_ = nullptr;

	bool isHit_ = false;

	static constexpr float kDefaultScale_ = 0.5f; //!< プレイヤーのデフォルトのスケール
	static constexpr Kouro::Vector4 kDefaultColor_ = { 0.071f, 0.429f, 1.0f, 1.0f };
	static constexpr Kouro::Vector4 kHitColor_ = { 1.0f, 0.0f, 0.0f, 1.0f };
	static constexpr float kDefaultOffset_ = 15.0f;


	static constexpr float kDefaultPlayerSpeed_ = 1.4f;
	static constexpr float kBoostPlayerSpeed_ = 1.5f;

	static constexpr float kDefaultFriction_ = 0.85f;
	static constexpr float kBoostFriction_ = 0.99f;

	static constexpr float bulletSpeed_ = 5.0f;

	static constexpr float kTargetTiltAngle_ = 0.3f; // プレイヤーの傾きの目標角度

	Kouro::Vector3 targetTilt_ = { 0.0f,0.0f,0.0f }; // プレイヤーの傾きの目標角度

	Kouro::Vector2 moveLimitMin_ = { -40.0f, -25.0f }; // プレイヤーの移動制限の最小値
	Kouro::Vector2 moveLimitMax_ = { 40.0f, 25.0f }; // プレイヤーの移動制限の最大値
};

