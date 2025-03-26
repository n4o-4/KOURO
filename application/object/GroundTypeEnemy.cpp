#include "GroundTypeEnemy.h"
#include "PlayerMissile.h"
#include <cmath>
#include <algorithm>
#include "PlayerMachineGun.h"

void GroundTypeEnemy::Initialize() {
	ModelManager::GetInstance()->LoadModel("enemy/kumo/kumo.obj");
	BaseEnemy::Initialize(ModelManager::GetInstance()->FindModel("enemy/kumo/kumo.obj"));

}

void GroundTypeEnemy::Update() {
	if (hp_ > 0) {
		// 状態の更新
		UpdateActionState();

		// 現在の状態に応じた行動
		switch (currentState_) {
		case ActionState::Wander:
			UpdateWanderState();
			break;
		case ActionState::Chase:
			UpdateChaseState();
			break;
		case ActionState::Combat:
			UpdateCombatState();
			break;
		}
	}


	intervalCounter_ += 1.0f / 60.0f;
	if (kIntervalTiem <= intervalCounter_ && currentState_ == ActionState::Combat) {
		Fire();
		intervalCounter_ = fmod(intervalCounter_, kIntervalTiem);
	}

	if (isBlinking_) {
		blinkTimer_ += 1.0f / 60.0f;

		// 点滅：0.05秒ごとに完全透明・通常色を切り替える
		int flashFrame = static_cast<int>(blinkTimer_ * 60.0f) % 2;
		if (flashFrame == 0) {
			model_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }); // 通常色（不透明）
		} else {
			model_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // 完全透明
		}

		// 点滅終了
		if (blinkTimer_ >= kBlinkDuration_) {
			isBlinking_ = false;
			model_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }); // 戻す
		}
	}

	BaseEnemy::Update();
}

void GroundTypeEnemy::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	BaseEnemy::Draw(viewProjection, directionalLight, pointLight, spotLight);
}

void GroundTypeEnemy::MoveToJump() {
	if (target_) {

		Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
		toTarget.y = 0.0f; // 高さ方向の距離は無視
		float distance = Length(toTarget);
		Vector3 direction = Normalize(toTarget);


		if (!isJumping_) {
			isJumping_ = true;
			jumpVelocity_ = 1.2f;
			jumpTime_ = 0.0f;
		}


		jumpTime_ += 1.0f / 1200.0f;
		jumpVelocity_ -= gravity_ * jumpTime_;


		Vector3 moveDirection = direction * speed_;
		moveDirection.y = jumpVelocity_;


		worldTransform_->transform.translate = worldTransform_->transform.translate + moveDirection;


		if (worldTransform_->transform.translate.y <= groundHeight_) {
			worldTransform_->transform.translate.y = groundHeight_;
			isJumping_ = false;
			jumpVelocity_ = 0.0f;
		}


	}

}

void GroundTypeEnemy::Attack() {
	if (target_) {





	}
}

void GroundTypeEnemy::OnCollisionEnter(BaseObject* other) {
	if (dynamic_cast<PlayerMissile*>(other)) {
		--hp_;
		HitJump();
	}
	if (dynamic_cast<PlayerMachineGun*>(other)) {
		--hp_;
		HitJump();
	}
}

void GroundTypeEnemy::OnCollisionStay(BaseObject* other) {
}

void GroundTypeEnemy::OnCollisionExit(BaseObject* other) {
}

void GroundTypeEnemy::HitJump()
{
	isBlinking_ = true;
	blinkTimer_ = 0.0f;

	// 点滅開始時に白色にする（仮に SetColor 関数があると仮定）
	model_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }); // 真っ白（光った感じ）
}

void GroundTypeEnemy::UpdateWanderState() {

	directionChangeTimer_ += 1.0f / 60.0f;


	if (worldTransform_->transform.translate.y > groundHeight_) {
		jumpVelocity_ -= gravity_ * (1.0f / 60.0f);
	} else {
		jumpVelocity_ = 0.0f;
		worldTransform_->transform.translate.y = groundHeight_;
	}


	if (directionChangeTimer_ >= directionChangeInterval_) {
		Vector3 toSpawn = spawnPosition_ - worldTransform_->transform.translate;
		float distanceToSpawn = Length(toSpawn);
		float spawnWeight = std::min(distanceToSpawn / wanderRadius_, 0.8f);

		if (distanceToSpawn > wanderRadius_) {

			velocity_ = Normalize(toSpawn) * speed_;
		} else {

			float angle = angleDist_(rng_);
			Vector3 randomDir = { cosf(angle), 0.0f, sinf(angle) };
			velocity_ = Normalize(randomDir) * speed_;
		}

		directionChangeTimer_ = 0.0f;
	}


	velocity_.y = jumpVelocity_;
	worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;


	if (Length(velocity_) > 0.01f) {
		float targetRotationY = std::atan2(velocity_.x, velocity_.z);
		worldTransform_->transform.rotate.y = targetRotationY;
	}
}

void GroundTypeEnemy::UpdateChaseState() {
	MoveToJump();
}

void GroundTypeEnemy::UpdateCombatState() {
	if (target_) {
		// ターゲットに向かうベクトルを計算
		Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
		float distance = Length(toTarget);
		Vector3 direction = Normalize(toTarget);

		// ストラッフィング（横移動）のベクトルを計算
		Vector3 strafeDir = { direction.z, 0.0f, -direction.x }; // 90度回転

		// 時間とともに方向を変えるための係数（-1から1の範囲）
		float strafeFactor = sinf(stateTimer_ * 0.5f);

		// 距離に基づいて行動を決定
		Vector3 moveDirection;

		if (distance < safeDistance_) {
			// 近すぎる場合、後退
			//moveDirection = -direction * 0.7f + strafeDir * strafeFactor * 0.3f;
			moveDirection.x = -direction.x * 0.7f + strafeDir.x * strafeFactor * 0.3f;
			moveDirection.z = -direction.z * 0.7f + strafeDir.z * strafeFactor * 0.3f;
		} else if (distance > combatDistance_) {
			// 遠すぎる場合、接近
			moveDirection = direction * 0.7f + strafeDir * strafeFactor * 0.3f;
		} else {
			// 適切な距離の場合、ストラッフィングのみ
			moveDirection = strafeDir * strafeFactor;
		}

		if (worldTransform_->transform.translate.y > groundHeight_) {
			jumpTime_ += 2.0f;
			jumpVelocity_ -= gravity_ * jumpTime_;
		} else {
			jumpVelocity_ = 0.0f;
			worldTransform_->transform.translate.y = groundHeight_;
		}

		moveDirection.y = jumpVelocity_;

		// 速度を更新
		velocity_ = Normalize(moveDirection) * speed_ * 2.8f;

		// 位置を更新
		worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;

		// 常にターゲットの方を向く
		float targetRotationY = std::atan2(direction.x, direction.z);
		worldTransform_->transform.rotate.y = targetRotationY;
	}
}

void GroundTypeEnemy::UpdateActionState() {
	// タイマーの更新
	stateTimer_ += 1.0f / 60.0f;

	// ターゲットが存在しない場合は徘徊
	if (!target_) {
		currentState_ = ActionState::Wander;
		return;
	}

	// ターゲットとの距離を計算
	Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
	float distance = Length(toTarget);

	// 距離に基づいて状態を変更
	ActionState newState = currentState_; // デフォルトは現在の状態を維持

	if (distance > chaseDistance_) {
		// プレイヤーが遠い場合は徘徊
		newState = ActionState::Wander;
	} else if (distance > combatDistance_) {
		// 追跡範囲内なら追跡
		newState = ActionState::Chase;
	} else {
		// 戦闘範囲内なら戦闘
		newState = ActionState::Combat;
	}

	// 状態が変化した場合、タイマーをリセット
	if (newState != currentState_) {
		currentState_ = newState;
		stateTimer_ = 0.0f;
	}
}
