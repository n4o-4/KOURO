#include "Player.h"

void Player::Initialize(Model* model)
{
	///=========================================
	/// �e�N���X
	
	// ������
	BaseCharacter::Initialize(model);
}

void Player::Update()
{
	///=========================================
	/// �e�N���X

	// �X�V
	BaseCharacter::Update();
}

void Player::Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	///=========================================
	/// �e�N���X
	
	// �`��
	BaseCharacter::Draw(directionalLight,pointLight,spotLight);
}
