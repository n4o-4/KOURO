#include "Player.h"

#include "EnemyBullet.h"

void Player::Initialize(LineModel* model)
{
	///=========================================
	/// 親クラス
	
	// 初期化
	BaseCharacter::Initialize(model);

	worldTransform_->useQuaternion_ = false;

	worldTransform_->transform.scale = { kDefaultScale,kDefaultScale,kDefaultScale };

	AABBCollider::Initialize(worldTransform_.get(),this);

	SetCollisionAttribute(0b1); // コリジョン属性を設定

	SetCollisionMask(0b1 << 1); // コリジョンマスクの設定

	// AABBのサイズの設定
	SetAABB(AABB({}, { -1.0f,-1.0f,-1.0f }, {1.0f,1.0f,1.0f}));

	// 線モデルの色を設定
	objectLine_->SetColor(kDefaultColor);

	// レールのコントロールポイントを設定
	rail.Initialize(controlPoints_);

	// 座標の位置を設定
	colliderTransform_->transform.translate = { 0.0f,0.0f, 0.0f };

	// 座標を一度更新
	colliderTransform_->UpdateMatrix();

	//colliderTransform_->SetParent(rail.GetWorldTransform());

	hp_ = 1000;

	// ブースト用の構造体の生成と初期化
	quickMoveData_ = std::make_unique<QuickMoveData>();
	quickMoveData_->isQuickMoving = false;
	quickMoveData_->duration = 1.0f;
	quickMoveData_->coolTime = 3.0f;
	quickMoveData_->actionTimer = 0.0f;
}

void Player::Update()
{
	// ヒットフラグをfalseに設定
	isHit_ = false;

	// 発射タイマーが発射インターバルの定数未満だった場合
	if(fireTimer_ < kFireInterval_)
	{
		// 発射タイマーから経過時間を減算
		fireTimer_ += kDeltaTime;
	}
	// 発射タイマーが発射インターバル以上だった場合
	else
	{
		// スペースキーを押していたら
		if (Input::GetInstance()->/*PushKey*/Triggerkey(DIK_SPACE) || Input::GetInstance()->PushKey(DIK_SPACE))
		{
			// 発射処理
			Fire();
		}
	}

	// ヒットインターバルタイマーがヒットインターバルの定数未満だった場合
	if(actionData_.hitIntervalTimer_ < actionData_.kHitInterval_)
	{
		// インターバルのタイマーから経過時間を減算
		actionData_.hitIntervalTimer_ += kDeltaTime;
	}
	
	// ヒットインターバルタイマーがヒットインターバル以上だった場合
	else
	{
		// 線モデルの色を設定
		objectLine_->SetColor(kDefaultColor);
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
				velocity_.x -= kBoostPlayerSpeed;
			}
			if (Input::GetInstance()->PushKey(DIK_D))
			{
				velocity_.x += kBoostPlayerSpeed;
			}
			if (Input::GetInstance()->PushKey(DIK_W))
			{
				velocity_.y += kBoostPlayerSpeed;
			}
			if (Input::GetInstance()->PushKey(DIK_S))
			{
				velocity_.y -= kBoostPlayerSpeed;
			}
		}

	}

	// 移動
	Move();

	colliderTransform_->transform;

	// 弾の更新
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
	
	ImGui::Text("scale : %.2f %.2f, %.2f", worldTransform_->GetScale().x, worldTransform_->GetScale().y, worldTransform_->GetScale().z);
	ImGui::Text("rotate : %.2f %.2f, %.2f", worldTransform_->GetRotate().x, worldTransform_->GetRotate().y, worldTransform_->GetRotate().z);
	ImGui::Text("translate : %.2f %.2f, %.2f", worldTransform_->GetTranslate().x, worldTransform_->GetTranslate().y, worldTransform_->GetTranslate().z);

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
	// hpが0より大きい場合
	if (hp_ > 0)
	{
		// trueを返す
		return true;
	}

	// それ以外の時はfalseを返す
	return false;
}

void Player::Move()
{

	// ブースト時間を過ぎていたら || ブースト中のフラグでなければ
	if (quickMoveData_->actionTimer > quickMoveData_->duration || !quickMoveData_->isQuickMoving)
	{
		// velocityを初期化
		velocity_ = { 0.0f,0.0f,0.0f };

		// 押しているキーによってvelocityに数値を加算または減算
		// TODO : 移動量をplayerSpeedとdeltaTimeで計算する
		if (Input::GetInstance()->PushKey(DIK_A))
		{
			velocity_.x -= kDefaultPlayerSpeed;
		}
		if (Input::GetInstance()->PushKey(DIK_D))
		{
			velocity_.x += kDefaultPlayerSpeed;
		}
		if (Input::GetInstance()->PushKey(DIK_W))
		{
			velocity_.y += kDefaultPlayerSpeed;
		}
		if (Input::GetInstance()->PushKey(DIK_S))
		{
			velocity_.y -= kDefaultPlayerSpeed;
		}
	}
	else
	{

	}
	
}

void Player::Fire()
{
	// 計測用のタイマーをインターバルタイマーの定数に設定
	fireTimer_ = 0.0f;

	// 弾の生成
	std::shared_ptr<PlayerBullet> bullet = std::make_shared<PlayerBullet>();

	// 初期化
	bullet->Initialize(lineModelManager_->FindLineModel("playerbullet/playerbullet.obj"), { worldTransform_->matWorld_.m[3][0],worldTransform_->matWorld_.m[3][1],worldTransform_->matWorld_.m[3][2] });

	// プレイヤーが向いている方向にvelocityを変換する
	Vector3 velocity = TransformNormal({ 0.0f,0.0f,bulletSpeed }, worldTransform_->matWorld_);

	// 弾に計算したvelocityを設定する
	bullet->SetVelocity(velocity);

	// forward ベクトルからオイラー角を計算
	Vector3 forward = Normalize(velocity);
	float yaw = std::atan2(forward.x, forward.z);
	float pitch = std::asin(-forward.y);
	float roll = 0.0f;

	bullet->GetWorldTransform()->transform.rotate = { pitch, yaw, roll };

	// コライダーマネージャーに弾を追加する
	colliderManager_->AddCollider(bullet);

	// 弾のリストに今回作った弾を移動する
	bullets_.push_back(std::move(bullet));
}

void Player::OnCollisionEnter(BaseCollider* other)
{
	// otherが敵の弾に型変換できた場合
	if (EnemyBullet* enemyBullet = dynamic_cast<EnemyBullet*>(other))
	{
		// ヒットフラグをtrueに
		isHit_ = true;

		// hpが0以外 && ヒットインターバル中で無ければ
		if (hp_ > 0 && actionData_.hitIntervalTimer_ >= actionData_.kHitInterval_)
		{

			// ヒットインターバルの計測用タイマーを0に設定する
			actionData_.hitIntervalTimer_ = 0.0f;

			// 線モデルの色をヒットアクションの色に変更
			objectLine_->SetColor(kHitColor);

			// hpを減算
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
