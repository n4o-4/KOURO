#include "Enemy.h"

void Enemy::Initialize(Model* model)
{
	///=========================================
	/// 親クラス

	// 初期化
	BaseCharacter::Initialize(model);

	worldTransform_->useQuaternion_ = false;

	worldTransform_->quaternionTransform.scale = { 5.0f,5.0f,5.0f };
	worldTransform_->transform.scale = { 1.0f,1.0f,1.0f };

	BaseCollider::Initialize(worldTransform_.get());

	SetCollisionAttribute(0b1 << 1); // コリジョン属性を設定
	SetCollisionMask(0xffffff0f); // コリジョンマスクを設定
}

void Enemy::Update()
{
	///=========================================
	/// 親クラス

	// 更新
	BaseCharacter::Update();

	AABBCollider::Update();
}

void Enemy::Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	///=========================================
	/// 親クラス

	// 描画
	BaseCharacter::Draw(directionalLight, pointLight, spotLight);
}

void Enemy::OnCollisionEnter(BaseCollider* other)
{
}

void Enemy::OnCollisionStay(BaseCollider* other)
{
}

void Enemy::OnCollisionExit(BaseCollider* other)
{
}
