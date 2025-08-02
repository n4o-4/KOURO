#include "RailCamera.h"

void RailCamera::Initialize()
{
	BaseCamera::Initialize();

	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->useQuaternion_ = false;

	
	controlPoints_ =
	{
		/*{0.0f,1.0f,0.0f},
		{10.0f,10.0f,20.0f},
		{0.0f,1.0f,40.0f},
		{-10.0f,10.0f,60.0f},
		{0.0f,1.0f,80.0f},
		{10.0f,10.0f,100.0f},
		{0.0f,1.0f,120.0f},
		{-10.0f,10.0f,140.0f},
		{0.0f,1.0f,160.0f},
		{10.0f,10.0f,180.0f},
		{0.0f,1.0f,200.0f}*/
		  {   0.0f, 1.0f,    0.0f },   // 開始点
	{   0.0f, 1.0f,  100.0f },   // Z+方向
	{ 100.0f, 1.0f,  100.0f },   // X+方向
	{ 100.0f, 1.0f,    0.0f },   // Z-方向
	{   0.0f, 1.0f,    0.0f },   // X-方向（戻ってくる）

	// 以下は Catmull-Rom のループ補完用
	{   0.0f, 1.0f,  100.0f },
	{ 100.0f, 1.0f,  100.0f }
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

	// 現在位置
	Vector3 position = viewProjection_->transform.translate;
	// 次の位置
	Vector3 target = viewTarget;

	// forwardベクトル（進行方向）
	Vector3 forward = Normalize(target - position);

	// forwardからオイラー角を算出
	float yaw = std::atan2(forward.x, forward.z);
	float pitch = std::asin(-forward.y);
	float roll = 0.0f; // 必要に応じて

	viewProjection_->transform.rotate = { pitch, yaw, roll };

	worldTransform_->transform = viewProjection_->transform;

	worldTransform_->UpdateMatrix();

#ifdef _DEBUG

	ImGui::Begin("RailCamera_Transform");
	ImGui::DragFloat3("Position", &viewProjection_->transform.translate.x, 0.01f);
	ImGui::DragFloat3("Rotate", &viewProjection_->transform.rotate.x, 0.01f);
	ImGui::DragFloat3("Scale", &viewProjection_->transform.scale.x, 0.01f);
	ImGui::End();

#endif

	BaseCamera::Update();
}
