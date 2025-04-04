#include "GroundTypeEnemy2.h"
#include <PlayerMissile.h>
#include <PlayerMachineGun.h>

void GroundTypeEnemy2::Initialize() {
	ModelManager::GetInstance()->LoadModel("enemy/bomb/bomb.obj");
	BaseEnemy::Initialize(ModelManager::GetInstance()->FindModel("enemy/bomb/bomb.obj"));

	startScale_.x = startScale_.y = startScale_.z = 1.0f;
	targetScale_.x = targetScale_.y = targetScale_.z = 1.0f;

	particleEmitter_->SetParticleCount(20);
	particleEmitter_->SetLifeTimeRange(ParticleManager::LifeTimeRange({ 1.0f,1.0f }));
	particleEmitter_->SetVelocityRange(ParticleManager::VelocityRange({ -3.0f,3.0f }, { -3.0f,3.0f }, { -3.0f,3.0f }));
}

void GroundTypeEnemy2::Update() {
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



	BaseEnemy::Update();
}

void GroundTypeEnemy2::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	BaseEnemy::Draw(viewProjection, directionalLight, pointLight, spotLight);
}



void GroundTypeEnemy2::Attack() {
}

void GroundTypeEnemy2::OnCollisionEnter(BaseObject* other) {
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

void GroundTypeEnemy2::OnCollisionStay(BaseObject* other) {
}

void GroundTypeEnemy2::OnCollisionExit(BaseObject* other) {
}

void GroundTypeEnemy2::HitJump()
{
	isHitReacting_ = true;
	hitReactionTimer_ = 0.0f;
	startScale_ = worldTransform_->transform.scale;
	targetScale_ = { 1.7f, 1.7f, 1.7f };   // 一時的に大きくする
}

void GroundTypeEnemy2::UpdateWanderState() {
	BaseEnemy::RandomMove();
}

void GroundTypeEnemy2::UpdateChaseState() {
	if (target_) {
		// ターゲットに向かうベクトルを計算
		Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
		float distance = Length(toTarget);

		Vector3 direction = Normalize(toTarget);
		velocity_ = { direction.x * speed_,0.0f,direction.z * speed_ };

		//
		worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;

		//
		float targetRotationY = std::atan2(direction.x, direction.z);
		worldTransform_->transform.rotate.y = targetRotationY;

		SetModelColor(Vector4{ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

void GroundTypeEnemy2::UpdateCombatState() {
	if (target_) {
		// 
		Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
		Vector3 direction = Normalize(toTarget);

		//
		float dashSpeed = speed_ * 3.0f;
		velocity_ = { direction.x * dashSpeed,0.0f,direction.z * dashSpeed };

		//
		worldTransform_->transform.translate += velocity_;

		// 
		float targetRotationY = std::atan2(direction.x, direction.z);
		worldTransform_->transform.rotate.y = targetRotationY;

		SetModelColor(Vector4{ 1.0f, 0.0f, 0.0f, 1.0f });
	}
}

void GroundTypeEnemy2::UpdateActionState() {
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
