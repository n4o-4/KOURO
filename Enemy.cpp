#include "Enemy.h"

void Enemy::Initialize(Model* model)
{
	///=========================================
	/// �e�N���X

	// ������
	BaseCharacter::Initialize(model);

	worldTransform_->useQuaternion_ = false;

	worldTransform_->quaternionTransform.scale = { 5.0f,5.0f,5.0f };
	worldTransform_->transform.scale = { 1.0f,1.0f,1.0f };
}

void Enemy::Update()
{
	///=========================================
	/// �e�N���X

	// �X�V
	BaseCharacter::Update();
}

void Enemy::Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	///=========================================
	/// �e�N���X

	// �`��
	BaseCharacter::Draw(directionalLight, pointLight, spotLight);
}
