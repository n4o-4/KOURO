#include "GroundTypeEnemy2.h"
#include <PlayerMissile.h>
#include <PlayerMachineGun.h>

void GroundTypeEnemy2::Initialize() {
	ModelManager::GetInstance()->LoadModel("enemy/bomb/bomb.obj");
	BaseEnemy::Initialize(ModelManager::GetInstance()->FindModel("enemy/bomb/bomb.obj"));
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


	// ジャンプ処理
	if (worldTransform_->transform.translate.y > 0.0f || jumpVelocity_ > 0.0f) {
		jumpVelocity_ -= 0.01f; // 重力
		worldTransform_->transform.translate.y += jumpVelocity_;

		// 地面に戻ったら停止
		if (worldTransform_->transform.translate.y < 0.0f) {
			worldTransform_->transform.translate.y = 0.0f;
			jumpVelocity_ = 0.0f;
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
	}
	if (dynamic_cast<PlayerMachineGun*>(other)) {
		--hp_;
		HitJump();
	}
}

void GroundTypeEnemy2::OnCollisionStay(BaseObject* other) {
}

void GroundTypeEnemy2::OnCollisionExit(BaseObject* other) {
}

void GroundTypeEnemy2::HitJump()
{
	// 簡単なジャンプ処理：ジャンプ速度を加算
	if (worldTransform_->transform.translate.y <= 0.1f) { // 地面にいる時だけジャンプ
		jumpVelocity_ = 0.3f; // ジャンプ力（調整可）
	}
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

		// 位置を更新
		worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;

		// 敵の向きを進行方向に合わせる
		float targetRotationY = std::atan2(direction.x, direction.z);
		worldTransform_->transform.rotate.y = targetRotationY;

		SetModelColor(Vector4{ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

void GroundTypeEnemy2::UpdateCombatState() {
	if (target_) {
		// プレイヤーへの方向ベクトルを計算
		Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
		Vector3 direction = Normalize(toTarget);

		// 通常速度の2倍で突進
		float dashSpeed = speed_ * 3.0f;
		velocity_ = { direction.x * dashSpeed,0.0f,direction.z * dashSpeed };

		// 位置を更新
		worldTransform_->transform.translate += velocity_;

		// 向きを進行方向に合わせる
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
