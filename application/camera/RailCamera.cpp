#include "RailCamera.h"

void RailCamera::Initialize()
{
	// 基底クラスの初期化
	BaseCamera::Initialize();

	// ワールド変形情報の生成と初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->useQuaternion_ = false;
}

void RailCamera::Update()
{
	// 基底クラスの更新
    BaseCamera::Update();
}

void RailCamera::SetRail(const Rail& rail, float moveTime)
{
    railFollower_ = std::make_unique<RailFollower>();
    railFollower_->Initialize(rail, moveTime);

    worldTransform_->SetParent(&railFollower_->GetWorldTransform());
}
