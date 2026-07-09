#include "BaseBullet.h"

void BaseBullet::Initialize(Kouro::LineModel* model, const Kouro::Vector3& spawnPos)
{
	// 生成と初期化
	objectLine_ = std::make_unique<Kouro::ObjectLine>();
	objectLine_->Initialize(model);

	// 生成と初期化
	worldTransform_ = std::make_unique<Kouro::WorldTransform>();
	worldTransform_->Initialize();

	worldTransform_->SetTranslate(spawnPos);

	// 速度の初期化
	velocity_ = { 0.0f,0.0f,0.0f };

	// 有効フラグの初期化
	isValid_ = true;
}

void BaseBullet::Update()
{
	///========================================
	/// ワールドトランスフォーム
	
	Kouro::Vector3 translate = worldTransform_->GetTranslate();
	worldTransform_->SetTranslate(translate + velocity_);

	// 更新
	worldTransform_->UpdateMatrix();

	lifeTimer_ += 1.0f / 60.0f;
	if (lifeTimer_ >= kLifeTime_)
	{
 		isValid_ = false; // 寿命が尽きたら無効化
	}
}

void BaseBullet::Draw()
{
	///========================================
	/// オブジェクト3D

	// 描画
	objectLine_->Draw(worldTransform_.get());
}
