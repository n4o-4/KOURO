#include "Enemy.h"
#include "algorithm"

void Enemy::Initialize(Model* model)
{
	///=========================================
	/// 親クラス

	// 初期化
	BaseCharacter::Initialize(model);

	worldTransform_->useQuaternion_ = false;

	worldTransform_->quaternionTransform.scale = { 5.0f,5.0f,5.0f };
	worldTransform_->transform.scale = { 1.0f,1.0f,1.0f };

	AABBCollider::Initialize(worldTransform_.get());

	SetCollisionAttribute(0b1 << 1); // コリジョン属性を設定
	SetCollisionMask(0b1); // コリジョンマスクを設定

	fireTimer_ = 0.0f; // 弾の発射タイマー初期化
}

void Enemy::Update()
{
	if (fireTimer_ >= kFireInterval)
	{
		Fire();
		fireTimer_ = 0.0f; // タイマーをリセット
	}
	else
	{
		fireTimer_ += kDeltaTime; // タイマーを更新
	}

	for (auto& bullet : bullets_)
	{
		bullet->Update();
	}

	///=========================================
	/// 親クラス

	// 更新
	BaseCharacter::Update();

	AABBCollider::Update();
}

void Enemy::Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	for (auto& bullet : bullets_)
	{
		bullet->Draw(directionalLight,pointLight,spotLight);
	}

	///=========================================
	/// 親クラス

	// 描画
	BaseCharacter::Draw(directionalLight, pointLight, spotLight);
}

void Enemy::SetPosition(const Vector3& position)
{
	if (worldTransform_->useQuaternion_) {
		worldTransform_->quaternionTransform.translate = position;
	}
	else {
		worldTransform_->transform.translate = position;
	}
	worldTransform_->UpdateMatrix();
}

void Enemy::Fire()
{
	// 弾の生成
	std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();

	// 初期化
	bullet->Initialize(ModelManager::GetInstance()->FindModel("playerbullet/playerbullet.obj"), { worldTransform_->matWorld_.m[3][0],worldTransform_->matWorld_.m[3][1],worldTransform_->matWorld_.m[3][2] });

	bullet->SetCamera(camera_);

	Matrix4x4 targetWMatrix= target_->GetWorldTransform()->matWorld_;

	// 目標のワールド行列から位置を取得し、弾の速度を計算
	Vector3 velocity = Normalize({
		targetWMatrix.m[3][0] - worldTransform_->matWorld_.m[3][0],
		targetWMatrix.m[3][1] - worldTransform_->matWorld_.m[3][1],
		targetWMatrix.m[3][2] - worldTransform_->matWorld_.m[3][2]
	});



	bullet->SetVelocity(velocity);

	colliderManager_->AddCollider(bullet.get());

	bullets_.push_back(std::move(bullet));
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
