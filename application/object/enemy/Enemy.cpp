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

	fireTimer_ = 0.0f; // 弾の発射タイマー初期化

	hp_ = 5;
}

void Enemy::Update()
{
	std::vector<ColliderVariant> colliders = colliderManager_->GetColliders();

	std::erase_if(bullets_, [](const std::unique_ptr<EnemyBullet>& bullet) {
		return bullet && !bullet->GetIsAlive();
		});

	if (actionData_.hitIntervalTimer_ < actionData_.kHitInterval_)
	{
		actionData_.hitIntervalTimer_ += kDeltaTime;
	}
	else
	{
		objectLine_->SetColor({ 0.196f, 0.929f, 0.369f,1.0f });
	}

	//// 生存していない弾をリストから削除
	//bullets_.erase(std::remove_if(bullets_.begin(), bullets_.end(),
	//	[](const std::shared_ptr<EnemyBullet>& bullet) { return !bullet->IsAlive(); }),
	//	bullets_.end());

	for (auto& bullet : bullets_)
	{
		bullet->Update();
	}

	///=========================================
	/// 親クラスA

	baseRailPosition_ = parent_->GetWorldPosition();

	state_->Update(this);

	worldTransform_->SetTranslate(baseRailPosition_ + moveOffset_ + basePosition_);

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

	// 描画
	BaseCharacter::Draw();
}

void Enemy::SetPosition(const Kouro::Vector3& position)
{
	worldTransform_->SetTranslate(position);
	worldTransform_->UpdateMatrix();
}

void Enemy::SetScale(const Kouro::Vector3& scale)
{
	worldTransform_->SetScale(scale);
	worldTransform_->UpdateMatrix();
}

void Enemy::SetRotate(const Kouro::Vector3& rotate)
{
	worldTransform_->SetRotate(rotate);
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

		SpawnRequestQueue::BulletSpawnInfo spawnInfo;

		spawnInfo.creator = []() { return std::make_unique<EnemyBullet>(); };
		spawnInfo.modelName = "playerbullet/playerbullet.obj";
		spawnInfo.position = worldTransform_->GetWorldPosition();
		spawnInfo.velocity = TransformNormal({dir }, worldTransform_->GetWorldMatrix());

		// 生成要求キューに弾の生成要求を追加
		spawnRequestQueue_->Push(spawnInfo);
	}
}

void Enemy::OnCollisionEnter(BaseCollider* other)
{
	if (isValid_ == false) return;
	if (actionData_.hitIntervalTimer_ <= actionData_.kHitInterval_) return;

	// 衝突したオブジェクトがPlayerBulletの場合、弾を消す
	if (PlayerBullet* playerBullet = dynamic_cast<PlayerBullet*>(other))
	{
		// プレイヤーの弾のダメージよりhpが大きかったら && ヒットインターバル計測用タイマーがヒットインターバルの定数以上だった場合
		if(hp_ > playerBullet->GetDamage())
		{
			// ヒットインターバル計測用タイマーを0に設定
			actionData_.hitIntervalTimer_ = 0.0f;

			// ヒットリアクションの色に変更
			objectLine_->SetColor({ 1.0f,0.0f,0.0f,1.0f });

			// hpをプレイヤーの弾のダメージ分減算
			hp_ -= playerBullet->GetDamage();
		}

		else if(hp_ <= playerBullet->GetDamage() && actionData_.hitIntervalTimer_ >= actionData_.kHitInterval_)
		{
			mEmitter_->Emit(worldTransform_->GetWorldMatrix());

			hp_ = 0;

			Destroy(Enemy::DestroyReason::CollisionWithBullet);
		}
	}
}

void Enemy::OnCollisionStay(BaseCollider* other)
{
}

void Enemy::OnCollisionExit(BaseCollider* other)
{
}
