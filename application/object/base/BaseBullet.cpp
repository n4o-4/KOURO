#include "BaseBullet.h"

void BaseBullet::Initialize(LineModel* model, Vector3 spawnPos)
{
	///========================================
	/// オブジェクト3D

	// 生成と初期化
	objectLine_ = std::make_unique<ObjectLine>();
	objectLine_->Initialize(model);

	///========================================
	/// ワールドトランスフォーム

	// 生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();


	worldTransform_->useQuaternion_ = false;
	// 初期値の設定

	worldTransform_->transform.translate = spawnPos;

	///========================================
	/// その他変数

	// 速度の初期化
	velocity_ = { 0.0f,0.0f,0.0f };

	// 有効フラグの初期化
	isAlive_ = true;
}

void BaseBullet::Update()
{
	///========================================
	/// ワールドトランスフォーム

	// クォータニオン／行列どちらの変換を使うかで移動先を更新
	if (worldTransform_->useQuaternion_)
	{
		worldTransform_->quaternionTransform.translate += velocity_;
	}
	else
	{
		worldTransform_->transform.translate += velocity_;
	}

	// 更新
	worldTransform_->UpdateMatrix();

	lifeTimer_ += 1.0f / 60.0f;
	if (lifeTimer_ >= kLifeTime_)
	{
 		isAlive_ = false; // 寿命が尽きたら無効化
	}
}

void BaseBullet::Draw()
{
	///========================================
	/// オブジェクト3D

	// 描画
	objectLine_->Draw(worldTransform_.get());
}
