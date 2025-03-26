#include "SkyTypeEnemy.h"
#include "PlayerMissile.h"
#include <PlayerMachineGun.h>
void SkyTypeEnemy::Initialize() {
	ModelManager::GetInstance()->LoadModel("enemy/bat/bat.obj");
	BaseEnemy::Initialize(ModelManager::GetInstance()->FindModel("enemy/bat/bat.obj"));


}

void SkyTypeEnemy::Update() {
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

void SkyTypeEnemy::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	BaseEnemy::Draw(viewProjection, directionalLight, pointLight, spotLight);
}

void SkyTypeEnemy::MoveToRush() {
	if (target_) {
		Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
		Vector3 direction = Normalize(toTarget);

		// 
		velocity_ = direction * rushSpeed_ * rushSpeedMultiplier_;

		// 
		worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;

		// 
		float targetRotationY = std::atan2(direction.x, direction.z);
		worldTransform_->transform.rotate.y = targetRotationY;

		// 
		rushTimer_ += 1.0f / 60.0f;
		if (rushTimer_ >= rushDuration_) {
			isRushing_ = false; // 
			isRising_ = true;   // 
			rushTimer_ = 0.0f;
			riseTimer_ = 0.0f;  // 
		}
	}
}

void SkyTypeEnemy::MoveToRise() {
	// 
	velocity_.y = riseSpeed_;

	// 
	worldTransform_->transform.translate.y += velocity_.y;

	// 
	riseTimer_ += 1.0f / 60.0f;
	if (riseTimer_ >= riseDuration_) {
		isRising_ = false; // 
		velocity_.y = 0.0f; // 
	}


}

void SkyTypeEnemy::Attack() {
}

void SkyTypeEnemy::OnCollisionEnter(BaseObject* other) {
	if (dynamic_cast<PlayerMissile*>(other)) {
		--hp_;
		HitReaction();
	}
	if (dynamic_cast<PlayerMachineGun*>(other)) {
		--hp_;
		HitReaction();
	}
}

void SkyTypeEnemy::OnCollisionStay(BaseObject* other) {
}

void SkyTypeEnemy::OnCollisionExit(BaseObject* other) {
}

void SkyTypeEnemy::HitReaction()
{
	isBlinking_ = true;
	blinkTimer_ = 0.0f;

	// 点滅開始時に白色にする（仮に SetColor 関数があると仮定）
	model_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }); // 真っ白（光った感じ）
}

void SkyTypeEnemy::UpdateWanderState() {
	BaseEnemy::RandomMove();
}

void SkyTypeEnemy::UpdateChaseState() {
	BaseEnemy::MoveToTarget();
}

void SkyTypeEnemy::UpdateCombatState() {
	// 
	if (isRising_) {
		MoveToRise();
		return;
	}

	// 
	if (isRushing_) {
		MoveToRush();
		return;
	}

	// 
	Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
	float distance = Length(toTarget);

	// 
	if (distance < combatDistance_ * 1.5f && !isRushing_ && !isRising_) {
		isRushing_ = true;
		rushTimer_ = 0.0f;
	} else {
		BaseEnemy::MoveToTarget(); // 
	}
}

void SkyTypeEnemy::UpdateActionState() {
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
