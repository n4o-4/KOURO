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

	worldTransform_->quaternionTransform.rotate.y += 0.01f;

	///=========================================
	/// 親クラス

	// 更新
	BaseCharacter::Update();
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
