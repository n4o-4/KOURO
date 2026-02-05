#include "Enemy.h"
#include "PlayerBullet.h"

#include "algorithm"
#include "GpuParticle.h"

void Enemy::Initialize(Kouro::LineModel* model)
{
	///=========================================
	/// 親クラス

	// 初期化
	BaseCharacter::Initialize(model);

	AABBCollider::Initialize(worldTransform_.get(),this);

	SetCollisionAttribute(0b1 << 1); // コリジョン属性を設定
	SetCollisionMask(0b1); // コリジョンマスクを設定

	dummy_ = std::make_shared<EnemyBullet>();
	dummy_->Initialize(model, {});

	fireTimer_ = 0.0f; // 弾の発射タイマー初期化

	hp_ = 5;
}

void Enemy::Update()
{
	std::vector<ColliderVariant> colliders = colliderManager_->GetColliders();

	std::erase_if(bullets_, [](const ColliderVariant& collider) {
		return std::visit([](auto& ptr) {
			return ptr && !ptr->GetIsAlive();
			}, collider);
		});

	if (actionData_.hitIntervalTimer_ < actionData_.kHitInterval_)
	{
		actionData_.hitIntervalTimer_ += kDeltaTime;
	}
	else
	{
		objectLine_->SetColor({ 0.196f, 0.929f, 0.369f,1.0f });
	}

	for (auto& bullet : bullets_)
	{
		bullet->Update();
	}

	if(drawDummy_)
	{
		dummy_->Update();
	}

	///=========================================
	/// 親クラスA

	state_->Update(this);

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

	if (drawDummy_)
	{
		dummy_->Draw();
	}
	else
	{
		///=========================================
	    /// 親クラス

	    // 描画
		BaseCharacter::Draw();
	}
}

void Enemy::SetPosition(const Kouro::Vector3& position)
{
	worldTransform_->SetTranslate(position);

	worldTransform_->UpdateMatrix();
}

void Enemy::ChangeState(std::unique_ptr<EnemyState> state)
{
	if (!state_)
	{
		state_ = std::move(state);

		RailCamera* railCamera = dynamic_cast<RailCamera*>(cameraManager_->GetActiveCamera());
		state_->OnEnter(this);
	}
	else
	{
		state_->OnExit(this);
		state_ = std::move(state);
		RailCamera* railCamera = dynamic_cast<RailCamera*>(cameraManager_->GetActiveCamera());
		state_->OnEnter(this);
	}
}

void Enemy::Fire()
{
	//// 弾の生成
	//std::shared_ptr<EnemyBullet> bullet = std::make_shared<EnemyBullet>();

	//// 初期化
	//bullet->Initialize(lineModelManager_->FindLineModel("playerbullet/playerbullet.obj"), { worldTransform_->matWorld_.m[3][0],worldTransform_->matWorld_.m[3][1],worldTransform_->matWorld_.m[3][2] });


	//Kouro::Matrix4x4 targetWMatrix= target_->GetWorldTransform()->matWorld_;

	//// 目標のワールド行列から位置を取得し、弾の速度を計算
	//Kouro::Vector3 velocity = Kouro::Normalize({
	//	targetWMatrix.m[3][0] - worldTransform_->matWorld_.m[3][0],
	//	targetWMatrix.m[3][1] - worldTransform_->matWorld_.m[3][1],
	//	targetWMatrix.m[3][2] - worldTransform_->matWorld_.m[3][2]
	//});

	//bullet->SetVelocity(velocity);

	//colliderManager_->AddCollider(bullet);

	//bullets_.push_back(std::move(bullet));

	const int bulletCount = 7;
	const float spreadAngle = DirectX::XMConvertToRadians(60.0f);

	Kouro::Matrix4x4 matWorld = worldTransform_->GetWorldMatrix();

	// 敵の位置
	Kouro::Vector3 enemyPos = {
		matWorld.m[3][0],
		matWorld.m[3][1],
		matWorld.m[3][2]
	};

	// 自機方向（XZだけ使う）
	Kouro::Matrix4x4 targetWMatrix = target_->GetWorldTransform()->GetWorldMatrix();
	Kouro::Vector3 targetPos = {
		targetWMatrix.m[3][0],
		targetWMatrix.m[3][1],
		targetWMatrix.m[3][2]
	};

	// 中心方向（XZ平面）
	Kouro::Vector3 centerDir = {
		targetPos.x - enemyPos.x,
		0.0f,
		targetPos.z - enemyPos.z
	};
	centerDir = Kouro::Normalize(centerDir);

	// 扇形の角度設定
	float startAngle = -spreadAngle * 0.5f;
	float angleStep =
		(bulletCount > 1) ? spreadAngle / (bulletCount - 1) : 0.0f;

	for (int i = 0; i < bulletCount; ++i)
	{
		float angle = startAngle + angleStep * i;

		// Y軸回転（X方向に広げる）
		Kouro::Matrix4x4 rot = Kouro::MakeRotateYMatrix(angle);

		Kouro::Vector3 dir =
			Kouro::Normalize(TransformNormal(centerDir, rot));

		std::shared_ptr<EnemyBullet> bullet =
			std::make_shared<EnemyBullet>();

		bullet->Initialize(
			lineModelManager_->FindLineModel("playerbullet/playerbullet.obj"),
			enemyPos
		);

		bullet->SetVelocity(dir);

		colliderManager_->AddCollider(bullet);
		bullets_.push_back(bullet);
	}
}

void Enemy::OnCollisionEnter(BaseCollider* other)
{
	if (BaseEntity::isAlive_ == false) return;

	// 衝突したオブジェクトがPlayerBulletの場合、弾を消す
	if (PlayerBullet* playerBullet = dynamic_cast<PlayerBullet*>(other))
	{
		// プレイヤーの弾のダメージよりhpが大きかったら && ヒットインターバル計測用タイマーがヒットインターバルの定数以上だった場合
		if(hp_ > playerBullet->GetDamage() && actionData_.hitIntervalTimer_ >= actionData_.kHitInterval_)
		{
			// ヒットインターバル計測用タイマーを0に設定
			actionData_.hitIntervalTimer_ = 0.0f;

			// ヒットリアクションの色に変更
			objectLine_->SetColor({ 1.0f,0.0f,0.0f,1.0f });

			// hpをプレイヤーの弾のダメージ分減算
			hp_ -= playerBullet->GetDamage();


			Kouro::ParticleManager::GetInstance()->GetParticleGroup("HitEffect")->enableDeceleration = true;
			
			//emitter_.SetPosition(worldTransform_->transform.translate);
			//emitter_.SetParticleCount(1);
			//emitter_.SetVelocityRange({ {0.0f,0.0f,0.0f },{0.0f,0.0f,0.0f} });
			//emitter_.SetStartScaleRange({ {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} });
			//emitter_.SetFinishScaleRange({ {3.0f,3.0f,3.0f},{3.0f,3.0f,3.0f} });
			//emitter_.SetRotateRange({ {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} });

			emitter_.Initialize("ray");
			emitter_.SetParticleCount(10);
			emitter_.SetVelocityRange({ {-10.0f,-10.0f,-10.0f },{10.0f,10.0f,10.0f} });
			emitter_.SetStartScaleRange({ {0.3f,0.3f,0.3f},{0.3f,0.3f,0.3f} });
			emitter_.SetFinishScaleRange({ {0.1f,0.1f,0.1f},{0.1f,0.1f,0.1f} });
			emitter_.SetRotateRange({ {0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} });


			emitter_.SetTranslateRange({ { 0.0f,0.0f,0.0f },{ 0.0f,0.0f,0.0f } });

			emitter_.SetStartColorRange({ {0.667f, 0.169f, 1.0f, 1.0f}, {0.667f, 0.169f, 1.0f, 1.0f} });
			emitter_.SetFinishColorRange({ {0.667f, 0.169f, 1.0f, 1.0f}, {0.667f, 0.169f, 1.0f, 1.0f} }); /*{ 1.0f, 0.72f, 0.19f, 1.0f }*/
			emitter_.SetLifeTimeRange({ 0.5f,0.5f });
			emitter_.SetFrequency(0.01f);

			emitter_.Emit();
		}

		else if(hp_ <= playerBullet->GetDamage() && actionData_.hitIntervalTimer_ >= actionData_.kHitInterval_)
		{
			mEmitter_->Emit(worldTransform_->GetWorldMatrix());

			hp_ = 0;
		}
	}

	if (hp_ == 0)
	{
		BaseEntity::isAlive_ = false;
		//GpuParticle::GetInstance()->LineEmit(colliderTransform_->matWorld_);
	}
}

void Enemy::OnCollisionStay(BaseCollider* other)
{
}

void Enemy::OnCollisionExit(BaseCollider* other)
{
}
