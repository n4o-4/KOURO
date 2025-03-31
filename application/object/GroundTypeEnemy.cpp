#include "GroundTypeEnemy.h"
#include "PlayerMissile.h"
#include <cmath>
#include <algorithm>
#include "PlayerMachineGun.h"

void GroundTypeEnemy::Initialize() {
	ModelManager::GetInstance()->LoadModel("enemy/kumo/kumo.obj");
	BaseEnemy::Initialize(ModelManager::GetInstance()->FindModel("enemy/kumo/kumo.obj"));

	startScale_.x = startScale_.y = startScale_.z = 1.0f;
	targetScale_.x = targetScale_.y = targetScale_.z = 1.0f;

	particleEmitter_->SetParticleCount(20);
	particleEmitter_->SetLifeTimeRange(ParticleManager::LifeTimeRange({ 1.0f,1.0f }));
	particleEmitter_->SetVelocityRange(ParticleManager::VelocityRange({ -3.0f,3.0f }, { -3.0f,3.0f }, { -3.0f,3.0f }));
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

	if (isHitReacting_) {
		hitReactionTimer_ += 1.0f / 60.0f;

		const float kTotalReactionTime = 0.3f; // 全体時間
		float t = hitReactionTimer_ / kTotalReactionTime;

		// clamp
		if (t > 1.0f) t = 1.0f;

		// イージング（バウンド風：t=0で1.0、t=0.5で1.7、t=1.0で1.0）
		float scaleFactor = 1.0f + 0.7f * sinf(t * 3.141592f); // πで1周期 → 1→1.7→1

		worldTransform_->transform.scale.x = scaleFactor;
		worldTransform_->transform.scale.y = scaleFactor;
		worldTransform_->transform.scale.z = scaleFactor;

		if (t >= 1.0f) {
			isHitReacting_ = false;
			hitReactionTimer_ = 0.0f;
			worldTransform_->transform.scale.x = 1.0f;
			worldTransform_->transform.scale.y = 1.0f;
			worldTransform_->transform.scale.z = 1.0f;
		}
	}

	std::vector<GroundTypeEnemy*> sameTypeEnemies;
	for (const auto& enemy : *BaseEnemy::s_allEnemies) {
		if (auto* same = dynamic_cast<GroundTypeEnemy*>(enemy.get())) {
			sameTypeEnemies.push_back(same);
		}
	}
	ApplySeparationFromOthers(sameTypeEnemies);

	BaseEnemy::Update();
}

void GroundTypeEnemy::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	BaseEnemy::Draw(viewProjection, directionalLight, pointLight, spotLight);
}

void GroundTypeEnemy::MoveToJump() {
	if (!target_) return;

	// ジャンプクールダウンタイマーの更新
	if (jumpCooldownTimer_ < kJumpInterval_) {
		jumpCooldownTimer_ += 1.0f / 60.0f;
		RandomWanderMove(); 
		return;
	}

	Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
	toTarget.y = 0.0f;
	float distance = Length(toTarget);
	Vector3 direction = Normalize(toTarget);

	// ジャンプ条件：現在ジャンプ中ではなく、クールダウンが過ぎたとき
	if (!isJumping_ && jumpCooldownTimer_ >= kJumpInterval_) {
		isJumping_ = true;
		jumpVelocity_ = 2.3f;
		jumpTime_ = 0.0f;
		jumpCooldownTimer_ = 0.0f; // クールダウンの初期化
	}

	// ジャンプ処理
	if (isJumping_) {
		jumpTime_ += 1.0f / 60.0f;
		jumpVelocity_ -= gravity_ * 0.2f * jumpTime_; // 落下速度少し遅い

		Vector3 moveDirection = direction * speed_;
		moveDirection.y = jumpVelocity_;

		worldTransform_->transform.translate += moveDirection;

		if (worldTransform_->transform.translate.y <= groundHeight_) {
			worldTransform_->transform.translate.y = groundHeight_;
			isJumping_ = false;
			jumpVelocity_ = 0.0f;
		}
	}

}

void GroundTypeEnemy::ApplySeparationFromOthers(const std::vector<GroundTypeEnemy*>& others) {
	Vector3 separationForce = { 0.0f, 0.0f, 0.0f };
	const float separationDistance = 2.0f; // 近いと遠くなる距離

	for (const auto& other : others) {
		if (other == this) continue;

		Vector3 toOther = worldTransform_->transform.translate - other->GetPosition();
		float dist = Length(toOther);

		if (dist < separationDistance && dist > 0.001f) {
			separationForce = separationForce + Normalize(toOther) * (separationDistance - dist);
		}
	}

	// この力を現在の速度に弱く加える
	velocity_ = velocity_ + separationForce * 0.01f;
}

void GroundTypeEnemy::Attack() {
	if (target_) {





	}
}

void GroundTypeEnemy::OnCollisionEnter(BaseObject* other) {
	if (dynamic_cast<PlayerMissile*>(other)) {
		--hp_;
		HitJump();
		particleEmitter_->Emit();
	}
	if (dynamic_cast<PlayerMachineGun*>(other)) {
		--hp_;
		HitJump();
		particleEmitter_->Emit();
	}
}

void GroundTypeEnemy::OnCollisionStay(BaseObject* other) {
}

void GroundTypeEnemy::OnCollisionExit(BaseObject* other) {
}

void GroundTypeEnemy::HitJump()
{
	isHitReacting_ = true;
	hitReactionTimer_ = 0.0f;
	startScale_ = worldTransform_->transform.scale;
	targetScale_ = { 1.7f, 1.7f, 1.7f };  // 一時的に大きくする
}

void GroundTypeEnemy::RandomWanderMove() {
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

void GroundTypeEnemy::UpdateWanderState() {
	RandomWanderMove();
	
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
