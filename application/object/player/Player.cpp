#include "Player.h"

#include "EnemyBullet.h"

void Player::Initialize(LineModel* model)
{
	///=========================================
	/// 親クラス
	
	// 初期化
	BaseCharacter::Initialize(model);

	worldTransform_->useQuaternion_ = false;

	worldTransform_->quaternionTransform.scale = { 1.0f,1.0f,1.0f };
	worldTransform_->transform.scale = { 0.5f,0.5f,0.5f };

	AABBCollider::Initialize(worldTransform_.get(),this);

	SetCollisionAttribute(0b1); // コリジョン属性を設定

	SetCollisionMask(0b1 << 1);

	SetAABB(AABB({}, { -1.0f,-1.0f,-1.0f }, {1.0f,1.0f,1.0f}));

	objectLine_->SetColor({ 0.071f, 0.429f, 1.0f,1.0f });

	rail.Initialize(controlPoints_);

	colliderTransform_->transform.translate = { 0.0f,0.0f,25.0f };

	colliderTransform_->UpdateMatrix();

	//colliderTransform_->SetParent(rail.GetWorldTransform());

	hp_ = 1000;

	quickMoveData_ = std::make_unique<QuickMoveData>();
	quickMoveData_->isQuickMoving = false;
	quickMoveData_->duration = 1.0f;
	quickMoveData_->coolTime = 3.0f;
	quickMoveData_->actionTimer = 0.0f;
}

void Player::Update()
{
	isHit_ = false;

	if(fireTimer_ > 0.0f)
	{
		fireTimer_ -= kDeltaTime;
	}
	else
	{
		if (Input::GetInstance()->/*PushKey*/Triggerkey(DIK_SPACE) || Input::GetInstance()->PushKey(DIK_SPACE))
		{
			Fire();
		}
	}

	if(actionData_.hitintervalTimer_ > 0.0f)
	{
		actionData_.hitintervalTimer_ -= kDeltaTime;
	}
	else
	{
		actionData_.hitintervalTimer_ = 0.0f;
		objectLine_->SetColor({ 0.071f, 0.429f, 1.0f,1.0f });
	}

	// quickMove中の場合
	if (quickMoveData_->isQuickMoving)
	{
		// 時間計測用の変数にdeltaTimeを加算
		quickMoveData_->actionTimer += kDeltaTime;


		float duration = quickMoveData_->duration;
		float coolTime = quickMoveData_->coolTime;
		float timer = quickMoveData_->actionTimer;

		// timerがquickMoveの全体動作時間を超えていたら再使用可能に
		if (duration + coolTime < timer)
		{
			quickMoveData_->actionTimer = 0.0f;
			quickMoveData_->isQuickMoving = false;
		}
	}
	// quickMove中じゃない場合
	else
	{
		// lShiftを押していたら
		if (Input::GetInstance()->Triggerkey(DIK_LSHIFT))
		{
			// quickMove中に変更
			quickMoveData_->isQuickMoving = true;

			if (Input::GetInstance()->PushKey(DIK_A))
			{
				velocity_.x -= 0.3f;
			}
			if (Input::GetInstance()->PushKey(DIK_D))
			{
				velocity_.x += 0.3f;
			}
			if (Input::GetInstance()->PushKey(DIK_W))
			{
				velocity_.y += 0.3f;
			}
			if (Input::GetInstance()->PushKey(DIK_S))
			{
				velocity_.y -= 0.3f;
			}
		}

	}

	// 移動
	Move();

	

	//rail.Update();

	colliderTransform_->transform;

	//worldTransform_->deltaRotate_ = { 0.01f,0.0f,0.0f };


	//if (moveTimer_ < kMoveTime)
	//{
	//	moveTimer_ += kDeltaTime;
	//}
	//else
	//{
	//	moveTimer_ = 0.0f;
	//}

	//float t = moveTimer_ / kMoveTime;

	//worldTransform_->transform.translate = CatmullRomPosition(controlPoints_, t);

	//float nextT = (moveTimer_ + (kDeltaTime * 100.0f)) / kMoveTime;

	//Vector3 viewTarget = CatmullRomPosition(controlPoints_, nextT);

	//// 現在位置
	//Vector3 position = worldTransform_->transform.translate;
	//// 次の位置
	//Vector3 target = viewTarget;

	//// forwardベクトル（進行方向）
	//Vector3 forward = Normalize(target - position);

	//// Y軸を上方向と仮定し、forwardからオイラー角を算出
	//float yaw = std::atan2(forward.x, forward.z);
	//float pitch = std::asin(-forward.y);
	//float roll = 0.0f; // 必要に応じて

	//worldTransform_->transform.rotate = { pitch, yaw, roll };

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
	/// 親クラス


#ifdef _DEBUG

	ImGui::Begin("Player Debug");
	ImGui::Text("Translate X: %.2f Translate Y: %.2f Translate Z: %.2f", worldTransform_->matWorld_.m[3][0], worldTransform_->matWorld_.m[3][1], worldTransform_->matWorld_.m[3][2]);
	ImGui::End();

#endif

	// 更新
	BaseCharacter::Update();

	AABBCollider::Update();

#ifdef _DEBUG

	ImGui::Begin("Player Transform");
	
	ImGui::DragFloat3("scale", &worldTransform_->quaternionTransform.scale.x, 0.01f);
	ImGui::DragFloat4("rotate", &worldTransform_->quaternionTransform.rotate.x, 0.01f);
	ImGui::DragFloat3("translate", &worldTransform_->quaternionTransform.translate.x, 0.01f);

	ImGui::End();

#endif
}

void Player::Draw()
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

bool Player::GetIsAlive()
{
	if (hp_ > 0)
	{
		return true;
	}

	return false;
}

void Player::Move()
{
	if (quickMoveData_->actionTimer > quickMoveData_->duration || !quickMoveData_->isQuickMoving)
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
			velocity_.y += 0.1f;
		}
		if (Input::GetInstance()->PushKey(DIK_S))
		{
			velocity_.y -= 0.1f;
		}
	}
	else
	{

	}
	
}

void Player::Fire()
{
	fireTimer_ = kFireInterval_;

	// 弾の生成
	std::shared_ptr<PlayerBullet> bullet = std::make_shared<PlayerBullet>();

	// 初期化
	bullet->Initialize(lineModelManager_->FindLineModel("playerbullet/playerbullet.obj"), { worldTransform_->matWorld_.m[3][0],worldTransform_->matWorld_.m[3][1],worldTransform_->matWorld_.m[3][2] });

	Vector3 velocity = TransformNormal({ 0.0f,0.0f,5.0f }, worldTransform_->matWorld_);

	bullet->SetVelocity(velocity);

	colliderManager_->AddCollider(bullet);

	bullets_.push_back(std::move(bullet));
}

void Player::OnCollisionEnter(BaseCollider* other)
{
	if (EnemyBullet* enemyBullet = dynamic_cast<EnemyBullet*>(other))
	{
		isHit_ = true;

		if (hp_ > 0 && actionData_.hitintervalTimer_ <= 0.0f)
		{
			actionData_.hitintervalTimer_ = actionData_.kHitInterval_;

			objectLine_->SetColor({ 1.0f,0.0f,0.0f,1.0f });

			--hp_;
		}
	}
}

void Player::OnCollisionStay(BaseCollider* other)
{
}

void Player::OnCollisionExit(BaseCollider* other)
{
}
