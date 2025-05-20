#include "GroundTypeEnemy3.h"
#include "PlayerMissile.h"
#include <cmath>
#include <algorithm>
#include "PlayerMachineGun.h"
#include "Player.h"

void GroundTypeEnemy3::Initialize() {
	ModelManager::GetInstance()->LoadModel("enemy/chair/chair.obj");
	BaseEnemy::Initialize(ModelManager::GetInstance()->FindModel("enemy/chair/chair.obj"));

	worldTransform_->transform.scale = (Vector3(modelScale_, modelScale_, modelScale_));

	particleEmitter_->SetParticleCount(20);
	particleEmitter_->SetLifeTimeRange(ParticleManager::Range({ 1.0f,1.0f }));
	particleEmitter_->SetVelocityRange(ParticleManager::Vec3Range({ -3.0f,-3.0f,-3.0f }, {3.0f,3.0f,3.0f}));

	AudioManager::GetInstance()->SoundLoadFile("Resources/se/爆発1.mp3");
    se1_ = std::make_unique<Audio>();
	se1_->Initialize();

}

void GroundTypeEnemy3::Update() {
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

		const float kTotalReactionTime = 0.3f; //
		float t = hitReactionTimer_ / kTotalReactionTime;

		// clamp
		if (t > 1.0f) t = 1.0f;

		// イージング（バウンド風：t=0で1.0、t=0.5で1.7、t=1.0で1.0）
		float scaleFactor = modelScale_ + 0.7f * sinf(t * 3.141592f); // πで1周期 → 1→1.7→1

		worldTransform_->transform.scale.x = scaleFactor;
		worldTransform_->transform.scale.y = scaleFactor;
		worldTransform_->transform.scale.z = scaleFactor;

		if (t >= 1.0f) {
			isHitReacting_ = false;
			hitReactionTimer_ = 0.0f;
			worldTransform_->transform.scale.x = modelScale_;
			worldTransform_->transform.scale.y = modelScale_;
			worldTransform_->transform.scale.z = modelScale_;
		}
	}
	BaseEnemy::Update();
}

void GroundTypeEnemy3::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	BaseEnemy::Draw(viewProjection, directionalLight, pointLight, spotLight);
}

void GroundTypeEnemy3::PushTargetAway() {
	if (!target_) return;
	// 現在地を保存
	Vector3 enemyPos = worldTransform_->transform.translate;
	Vector3 targetPos = target_->transform.translate;

	// 敵→ターゲット方向
	Vector3 direction = targetPos - enemyPos;
	direction.y = 0.0f; // y軸の影響を取り除く

	// 方向ベクトルの正規化
	direction = Normalize(direction);

	// 押し出す力
	const float pushStrength = 30.5f;

	// y軸値の保存
	float originalY = target_->transform.translate.y;

	// 押し出す
	target_->transform.translate += direction * pushStrength;

	// y軸の復元
	target_->transform.translate.y = originalY;
}



void GroundTypeEnemy3::OnCollisionEnter(BaseObject* other) {
	if (dynamic_cast<PlayerMissile*>(other)) {
		se1_->SoundPlay("Resources/se/爆発1.mp3", 0);
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

void GroundTypeEnemy3::OnCollisionStay(BaseObject* other) {
}

void GroundTypeEnemy3::OnCollisionExit(BaseObject* other) {
}

void GroundTypeEnemy3::HitJump() {
	isHitReacting_ = true;
	hitReactionTimer_ = 0.0f;
	modelScale_ = modelScale_ / 2.0f;
}


void GroundTypeEnemy3::UpdateWanderState() {
	BaseEnemy::RandomMove(modelScale_);
}

void GroundTypeEnemy3::UpdateChaseState() {
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

void GroundTypeEnemy3::UpdateCombatState() {
	if (!target_) return;

	// ターゲットに向かうベクトルを計算
	Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;

	// y軸の差を確認（高さがほぼ同じかチェック）
	float yDiff = std::abs(toTarget.y);
	const float yTolerance = 3.0f; // 高さの差が3以下のときのみ押すことができます

	// 水平方向のベクトル（XZ平面のみで距離計算）
	toTarget.y = 0.0f;
	float distance = Length(toTarget);

	// 追跡移動
	Vector3 direction = Normalize(toTarget);
	velocity_ = direction * speed_ * 3.0f;

	// 位置更新（y軸維持）
	float originalY = worldTransform_->transform.translate.y;
	worldTransform_->transform.translate += velocity_;
	worldTransform_->transform.translate.y = originalY;

	// 回転の更新
	float targetRotationY = std::atan2(direction.x, direction.z);
	worldTransform_->transform.rotate.y = targetRotationY;

	// 一定距離内にあり、y軸もほぼ同じときだけ押し出す
	const float pushThreshold = 1.0f;
	if (distance < pushThreshold && yDiff < yTolerance) {
		PushTargetAway();
	}

	// カラー変更(戦闘状態用)
	SetModelColor(Vector4{ 0.4f, 0.0f, 0.0f, 1.0f });
	
}

void GroundTypeEnemy3::UpdateActionState() {
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
