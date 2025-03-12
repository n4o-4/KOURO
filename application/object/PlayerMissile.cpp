#define _USE_MATH_DEFINES
#define NOMINMAX
#include <algorithm>
#include <cmath>
#include <random>
#include "PlayerMissile.h"

// コンストラクタ実装
PlayerMissile::PlayerMissile(const Vector3 &position, const Vector3 &initialVelocity,
	const Vector3 &scale, const Vector3 &rotate) {
	model_ = std::make_unique<Object3d>();
	model_->Initialize(Object3dCommon::GetInstance());

	ModelManager::GetInstance()->LoadModel("missile.obj");
	model_->SetModel("missile.obj");

	isActive_ = true;  // 明示的にアクティブに設定

	// トランスフォームの初期化
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->transform.translate = position;
	worldTransform_->transform.scale = scale;
	worldTransform_->transform.rotate = rotate;

	// 初期速度の設定
	velocity_ = initialVelocity;

	// ミサイル性能にランダムなばらつきを追加
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.64f, 1.28f);
	performanceVariation_ = dis(gen);

	// 前回視線ベクトルの初期化（初回はゼロでよい）
	prevLineOfSight_ = { 0, 0, 0 };

	//========================================
	// 当たり判定との同期
	BaseObject::Initialize(worldTransform_->transform.translate, 1.0f);
}


void PlayerMissile::Update() {
	// ライフタイムを更新
	lifeTime_++;

	// 状態に応じた更新処理
	switch(state_) {
	case BulletState::kLaunch:
		UpdateLaunchState();
		break;
	case BulletState::kTracking:
		UpdateTrackingState();
		break;
	case BulletState::kFinal:
		UpdateFinalState();
		break;
	}

	// 位置の更新
	worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;
	model_->SetLocalMatrix(MakeIdentity4x4());// ローカル行列を単位行列に
	worldTransform_->UpdateMatrix();

	// 当たり判定の更新
	BaseObject::Update(worldTransform_->transform.translate);

	// 近接信管の処理
	ApplyProximityFuse();

	// ライフタイム経過によるチェック（寿命）
	if(lifeTime_ > kLifeTimeLimit) {
		isActive_ = false;
	}

	// ターゲットが無効になった場合のチェック
	if(target_ && target_->GetHp() <= 0) {
		// ターゲットが無効になった場合、直進するために状態を維持
		target_ = nullptr;

		// 状態が最終状態でない場合は直進状態に切り替える
		if(state_ == BulletState::kTracking) {
			// 現在速度のまま直進する
			velocity_ = Normalize(velocity_) * kBaseTrackingSpeed;
		}
	}
}

// 発射初期状態の更新
void PlayerMissile::UpdateLaunchState() {
	// タイマー更新
	stateTimer_++;

	// 慣性飛行（最初の数フレームは初期方向に飛ぶ）
	if(stateTimer_ <= kInertialFlightTime) {
		// 初期速度で直進
		float launchSpeedFactor = std::min(1.0f, stateTimer_ / 10.0f);
		velocity_ = Normalize(velocity_) * ( kInitialSpeed + launchSpeedFactor * 0.1f );
	} else {
		// 徐々に目標方向へ旋回を開始
		if(target_ && target_->GetHp() > 0) {
			// ターゲットへの方向ベクトル
			Vector3 targetPos = target_->GetPosition();
			Vector3 direction = targetPos - worldTransform_->transform.translate;
			Vector3 normalizedDirection = Normalize(direction);

			// 現在の速度方向
			Vector3 currentDir = Normalize(velocity_);

			// 徐々にターゲット方向に旋回
			float turnRate = ( stateTimer_ - kInertialFlightTime ) / 30.0f * 0.02f;
			turnRate = std::min(turnRate, 0.03f);

			// 現在の方向と目標方向の間を補間
			Vector3 newDir = currentDir + ( normalizedDirection - currentDir ) * turnRate;
			newDir = Normalize(newDir);

			// 速度を徐々に増加
			float speedFactor = std::min(1.0f, stateTimer_ / kLaunchSpeedDivider);
			float currentSpeed = kInitialSpeed + speedFactor * kSpeedIncreaseFactor;
			velocity_ = newDir * currentSpeed;
		} else {
			// ターゲットがない場合は直進し続ける
			float speedFactor = std::min(1.0f, stateTimer_ / kLaunchSpeedDivider);
			float currentSpeed = kInitialSpeed + speedFactor * kSpeedIncreaseFactor;
			velocity_ = Normalize(velocity_) * currentSpeed;
		}
	}

	// わずかにランダムな動きを加える（発射演出）
	if(stateTimer_ % kRandomMoveInterval == 0 && stateTimer_ < kLaunchStateTransitionTime * 0.7f) {
		float randomX = ( rand() % 100 - 50 ) / kRandomMoveFactor;
		float randomY = ( rand() % 100 - 50 ) / kRandomMoveFactor;
		velocity_ = velocity_ + Vector3{ randomX, randomY, 0.0f };
		velocity_ = Normalize(velocity_) * Length(velocity_); // 速度の大きさを維持
	}

	// 一定時間経過後、トラッキング状態に移行
	if(stateTimer_ >= kLaunchStateTransitionTime) {
		state_ = BulletState::kTracking;
		stateTimer_ = 0;

		// 前回の視線方向を初期化（追尾状態開始時）
		if(target_ && target_->GetHp() > 0) {
			prevLineOfSight_ = target_->GetPosition() - worldTransform_->transform.translate;
		}
	}
}

// 追尾状態の更新
void PlayerMissile::UpdateTrackingState() {
	stateTimer_++;

	if(target_ && target_->GetHp() > 0) {
		// ターゲットの位置を取得
		Vector3 targetPos = target_->GetPosition();
		Vector3 myPos = worldTransform_->transform.translate;
		Vector3 direction = targetPos - myPos;
		float distance = Length(direction);

		// 比例航法による誘導
		Vector3 navVector = CalculateNavigationVector();

		// 旋回性能の限界を考慮（より小さくして緩やかに）
		float navMagnitude = Length(navVector);
		if(navMagnitude > maxTurnRate_ * 0.7f) {  // 旋回速度を70%に制限
			navVector = Normalize(navVector) * ( maxTurnRate_ * 0.7f );
		}

		// 弧を描くような動きを追加（より穏やかな弧を描く）
		float arcFactor = sinf(stateTimer_ * ( kArcOscillationSpeed * 0.7f )) * ( kArcStrength * 0.8f );
		Vector3 perpVector = Cross(Normalize(velocity_), Vector3{ 0, 1, 0 });
		perpVector = perpVector * arcFactor;

		// 追尾遅延のシミュレーション（値を大きくしてより滑らかに）
		navVector = navVector * ( kTrackingDelayFactor * 0.7f ) * performanceVariation_;

		// 速度ベクトルを更新（より緩やかな補間で）
		// スムージング係数を導入（1.0だと即座に変更、小さいほど緩やかに変更）
		float smoothingFactor = 0.15f;
		Vector3 desiredChangeVector = navVector + perpVector;
		Vector3 smoothedChangeVector = desiredChangeVector * smoothingFactor;

		velocity_ = velocity_ + smoothedChangeVector;

			// 速度の大きさを調整（より高速に）
		float currentMaxSpeed = kBaseTrackingSpeed +
			std::min(kMaxSpeedIncrease, stateTimer_ / kSpeedIncreaseDivider);

		// 性能のバラつきを適用（高性能なミサイルはより速く）
		currentMaxSpeed *= ( 1.0f + ( performanceVariation_ - 1.0f ) * 0.5f );

		velocity_ = Normalize(velocity_) * currentMaxSpeed;

				// 近づいたら最終段階へ
		if(distance < kFinalStateTransitionDistance) {
			state_ = BulletState::kFinal;
			stateTimer_ = 0;
		}
	} else {
		// ターゲットがいない場合は直進
		velocity_ = Normalize(velocity_) * kBaseTrackingSpeed;
	}

	// 長時間追尾してもターゲットに到達できない場合
	if(stateTimer_ > kAutoFinalStateTransitionTime) {
		state_ = BulletState::kFinal;
		stateTimer_ = 0;
	}
}

// 最終接近状態の更新
void PlayerMissile::UpdateFinalState() {
	stateTimer_++;

	if(target_ && target_->GetHp() > 0) {
		// ターゲットへの方向
		Vector3 targetPos = target_->GetPosition();
		Vector3 myPos = worldTransform_->transform.translate;
		Vector3 direction = targetPos - myPos;
		float distance = Length(direction);

		// 現在の速度方向
		Vector3 currentDir = Normalize(velocity_);
		// 目標方向
		Vector3 targetDir = Normalize(direction);

		// 最終接近段階での急旋回（性能限界を超えた急旋回）
		float finalTurnRate = maxTurnRate_ * 1.5f; // 性能限界を超えた旋回

		// 現在方向と目標方向の間を補間
		Vector3 newDir = currentDir + ( targetDir - currentDir ) * finalTurnRate;
		newDir = Normalize(newDir);

		// 急加速して直進
		float finalSpeed = kBaseFinalSpeed +
			std::min(kMaxFinalSpeedIncrease, stateTimer_ / kFinalSpeedIncreaseDivider);
		velocity_ = newDir * finalSpeed;

		// ミサイル性能のランダムばらつきを適用
		// 性能ばらつきにより、完全命中しない場合もある
		if(stateTimer_ > 10) { // 最初の数フレームは正確に誘導
			float randomDeviation = ( rand() % 100 - 50 ) / 2000.0f;
			velocity_.x += randomDeviation;
			velocity_.y += randomDeviation;
		}
	} else {
		// ターゲットがない場合は現在の速度で直進
		float finalSpeed = kBaseFinalSpeed +
			std::min(kMaxFinalSpeedIncrease, stateTimer_ / kFinalSpeedIncreaseDivider);
		velocity_ = Normalize(velocity_) * finalSpeed;
	}
}

// 視線ベクトル（LOS: Line of Sight）の計算
Vector3 PlayerMissile::CalculateLOS() {
	if(!target_ || target_->GetHp() <= 0) {
		return Normalize(velocity_); // ターゲットがない場合は現在の進行方向
	}

	// ミサイルからターゲットへのベクトル
	Vector3 los = target_->GetPosition() - worldTransform_->transform.translate;
	return los;
}

// 視線角速度（LOS Rate）の計算
Vector3 PlayerMissile::CalculateLOSRate() {
	if(!target_ || target_->GetHp() <= 0) {
		return Vector3{ 0, 0, 0 }; // ターゲットがない場合はゼロ
	}

	// 現在の視線ベクトル
	Vector3 currentLOS = CalculateLOS();

	// 前回の視線ベクトルとの差分から角速度を近似
	Vector3 losRate = currentLOS - prevLineOfSight_;

	// 前回の視線ベクトルを更新
	prevLineOfSight_ = currentLOS;

	return losRate;
}

// 比例航法ベクトルの計算（PN: Proportional Navigation）
Vector3 PlayerMissile::CalculateNavigationVector() {
	if(!target_ || target_->GetHp() <= 0) {
		return Vector3{ 0, 0, 0 };
	}

	// 視線角速度を取得
	Vector3 losRate = CalculateLOSRate();

	// 速度ベクトルとの外積でPN方向を計算
	// a_missile = N * (v_missile × ω_LOS)
	// 比例航法のゲインを下げる（より緩やかな追尾に）
	float reducedNavGain = kNavigationGain * 0.6f;  // 60%に低減
	Vector3 pnAccel = Cross(velocity_, losRate) * reducedNavGain;

	// 現在の速度方向
	Vector3 currentDir = Normalize(velocity_);

	// 視線方向の単位ベクトル
	Vector3 losUnit = Normalize(CalculateLOS());

	// 相対速度から視線方向成分を除去（横方向加速度成分の計算）
	Vector3 velocityPerp;
	velocityPerp.x = velocity_.x - Dot(velocity_, losUnit) * losUnit.x;
	velocityPerp.y = velocity_.y - Dot(velocity_, losUnit) * losUnit.y;
	velocityPerp.z = velocity_.z - Dot(velocity_, losUnit) * losUnit.z;

	// 曲線的な軌道を描くための補正（より緩やかに）
	float curveFactor = sinf(stateTimer_ * ( kCurveOscillationSpeed * 0.75f )) * ( kCurveStrength * 0.75f );
	Vector3 curveVector = Cross(losUnit, Vector3{ 0, 1, 0 }) * curveFactor;

	// 操舵方向を計算（垂直方向成分の影響度を下げる）
	Vector3 steeringDir = Normalize(losUnit + velocityPerp * ( kVelocityPerpWeight * 0.7f ) + curveVector);

	// 操舵力を計算（現在の方向から目標方向への変化）
	// navigationGain_も下げることで、より滑らかな動きに
	Vector3 steeringForce = ( steeringDir - currentDir ) * ( navigationGain_ * 0.75f );

	// 比例航法と操舵力を組み合わせた最終的な誘導力
	return pnAccel + steeringForce;
}

// 近接信管の処理
void PlayerMissile::ApplyProximityFuse() {
	if(!target_ || target_->GetHp() <= 0 || proximityFused_) {
		return;
	}

	// ターゲットとの距離を計算
	Vector3 targetPos = target_->GetPosition();
	Vector3 myPos = worldTransform_->transform.translate;
	float distance = Length(targetPos - myPos);

	// 近接信管の発動距離内に入った場合
	if(distance < kProximityFuseDistance) {
		proximityFused_ = true;
		// ここで爆発エフェクト等を発生させる処理を追加できます
		// 例: CreateExplosionEffect();

		isActive_ = false; // ミサイルを非アクティブ化
	}
}

void PlayerMissile::Draw(ViewProjection viewProjection, DirectionalLight directionalLight,
	PointLight pointLight, SpotLight spotLight) {
// アクティブな場合のみ描画
	if(isActive_) {
		// 弾を進行方向に向ける
		if(Length(velocity_) > 0.01f) {
			Vector3 direction = Normalize(velocity_);
			// Y軸回転角を計算
			float rotY = atan2f(direction.x, direction.z);
			// X軸回転角を計算（上下の角度）
			float xzLength = sqrtf(direction.x * direction.x + direction.z * direction.z);
			float rotX = -atan2f(direction.y, xzLength);

			worldTransform_->transform.rotate = { rotX, rotY, 0.0f };
		}
	}
	// モデルの描画
	model_->Draw(*worldTransform_.get(), viewProjection, directionalLight,
		pointLight, spotLight);
}

///=============================================================================
///						当たり判定
///--------------------------------------------------------------
///						接触開始処理
void PlayerMissile::OnCollisionEnter(BaseObject *other) {
	//========================================
	// 敵接触
	if(Enemy *enemy = dynamic_cast<Enemy *>( other )) {
		//---------------------------------------
		//弾を消す
		isActive_ = false;

		// 接触時に爆発エフェクトを発生させるなど
		// 追加の処理をここに実装できます
	}
}

///--------------------------------------------------------------
///						接触継続処理
void PlayerMissile::OnCollisionStay(BaseObject *other) {
	// 継続的な衝突処理が必要な場合はここに実装
}

///--------------------------------------------------------------
///						接触終了処理
void PlayerMissile::OnCollisionExit(BaseObject *other) {
	// 衝突終了時の処理が必要な場合はここに実装
}