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
