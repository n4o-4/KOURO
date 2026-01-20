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
    if (!isMove_)
    {
        return;
    } 

    //railFollower_->Update(1.0f / 60.0f);

    worldTransform_->UpdateMatrix();

    //viewProjection_->matView_ = Inverse(worldTransform_->matWorld_);

    viewProjection_->TransferMatrix();

    //viewProjection_->transform = worldTransform_->transform;
    //viewProjection_->transform.rotate = worldTransform_->transform.rotate;

#ifdef _DEBUG
    /*ImGui::Begin("RailCamera_Transform");
    ImGui::DragFloat3("Position", &viewProjection_->transform.translate.x, 0.01f);
    ImGui::DragFloat3("Rotate", &viewProjection_->transform.rotate.x, 0.01f);
    ImGui::DragFloat3("Scale", &viewProjection_->transform.scale.x, 0.01f);
    ImGui::End();*/
#endif

	// 基底クラスの更新
    BaseCamera::Update();
}

void RailCamera::SetRail(const Rail& rail, float moveTime)
{
    railFollower_ = std::make_unique<RailFollower>();
    railFollower_->Initialize(rail, moveTime);

    worldTransform_->SetParent(&railFollower_->GetWorldTransform());
}
