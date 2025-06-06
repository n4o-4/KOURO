#include "RailCamera.h"

void RailCamera::Initialize()
{
	BaseCamera::Initialize();

	controlPoints_ =
	{
		{0.0f,1.0f,0.0f},
		{10.0f,10.0f,20.0f},
		{0.0f,1.0f,40.0f},
		{-10.0f,10.0f,60.0f},
		{0.0f,1.0f,80.0f},
		{10.0f,10.0f,100.0f},
		{0.0f,1.0f,120.0f},
		{-10.0f,10.0f,140.0f},
		{0.0f,1.0f,160.0f},
		{10.0f,10.0f,180.0f},
		{0.0f,1.0f,200.0f}
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

	float nextT = (moveTimer_ + (kDeltaTime * 26.0f)) / kMoveTime;

	Vector3 viewTarget = CatmullRomPosition(controlPoints_, nextT);

	// ���݈ʒu
	Vector3 position = viewProjection_->transform.translate;
	// ���̈ʒu
	Vector3 target = viewTarget;

	// forward�x�N�g���i�i�s�����j
	Vector3 forward = Normalize(target - position);

	// Y����������Ɖ��肵�Aforward����I�C���[�p���Z�o
	float yaw = std::atan2(forward.x, forward.z);
	float pitch = std::asin(-forward.y);
	float roll = 0.0f; // �K�v�ɉ�����

	viewProjection_->transform.rotate = { pitch, yaw, roll };


#ifdef _DEBUG

	ImGui::Begin("RailCamera_Transform");
	ImGui::DragFloat3("Position", &viewProjection_->transform.translate.x, 0.01f);
	ImGui::DragFloat3("Rotate", &viewProjection_->transform.rotate.x, 0.01f);
	ImGui::DragFloat3("Scale", &viewProjection_->transform.scale.x, 0.01f);
	ImGui::End();

#endif

	BaseCamera::Update();
}
