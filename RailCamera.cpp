#include "RailCamera.h"

void RailCamera::Initialize()
{
	BaseCamera::Initialize();

	controlPoints_ =
	{
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,20.0f},
		{0.0f,0.0f,40.0f},
		{0.0f,0.0f,80.0f},
	};

}

void RailCamera::Update()
{

	if (moveTimer_ < kMoveTime)
	{
		moveTimer_ += kDeltaTime;
	}
	else
	{
		moveTimer_ = 0.0f;
	}

	float t = moveTimer_ / kMoveTime;

	viewProjection_->transform.translate = CatmullRomPosition(controlPoints_,t);

#ifdef _DEBUG

	ImGui::Begin("RailCamera_Transform");
	ImGui::DragFloat3("Position", &viewProjection_->transform.translate.x, 0.01f);
	ImGui::DragFloat3("Rotate", &viewProjection_->transform.rotate.x, 0.01f);
	ImGui::DragFloat3("Scale", &viewProjection_->transform.scale.x, 0.01f);
	ImGui::End();

#endif

	BaseCamera::Update();
}
