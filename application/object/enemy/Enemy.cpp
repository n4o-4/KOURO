#include "Enemy.h"
#include "PlayerBullet.h"

#include "algorithm"

void Enemy::Initialize(LineModel* model)
{
	///=========================================
	/// 親クラス

	// 初期化
	BaseCharacter::Initialize(model);

	worldTransform_->useQuaternion_ = false;

	worldTransform_->quaternionTransform.scale = { 5.0f,5.0f,5.0f };
	worldTransform_->transform.scale = { 1.0f,1.0f,1.0f };

	AABBCollider::Initialize(worldTransform_.get(),this);

	SetCollisionAttribute(0b1 << 1); // コリジョン属性を設定
	SetCollisionMask(0b1); // コリジョンマスクを設定

	fireTimer_ = 0.0f; // 弾の発射タイマー初期化

	hp_ = 5;

	emitter_.Initialize("HitEffect");
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

	std::vector<ColliderVariant> colliders = colliderManager_->GetColliders();

	std::erase_if(bullets_, [](const ColliderVariant& collider) {
		return std::visit([](auto& ptr) {
			return ptr && !ptr->GetIsAlive();
			}, collider);
		});

	///=========================================
	/// 親クラスA

	// 更新
	BaseCharacter::Update();

	AABBCollider::Update();
}

void Enemy::Draw()
{
	for (auto& bullet : bullets_)
	{
		bullet->Draw();
	}

	///=========================================
	/// 親クラス

	// 描画
	BaseCharacter::Draw();
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

void Enemy::ChangeState(std::unique_ptr<EnemyState> state)
{
	if (!state_)
	{
		state_ = std::move(state);
		state_->Enter(this);
	}
	else
	{
		state_->Exit(this);
		state_ = std::move(state);
		state_->Enter(this);
	}
}

void Enemy::Fire()
{
	// 弾の生成
	std::shared_ptr<EnemyBullet> bullet = std::make_shared<EnemyBullet>();

	// 初期化
	bullet->Initialize(lineModelManager_->FindLineModel("playerbullet/playerbullet.obj"), { worldTransform_->matWorld_.m[3][0],worldTransform_->matWorld_.m[3][1],worldTransform_->matWorld_.m[3][2] });


	Matrix4x4 targetWMatrix= target_->GetWorldTransform()->matWorld_;

	// 目標のワールド行列から位置を取得し、弾の速度を計算
	Vector3 velocity = Normalize({
		targetWMatrix.m[3][0] - worldTransform_->matWorld_.m[3][0],
		targetWMatrix.m[3][1] - worldTransform_->matWorld_.m[3][1],
		targetWMatrix.m[3][2] - worldTransform_->matWorld_.m[3][2]
	});

	bullet->SetVelocity(velocity);

	colliderManager_->AddCollider(bullet);

	bullets_.push_back(std::move(bullet));
}

void Enemy::OnCollisionEnter(BaseCollider* other)
{
	// 衝突したオブジェクトがPlayerBulletの場合、弾を消す
	if (PlayerBullet* playerBullet = dynamic_cast<PlayerBullet*>(other))
	{
		if(hp_ > playerBullet->GetDamage())
		{
			hp_ -= playerBullet->GetDamage();


			ParticleManager::GetInstance()->GetParticleGroup("HitEffect")->enableDeceleration = true;

			
			emitter_.SetPosition(worldTransform_->transform.translate);
			emitter_.SetParticleCount(1);
			emitter_.SetVelocityRange({ {0.0f,0.0f,0.0f },{0.0f,0.0f,0.0f} });
			emitter_.SetStartScaleRange({ {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} });
			emitter_.SetFinishScaleRange({ {3.0f,3.0f,3.0f},{3.0f,3.0f,3.0f} });
			emitter_.SetRotateRange({ {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} });


			emitter_.SetTranslateRange({ { 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f } });

			emitter_.SetStartColorRange({ {1.0f,1.0f,1.0f,1.0f}, {1.0f,1.0f,1.0f,1.0f} });
			emitter_.SetFinishColorRange({ {1.0f,1.0f,1.0f,0.0f},{1.0f,1.0f,1.0f,0.0f} }); /*{ 1.0f, 0.72f, 0.19f, 1.0f }*/
			emitter_.SetLifeTimeRange({ 0.5f,0.5f });
			emitter_.SetFrequency(0.2f);

			emitter_.Emit();
		}
		else
		{
			hp_ = 0;
		}
	}

	if (hp_ == 0)
	{
		BaseEntity::isAlive_ = false;
	}
}

void Enemy::OnCollisionStay(BaseCollider* other)
{
}

void Enemy::OnCollisionExit(BaseCollider* other)
{
}
