#pragma once
#include "BaseCharacter.h"
#include "EnemyBullet.h"

#include "LineModelManager.h"

#include "EnemyState.h"
#include "ApproachState.h"

#include "ModelEdgeEmitter.h"

// \brief Enemy 敵クラス

class Enemy : public BaseCharacter, public AABBCollider
{
public: // 公開メンバ関数

	/**
	* \brief  初期化
	* \param  LineModel 線モデル
	*/
	void Initialize(Kouro::LineModel* model) override;

	// \brief Update 更新
	void Update() override;

	// \brief Draw 描画
	void Draw() override;

	/**
	* \brief SetPosition
	* \param Vector3 座標
	*/
	void SetPosition(const Kouro::Vector3& position);

	/**
	* \brief SetTarget ターゲットの設定
	* \param BaseCharacter ターゲットのポインタ
	*/
	void SetTarget(BaseCharacter* target) { target_ = target; }

	/**
	* \brief SetColliderManager コライダーマネージャーを設定する
	* \param ColliderManager コライダーマネージャーのポインタ
	*/
	void SetColliderManager(ColliderManager* colliderManager) { colliderManager_ = colliderManager; colliderManager_->AddCollider(dummy_);
	}

	/**
	* \brief SetLineModelManager ラインモデルマネージャーを設定する
	* \param LineModelManager ラインモデルマネージャーのポインタ
	*/
	void SetLineModelManager(Kouro::LineModelManager* lineModelManager) { lineModelManager_ = lineModelManager; }

	/**
	* \brief  エミッターの設定
	* \param  emitter エミッターのポインタ
	*/
	void SetEmitter(Kouro::ModelEdgeEmitter* emitter) { mEmitter_ = emitter; }

	/**
	* \brief  カメラマネージャーの設定
	* \param  cameraManager カメラマネージャーのポインタ
	*/
	void SetCameraManager(Kouro::CameraManager* cameraManager) { cameraManager_ = cameraManager; };

	/**
	* \brief  ゴールオフセットの設定
	* \param  offset ゴールオフセット
	*/
	void SetGoalOffset(const Kouro::Vector3 offset) { kGoalOffset_ = offset; basePosition_ = kGoalOffset_; };

	/**
	* \brief  SetParent 親の設定
	* \param  WorldTransform 親のワールドトランスフォームのポインタ
	*/
	void SetParent(const Kouro::WorldTransform* parent) { worldTransform_->SetParent(parent); }

	/**
	* \brief ダミーキャラクターの描画フラグを設定する
	* \param drawDummy ダミーキャラクターを描画するかどうか
	*/
	void SetDrawDummy(bool drawDummy) { drawDummy_ = drawDummy; }

	/**
	* \brief ダミーキャラクターの速度を設定する
	* \param velocity 設定する速度
	*/
	void SetDummyVelocity(const Kouro::Vector3& velocity) { dummy_->SetVelocity(velocity); }

	/**
	* \brief ダミーキャラクターの位置を設定する
	* \param position 設定する位置
	*/
	void SetDummyPosition(const Kouro::Vector3& position) { dummy_->SetPosition(position); }

	/**
	* \brief ダミーキャラクターの回転を設定する
	* \param rotate 設定する回転
	*/
	void SetDummyRotate(const Kouro::Vector3& rotate) { dummy_->SetRotate(rotate); }

	/**
	* \brief  ターゲットを取得する
	* \return BaseCharacter キャラクターのポインタ
	*/
	BaseCharacter* GetTarget() const { return target_; }

	/**
	* \brief  カメラマネージャーの取得
	* \return カメラマネージャーのポインタ
	*/
	const Kouro::CameraManager* GetCameraManager() const { return cameraManager_; }

	/**
	* \brief  ゴールオフセットの取得
	* \return ゴールオフセット
	*/
	const Kouro::Vector3& GetGoalOffset() const { return kGoalOffset_; }

	/**
	* \brief  基本位置の取得
	* \return 基本位置
	*/
	const Kouro::Vector3& GetBasePosition() const { return basePosition_; }

	/**
	* \brief  ダミーキャラクターの回転の取得
	* \return ダミーキャラクターの回転
	*/
	const Kouro::Vector3& GetDummyRotate() const { return dummy_->GetWorldTransform()->GetRotate(); }

	/**
	* \brief  ダミーキャラクターの位置の取得
	* \return ダミーキャラクターの位置
	*/
	const Kouro::Vector3& GetDummyPosition() const { return dummy_->GetWorldTransform()->GetTranslate(); }

	/**
	* \brief ChangeState 状態を変える
	* \param EnemyState 状態
	*/
	void ChangeState(std::unique_ptr<EnemyState> state);

	// \brief Fire 弾の発射処理
	void Fire();

private: // 非公開メンバ関数

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

	ColliderManager* colliderManager_ = nullptr; //!< コライダーマネージャ
	
	BaseCharacter* target_ = nullptr; //!< ターゲットキャラクター

	std::vector<std::shared_ptr<EnemyBullet>> bullets_; //!< 弾のリスト

	const float kFireInterval = 2.0f; //!< 弾の発射間隔

	float fireTimer_ = 0.0f; //!< 弾の発射タイマー

	std::unique_ptr<EnemyState> state_ = nullptr;

	Kouro::LineModelManager* lineModelManager_ = nullptr;

	Kouro::ParticleEmitter emitter_;

	Kouro::ModelEdgeEmitter* mEmitter_ = nullptr;

	Kouro::CameraManager* cameraManager_ = nullptr;

	Kouro::Vector3 kGoalOffset_ = {0.0f,0.0f,40.0f};

	Kouro::Vector3 basePosition_ = { 0.0f,0.0f,0.0f };

	std::shared_ptr<EnemyBullet> dummy_ = nullptr; //!< ダミー

	bool drawDummy_ = false;
};

