#include "EnemyBullet.h"

#include "PlayerBullet.h"

void EnemyBullet::Initialize(Kouro::LineModel* model, Kouro::Vector3 spawnPos)
{
	// 親クラスの初期化

	BaseBullet::Initialize(model, spawnPos);

	// Colliderの初期化と設定

	SphereCollider::Initialize(worldTransform_.get(),this);
	SetCollisionAttribute(0b1 << 1); // コリジョン属性を設定
	SetCollisionMask(0b1); // コリジョンマスクを設定
	SetSphere(Kouro::Sphere({}, 1.0f));

	// パーティクルエミッターの初期化

	emitter_ = std::make_unique<Kouro::ParticleEmitter>();

	emitter_ = std::make_unique<Kouro::ParticleEmitter>();
	emitter_->Initialize("plane_Particle");
	emitter_->SetPosition({ 0.0f,2.0f,0.0f });
	emitter_->SetParticleCount(100);
	emitter_->SetVelocityRange({ {-20.0f,-20.0f,-20.0f },{20.0f,20.0f,20.0f} });
	emitter_->SetStartScaleRange({ {0.3f,0.3f,0.3f},{0.3f,0.3f,0.3f} });
	emitter_->SetFinishScaleRange({ {0.1f,0.1f,0.1f},{0.1f,0.1f,0.1f} });
	emitter_->SetStartColorRange({ {0.98f,0.37f,0.01f,1.0f}, {1.0f,0.72f,0.19f,1.0f} });
	emitter_->SetFinishColorRange({ {0.98f,0.0f,0.0f,1.0f}, {1.0f,0.72f,0.19f,1.0f} });
	emitter_->SetLifeTimeRange({ 0.5f,0.8f });
	emitter_->SetFrequency(1.0f);
}

void EnemyBullet::Update()
{
	// forward ベクトルからオイラー角を計算
	Kouro::Vector3 forward = Kouro::Normalize(velocity_);
	float yaw = std::atan2(forward.x, forward.z);
	float pitch = std::asin(-forward.y);
	float roll = 0.0f;

	worldTransform_->SetRotate({ pitch, yaw, roll });

	// 親クラスの更新

	BaseBullet::Update();

	// Colliderの更新

	SphereCollider::Update();
}

void EnemyBullet::Draw()
{
	// 親クラスの描画

	BaseBullet::Draw();
}

void EnemyBullet::OnCollisionEnter(BaseCollider* other)
{
	// 衝突したオブジェクトがPlayerBulletの場合、弾を消す
	if (PlayerBullet* playerBullet = dynamic_cast<PlayerBullet*>(other))
	{
		BaseEntity::isAlive_ = false; // 弾を消す
	}
}

void EnemyBullet::OnCollisionStay(BaseCollider* other)
{
}

void EnemyBullet::OnCollisionExit(BaseCollider* other)
{
}
