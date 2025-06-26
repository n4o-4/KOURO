#include "Player.h"

void Player::Initialize(Model* model)
{
	///=========================================
	/// 親クラス
	
	// 初期化
	BaseCharacter::Initialize(model);

	worldTransform_->useQuaternion_ = false;

	worldTransform_->quaternionTransform.scale = { 50.0f,50.0f,50.0f };
	worldTransform_->transform.scale = { 50.0f,50.0f,50.0f };
}

void Player::Update()
{
	// 移動
	//Move();

	//worldTransform_->deltaRotate_ = { 0.01f,0.0f,0.0f };


	//if (moveTimer_ < kMoveTime)
	//{
	//	moveTimer_ += kDeltaTime;
	//}
	//else
	//{
	//	moveTimer_ = 0.0f;
	//}

	//float t = moveTimer_ / kMoveTime;

	//worldTransform_->transform.translate = CatmullRomPosition(controlPoints_, t);

	//float nextT = (moveTimer_ + (kDeltaTime * 100.0f)) / kMoveTime;

	//Vector3 viewTarget = CatmullRomPosition(controlPoints_, nextT);

	//// 現在位置
	//Vector3 position = worldTransform_->transform.translate;
	//// 次の位置
	//Vector3 target = viewTarget;

	//// forwardベクトル（進行方向）
	//Vector3 forward = Normalize(target - position);

	//// Y軸を上方向と仮定し、forwardからオイラー角を算出
	//float yaw = std::atan2(forward.x, forward.z);
	//float pitch = std::asin(-forward.y);
	//float roll = 0.0f; // 必要に応じて

	//worldTransform_->transform.rotate = { pitch, yaw, roll };


	///=========================================
	/// 親クラス

	// 更新
	BaseCharacter::Update();

#ifdef _DEBUG


	ImGui::Begin("Player Transform");
	
	ImGui::DragFloat3("scale", &worldTransform_->quaternionTransform.scale.x, 0.01f);
	ImGui::DragFloat4("rotate", &worldTransform_->quaternionTransform.rotate.x, 0.01f);
	ImGui::DragFloat3("translate", &worldTransform_->quaternionTransform.translate.x, 0.01f);

	ImGui::End();

#endif
}

void Player::Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	///=========================================
	/// 親クラス
	
	// 描画
	BaseCharacter::Draw(directionalLight,pointLight,spotLight);
}

void Player::Move()
{
	velocity_ = { 0.0f,0.0f,0.0f };

	if (Input::GetInstance()->PushKey(DIK_A))
	{
		velocity_.x -= 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_D))
	{
		velocity_.x += 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_W))
	{
		velocity_.z += 0.1f;
	}
	if (Input::GetInstance()->PushKey(DIK_S))
	{
		velocity_.z -= 0.1f;
	}


}
