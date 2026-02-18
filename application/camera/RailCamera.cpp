#include "RailCamera.h"

void RailCamera::Initialize()
{
	// 基底クラスの初期化
	Kouro::BaseCamera::Initialize();

	// ワールド変形情報の生成と初期化
	worldTransform_ = std::make_unique<Kouro::WorldTransform>();
	worldTransform_->Initialize();

	worldTransform_->SetTranslate({ 0.0f, 5.0f, 0.0f });
	//worldTransform_->useQuaternion_ = false;
}

void RailCamera::Update()
{
	if(target_)
	{
		Kouro::Vector2 playerPosXY = { target_->GetTranslate().x, target_->GetTranslate().y };

		followRate_;

		Kouro::Vector3 position = worldTransform_->GetTranslate();

		position.x = playerPosXY.x * followRate_;
		position.y = playerPosXY.y * followRate_;

		shakeOffset_ = position;
	}

	// 基底クラスの更新
    Kouro::BaseCamera::Update();
}

void RailCamera::SetRail(const Rail& rail, float moveTime)
{
    railFollower_ = std::make_unique<RailFollower>();
    railFollower_->Initialize(rail, moveTime);

    worldTransform_->SetParent(&railFollower_->GetWorldTransform());
}
