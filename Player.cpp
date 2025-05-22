#include "Player.h"

void Player::Initialize(Model* model)
{
	///=========================================
	/// 親クラス
	
	// 初期化
	BaseCharacter::Initialize(model);

	worldTransform_->useQuaternion_ = true;
}

void Player::Update()
{
	// 移動
	Move();

	//worldTransform_->deltaRotate_ = { 0.01f,0.0f,0.0f };

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
