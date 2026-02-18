#include "Player.h"

#include <algorithm>

#include "EnemyBullet.h"
#include "Easing.h"


void Player::Initialize(Kouro::LineModel* model)
{
	///=========================================
	/// 親クラス
	
	// 初期化
	BaseCharacter::Initialize(model);

	AABBCollider::Initialize(worldTransform_.get(),this);

	SetCollisionAttribute(0b1); // コリジョン属性を設定

	SetCollisionMask(0b1 << 1); // コリジョンマスクの設定

	// AABBのサイズの設定
	SetAABB(Kouro::AABB({}, { -1.0f,-1.0f,-1.0f }, {1.0f,1.0f,1.0f}));

	// 線モデルの色を設定
	objectLine_->SetColor(kDefaultColor_);

	// レールのコントロールポイントを設定
	rail.Initialize(controlPoints_);

	// 座標の位置を設定
	colliderTransform_->SetTranslate({ 0.0f,0.0f, 0.0f });

	// 座標を一度更新
	colliderTransform_->UpdateMatrix();

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
		if (Kouro::Input::GetInstance()->TriggerKey(DIK_SPACE) || Kouro::Input::GetInstance()->PushKey(DIK_SPACE))
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
		objectLine_->SetColor(kDefaultColor_);
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
		float length = Kouro::Length(velocity_);

		// lShiftを押していたら
		if (Kouro::Input::GetInstance()->TriggerKey(DIK_LSHIFT) && length != 0.0f)
		{
			// quickMove中に変更
			quickMoveData_->isQuickMoving = true;
		}

	}

	// 移動
	Move();

	// 移動制限を適用
	Kouro::Vector3 pos = worldTransform_->GetTranslate();

	pos.x = std::clamp(pos.x, moveLimitMin_.x, moveLimitMax_.x);
	pos.y = std::clamp(pos.y, moveLimitMin_.y, moveLimitMax_.y);

	worldTransform_->SetTranslate(pos);

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
	// 入力の取得
	Kouro::Input* input = Kouro::Input::GetInstance();

	// 入力に応じた速度の計算
	Kouro::Vector3 inputVelocity = { 0.0f,0.0f,0.0f };

	// 横方向
	if (input->PushKey(DIK_A)) { inputVelocity.x -= 1.0f; }
	if (input->PushKey(DIK_D)) { inputVelocity.x += 1.0f; }

	// 縦方向
	if (input->PushKey(DIK_W)) { inputVelocity.y += 1.0f; }
	if (input->PushKey(DIK_S)) { inputVelocity.y -= 1.0f; }

	// 速度の正規化とスピードの適用
	float length = Kouro::Length(inputVelocity);

	// ブースト時間を過ぎていたら || ブースト中のフラグでなければ
	if (quickMoveData_->actionTimer > quickMoveData_->duration || !quickMoveData_->isQuickMoving)
	{
		// 入力ベクトルを正規化してから通常のスピードを掛ける
		inputVelocity = Normalize(inputVelocity) * kDefaultPlayerSpeed_ * kDeltaTime;

		velocity_ += inputVelocity;

		velocity_ *= kDefaultFriction_;

		length = Kouro::Length(velocity_);

		if (length > kDefaultPlayerSpeed_)
		{
			velocity_ = Normalize(velocity_) * kDefaultPlayerSpeed_;
		}

		float targetAngleZ = 0;
		if (inputVelocity.x > 0.0f)
		{
			targetAngleZ = -kTargetTiltAngle_;
		}
		else if (inputVelocity.x < 0.0f)
		{
			targetAngleZ = kTargetTiltAngle_;
		}

		Kouro::Vector3 rotate = worldTransform_->GetRotate();

		rotate.z = Kouro::Lerp(rotate.z, targetAngleZ , 0.1f);
		
		worldTransform_->SetRotate(rotate);
	}
	else
	{
		velocity_ = Normalize(velocity_) * kBoostPlayerSpeed_;

		velocity_ *= kBoostFriction_;

		length = Kouro::Length(velocity_);

		if (length > kBoostPlayerSpeed_)
		{
			velocity_ = Normalize(velocity_) * kBoostPlayerSpeed_;
		}
	}
}

void Player::Fire()
{
	// 計測用のタイマーをインターバルタイマーの定数に設定
	fireTimer_ = 0.0f;

	// 弾の生成
	std::shared_ptr<PlayerBullet> bullet = std::make_shared<PlayerBullet>();

	Kouro::Matrix4x4 matWorld = worldTransform_->GetWorldMatrix();

	// 初期化
	bullet->Initialize(lineModelManager_->FindLineModel("playerbullet/playerbullet.obj"), { matWorld.m[3][0],matWorld.m[3][1],matWorld.m[3][2] });

	// プレイヤーが向いている方向にvelocityを変換する
	Kouro::Vector3 velocity = TransformNormal({ 0.0f,0.0f,bulletSpeed_ }, matWorld);

	// 弾に計算したvelocityを設定する
	bullet->SetVelocity(velocity);

	// forward ベクトルからオイラー角を計算
	Kouro::Vector3 forward = Normalize(velocity);
	float yaw = std::atan2(forward.x, forward.z);
	float pitch = std::asin(-forward.y);
	float roll = 0.0f;

	bullet->GetWorldTransform()->SetRotate({ pitch, yaw, roll });

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
			objectLine_->SetColor(kHitColor_);

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
