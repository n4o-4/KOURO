#include "Player.h"

void Player::Initialize(Model* model)
{
	///=========================================
	/// 親クラス
	
	// 初期化
	BaseCharacter::Initialize(model);

	worldTransform_->useQuaternion_ = false;

	worldTransform_->quaternionTransform.scale = { 1.0f,1.0f,1.0f };
	worldTransform_->transform.scale = { 0.5f,0.5f,0.5f };
	worldTransform_->transform.translate.z = 25.0f; // 初期位置を設定
	worldTransform_->transform.translate.y = -5.0f; // 初期位置を設定

	AABBCollider::Initialize(worldTransform_.get(),this);

	SetCollisionAttribute(0b1); // コリジョン属性を設定

	SetCollisionMask(0b1 << 1);

	SetAABB(AABB({}, { -1.0f,-1.0f,-1.0f }, {1.0f,1.0f,1.0f}));

	hp_ = 10;
}

void Player::Update()
{
	// 移動
	Move();

	if (Input::GetInstance()->/*PushKey*/Triggerkey(DIK_SPACE))
	{
		Fire();
	}

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

void Player::Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	for (auto& bullet : bullets_)
	{
		bullet->Draw(directionalLight, pointLight, spotLight);
	}

	///=========================================
	/// 親クラス
	
	// 描画
	//BaseCharacter::Draw(directionalLight,pointLight,spotLight);
}

void Player::Move()
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

void Player::Fire()
{
	// 弾の生成
	std::shared_ptr<PlayerBullet> bullet = std::make_shared<PlayerBullet>();

	// 初期化
	bullet->Initialize(ModelManager::GetInstance()->FindModel("playerbullet/playerbullet.obj"), { worldTransform_->matWorld_.m[3][0],worldTransform_->matWorld_.m[3][1],worldTransform_->matWorld_.m[3][2] });

	bullet->SetCamera(camera_);

	Vector3 velocity = TransformNormal({ 0.0f,0.0f,5.0f }, worldTransform_->matWorld_);

	bullet->SetVelocity(velocity);

	colliderManager_->AddCollider(bullet);

	bullets_.push_back(std::move(bullet));
}

void Player::OnCollisionEnter(BaseCollider* other)
{
}

void Player::OnCollisionStay(BaseCollider* other)
{
}

void Player::OnCollisionExit(BaseCollider* other)
{
}
