#define _USE_MATH_DEFINES
#define NOMINMAX
#include "FollowCamera.h"
#include "BaseEnemy.h"
#include "Input.h"
#include "MyMath.h"
#include <algorithm>
#include <cmath>

///=============================================================================
///                        初期化
void FollowCamera::Initialize() {
	// カメラの初期オフセット設定
	offset_ = defaultOffset_;

	// 基底クラスの初期化
	BaseCamera::Initialize();

	// 初期位置を設定
	if (target_) {
		currentPosition_ = target_->transform.translate + offset_;
		viewProjection_->transform.translate = currentPosition_;
	}
	// 軽く傾ける
	viewProjection_->transform.rotate = {0.1f, 0.0f, 0.0f};
}

///=============================================================================
///                        更新
void FollowCamera::Update() {
	// ターゲットが設定されていない場合はエラー
	if (target_ == nullptr) {
		assert(0);
	}

	// エイムアシスト処理
	UpdateAimAssist();

	// オフセットの計算
	CalculationRotate();

	// 回転の計算
	CalculationTranslate();

	// 高度によるFOV演出
	UpdateHeightFOVEffect();

	// マシンガン発射時のカメラ演出を更新
	UpdateFiringCameraEffect();

	// ミサイル発射時のカメラ演出を更新
	UpdateMissileFiringCameraEffect();

	// 基底クラスの更新
	BaseCamera::Update();
}

///=============================================================================
///                        前方向ベクトルを取得
Vector3 FollowCamera::GetForwardDirection() const {
	// カメラの回転行列から前方向ベクトルを計算
	Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->transform.rotate);
	Vector3 forward = {0.0f, 0.0f, 1.0f}; // デフォルトの前方向
	return TransformNormal(forward, rotateMatrix);
}

///=============================================================================
///                        カメラからの右方向ベクトルを取得
Vector3 FollowCamera::GetRightDirection() const {
	// カメラの回転行列から右方向ベクトルを計算
	Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->transform.rotate);
	Vector3 right = {1.0f, 0.0f, 0.0f}; // デフォルトの右方向
	return TransformNormal(right, rotateMatrix);
}

///=============================================================================
///                        カメラからの上方向ベクトルを取得
Vector3 FollowCamera::GetUpDirection() const {
	// カメラの回転行列から上方向ベクトルを計算
	Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->transform.rotate);
	Vector3 up = {0.0f, 1.0f, 0.0f}; // デフォルトの上方向
	return TransformNormal(up, rotateMatrix);
}

///=============================================================================
///                        オフセットの計算
Vector3 FollowCamera::CalculationOffset() {
	Vector3 offset = offset_;

	Matrix4x4 rotateMatrix = MakeRotateMatrix(viewProjection_->transform.rotate);

	offset = TransformNormal(offset, rotateMatrix);

	return offset;
}

///=============================================================================
///                        回転の計算
void FollowCamera::CalculationRotate() {
	/// 右スティック傾きを取得
	Vector2 rightStickVector = Input::GetInstance()->GetRightStick();

	// プレイヤーの入力がある場合、エイムアシストを弱める
	if (std::abs(rightStickVector.x) > assistInputTriggerThreshold_ ||
		std::abs(rightStickVector.y) > assistInputTriggerThreshold_) {
		DecayAimAssist();
		timeSinceLastInput_ = 0.0f;
	} else {
		// 入力がない時間を更新
		timeSinceLastInput_ += 1.0f / 60.0f; // 60FPSを想定
	}

	Vector2 rotate = {-rightStickVector.y * rotateSpeed_, rightStickVector.x * rotateSpeed_};

	destinationRotate = {destinationRotate.x + rotate.x, destinationRotate.y + rotate.y, destinationRotate.z};

	viewProjection_->transform.rotate = Lerp(viewProjection_->transform.rotate, destinationRotate, easingFactor_);

	// 回転Xを範囲制限
	float clampedX = std::clamp(viewProjection_->transform.rotate.x, -0.8f, 1.5f);
	if (clampedX != viewProjection_->transform.rotate.x) {
		destinationRotate.x = clampedX;
	}
	viewProjection_->transform.rotate.x = clampedX;
}

///=============================================================================
///                        位置の計算
void FollowCamera::CalculationTranslate() {
	interTarget_ = Lerp(interTarget_, target_->transform.translate, easingFactor_);

	Vector3 offset = CalculationOffset();

	viewProjection_->transform.translate = interTarget_ + offset;

	/// カメラの角度の修正が必要無ければ
	if (viewProjection_->transform.translate.y >= 0.1f) {
		return;
	}

	// カメラの高さが0.1以下なら、下方向（上向き）への回転を制限
	while (viewProjection_->transform.translate.y <= 0.1f) {
		// 下向きすぎないように制限（上を向かせすぎない）
		// destinationRotate.x = std::max(destinationRotate.x, 0.1f);

		viewProjection_->transform.rotate.x += 0.001f;

		interTarget_ = Lerp(interTarget_, target_->transform.translate, easingFactor_);

		Vector3 offset = CalculationOffset();

		viewProjection_->transform.translate = interTarget_ + offset;
	}

	// 目標角度を修正
	destinationRotate.x = viewProjection_->transform.rotate.x;
}

///=============================================================================
///                        高度によるFOV演出
void FollowCamera::UpdateHeightFOVEffect() {
	// ターゲットが設定されていない場合は処理しない
	if (!target_)
		return;

	// プレイヤーの高度を取得
	float playerHeight = target_->transform.translate.y;

	// 高度に基づいたFOV調整（Y座標が0.0以上の場合）
	if (playerHeight >= 0.0f) {
		// 高度に応じてFOV増加量を計算
		// 高度が高いほどFOVを広げる（最大でmaxHeightForFOV_まで考慮）
		float heightFactor = std::min(playerHeight / maxHeightForFOV_, 1.0f);

		// 高度に応じたFOV増加を計算
		float fovIncrease = heightFOVIncrease_ * heightFactor;
		targetFOV_ = baseFOV_ + fovIncrease;
	} else {
		// 地上（Y < 0.0）では基本FOVに戻す
		targetFOV_ = baseFOV_;
	}

	// 現在のFOVを目標値へ滑らかに補間
	currentFOV_ = currentFOV_ + (targetFOV_ - currentFOV_) * fovLerpSpeed_;

	// フォールバックチェック（極端な値にならないよう制限）
	currentFOV_ = std::max(baseFOV_, std::min(currentFOV_, baseFOV_ + heightFOVIncrease_));

	// FOVをカメラに適用（ラジアンへの変換が必要な場合）
	viewProjection_->fovY = currentFOV_ * (3.14159f / 180.0f);

	// 現在の高度を保存（必要に応じて）
	prevPlayerHeight_ = playerHeight;
}

///=============================================================================
///                        マシンガン発射時のカメラ演出を更新
void FollowCamera::UpdateFiringCameraEffect() {
	// ミサイル発射中はマシンガン発射中の演出を無効化
	if (isMissileFiring_)
		return;

	Vector3 targetOffset = defaultOffset_;

	// マシンガン発射中はカメラを近づける
	// RTボタンが押されている間だけ近づける
	if (Input::GetInstance()->PushKey(DIK_J) ||
		Input::GetInstance()->PushGamePadButton(Input::GamePadButton::LEFT_SHOULDER)) {
		// Z方向（距離）のみ調整して近づける
		targetOffset.z = defaultOffset_.z * firingOffsetFactor_;
	}

	// 現在のオフセットを目標値に滑らかに補間
	offset_ = Lerp(offset_, targetOffset, firingLerpSpeed_);
}

///=============================================================================
///                        ミサイル発射時のカメラ演出を更新
void FollowCamera::UpdateMissileFiringCameraEffect() {
	Vector3 targetOffset = defaultOffset_;

	// ミサイル発射ボタンが押された時に発射状態を開始
	if (Input::GetInstance()->PushKey(DIK_SPACE) ||
		Input::GetInstance()->PushGamePadButton(Input::GamePadButton::RIGHT_SHOULDER)) {
		isMissileFiring_ = true;
		missileFireTimer_ = 0.0f; // タイマーをリセット
	}

	// ミサイル発射中の処理（数秒間継続）
	if (isMissileFiring_) {
		// カメラを遠ざける
		targetOffset.z = defaultOffset_.z * missileFiringOffsetFactor_;

		// タイマーを進める（60FPSを想定）
		missileFireTimer_ += 1.0f / 60.0f;

		// 持続時間が経過したら演出を終了
		if (missileFireTimer_ >= missileFireDuration_) {
			isMissileFiring_ = false;
		}
	}

	// 現在のオフセットを目標値に滑らかに補間
	offset_ = Lerp(offset_, targetOffset, missileFiringLerpSpeed_);
}

///=============================================================================
///                        エイムアシスト処理
void FollowCamera::UpdateAimAssist() {
	// エイムアシストが無効または敵リストが存在しない場合は処理しない
	if (aimAssistFactor_ <= 0.0f || !enemies_ || !spawns_) {
		return;
	}

	// 最も近い敵を見つける
	BaseEnemy *nearestEnemy = FindNearestEnemyInSight();

	// アシスト対象が変わった場合、強度をリセット
	if (nearestEnemy != currentAimTarget_) {
		currentAimTarget_ = nearestEnemy;
		currentAimAssistStrength_ = 0.0f;
	}

	// アシスト対象が存在しない場合は終了
	if (!currentAimTarget_) {
		return;
	}

	// プレイヤーからターゲットへの方向を計算
	Vector3 targetPos = currentAimTarget_->GetPosition();
	Vector3 playerPos = target_->transform.translate;
	Vector3 toTarget = targetPos - playerPos;

	// Y軸回転（水平方向）の目標角度を計算
	float targetRotationY = std::atan2(toTarget.x, toTarget.z);

	// X軸回転（垂直方向）の目標角度を計算
	float distanceXZ = std::sqrt(toTarget.x * toTarget.x + toTarget.z * toTarget.z);
	float targetRotationX = -std::atan2(toTarget.y, distanceXZ);

	// 入力がないとき、徐々にアシスト強度を上げる
	if (timeSinceLastInput_ > 0.0f && timeSinceLastInput_ < assistRampUpTime_) {
		currentAimAssistStrength_ = std::min(1.0f, timeSinceLastInput_ / assistRampUpTime_);
	} else if (timeSinceLastInput_ >= assistRampUpTime_) {
		currentAimAssistStrength_ = 1.0f;
	}

	// エイムアシストの強さを適用
	float effectiveAssistStrength = currentAimAssistStrength_ * aimAssistFactor_;

	// 現在の角度と目標角度の差分を計算
	float deltaY = targetRotationY - destinationRotate.y;
	// -πからπの範囲に正規化
	deltaY = std::fmod(deltaY + static_cast<float>(M_PI), 2.0f * static_cast<float>(M_PI)) - static_cast<float>(M_PI);

	float deltaX = targetRotationX - destinationRotate.x;

	// 目標との角度差が大きいほど、アシスト効果を弱める（自然な感覚に）
	float angleFactorY = 1.0f;
	float absAngleY = std::abs(deltaY * (180.0f / static_cast<float>(M_PI))); // 度に変換
	if (absAngleY > assistInnerAngle_) {
		// 内側範囲を超えると強度が徐々に下がる
		angleFactorY = std::max(0.0f, 1.0f - ((absAngleY - assistInnerAngle_) / (aimAssistAngle_ - assistInnerAngle_)));
	}

	float angleFactorX = 1.0f;
	float absAngleX = std::abs(deltaX * (180.0f / static_cast<float>(M_PI))); // 度に変換
	if (absAngleX > assistInnerAngle_) {
		angleFactorX = std::max(0.0f, 1.0f - ((absAngleX - assistInnerAngle_) / (aimAssistAngle_ - assistInnerAngle_)));
	}

	// 最終的なアシスト回転量を計算
	float assistRotationY = deltaY * aimAssistSpeed_ * effectiveAssistStrength * angleFactorY;
	float assistRotationX = deltaX * aimAssistSpeed_ * effectiveAssistStrength * angleFactorX;

	// アシスト効果を目標角度に適用
	destinationRotate.y += assistRotationY;
	destinationRotate.x += assistRotationX;

	// X回転を範囲内に制限
	destinationRotate.x = std::clamp(destinationRotate.x, -0.8f, 1.5f);
}

///=============================================================================
///                        最も近い敵を見つける
BaseEnemy *FollowCamera::FindNearestEnemyInSight() {
	if (!enemies_ || !spawns_) {
		return nullptr;
	}

	BaseEnemy *nearestEnemy = nullptr;
	float nearestDistance = FLT_MAX;
	float nearestAngle = 180.0f; // 180度（最大値）で初期化

	// プレイヤー位置
	Vector3 playerPos = target_->transform.translate;

	// カメラの前方向ベクトル
	Vector3 cameraForward = GetForwardDirection();

	// 全ての敵をチェック
	auto checkEnemy = [&](BaseEnemy *enemy) {
		if (enemy && enemy->GetHp() > 0) {
			Vector3 enemyPos = enemy->GetPosition();
			Vector3 toEnemy = enemyPos - playerPos;
			float distance = Length(toEnemy);

			// 距離が範囲内か
			if (distance < aimAssistRange_) {
				// カメラの前方向と敵への方向の角度を計算
				Vector3 toEnemyNorm = Normalize(toEnemy);
				float dotProduct = Dot(cameraForward, toEnemyNorm);
				float angle = std::acos(std::clamp(dotProduct, -1.0f, 1.0f)) * (180.0f / static_cast<float>(M_PI)); // ラジアンから度に変換

				// 視野角内かつ最も近い敵を選定
				if (angle < aimAssistAngle_) {
					// 距離と角度を組み合わせたスコアを計算
					// 角度が小さいほど優先度が高い
					float angleWeight = 2.0f; // 角度の重要度（調整可能）
					float score = distance + (angle * angleWeight);

					if (score < nearestDistance) {
						nearestDistance = score;
						nearestAngle = angle;
						nearestEnemy = enemy;
					}
				}
			}
		}
	};

	// 通常の敵をチェック
	if (enemies_) {
		for (const auto &enemyPtr : *enemies_) {
			checkEnemy(enemyPtr.get());
		}
	}

	// スポーン敵をチェック
	if (spawns_) {
		for (const auto &spawnPtr : *spawns_) {
			checkEnemy(spawnPtr.get());
		}
	}

	return nearestEnemy;
}

///=============================================================================
///                        エイムアシストの減衰（プレイヤー入力時）
void FollowCamera::DecayAimAssist() {
	currentAimAssistStrength_ *= aimAssistDecayRate_;
	if (currentAimAssistStrength_ < 0.01f) {
		currentAimAssistStrength_ = 0.0f;
	}
}

///=============================================================================
///                        敵がエイムアシスト対象となる角度内かチェック
bool FollowCamera::IsEnemyInAssistRange(const Vector3 &enemyPos) {
	// プレイヤーの位置
	Vector3 playerPos = target_->transform.translate;

	// カメラの前方向
	Vector3 cameraForward = GetForwardDirection();

	// プレイヤーから敵への方向
	Vector3 toEnemy = enemyPos - playerPos;
	float distance = Length(toEnemy);

	// 距離が範囲外なら対象外
	if (distance > aimAssistRange_) {
		return false;
	}

	// 正規化
	Vector3 toEnemyNorm = {0.0f, 0.0f, 0.0f};
	if (distance > 0.001f) { // ゼロ除算を避ける
		toEnemyNorm.x = toEnemy.x / distance;
		toEnemyNorm.y = toEnemy.y / distance;
		toEnemyNorm.z = toEnemy.z / distance;
	}

	// カメラの前方向と敵への方向のドット積から角度を算出
	float dotProduct = Dot(cameraForward, toEnemyNorm);
	float angle = std::acos(std::clamp(dotProduct, -1.0f, 1.0f)) * (180.0f / static_cast<float>(M_PI)); // ラジアンから度に変換

	// 視野角内かどうか
	return angle <= aimAssistAngle_;
}