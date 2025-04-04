#include "GroundTypeEnemy3.h"
#include "PlayerMissile.h"
#include <cmath>
#include <algorithm>
#include "PlayerMachineGun.h"
#include "Player.h"

void GroundTypeEnemy3::Initialize() {
	ModelManager::GetInstance()->LoadModel("enemy/chair/chair.obj");
	BaseEnemy::Initialize(ModelManager::GetInstance()->FindModel("enemy/chair/chair.obj"));

	startScale_.x = startScale_.y = startScale_.z = 1.0f;
	targetScale_.x = targetScale_.y = targetScale_.z = 1.0f;

	particleEmitter_->SetParticleCount(20);
	particleEmitter_->SetLifeTimeRange(ParticleManager::LifeTimeRange({ 1.0f,1.0f }));
	particleEmitter_->SetVelocityRange(ParticleManager::VelocityRange({ -3.0f,3.0f }, { -3.0f,3.0f }, { -3.0f,3.0f }));

}

void GroundTypeEnemy3::Update() {
	if (hp_ > 0) {
		// ��Ԃ̍X�V
		UpdateActionState();

		// ���݂̏�Ԃɉ������s��
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

		// �C�[�W���O�i�o�E���h���Ft=0��1.0�At=0.5��1.7�At=1.0��1.0�j
		float scaleFactor = 1.0f + 0.7f * sinf(t * 3.141592f); // �΂�1���� �� 1��1.7��1

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

void GroundTypeEnemy3::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	BaseEnemy::Draw(viewProjection, directionalLight, pointLight, spotLight);
}

void GroundTypeEnemy3::PushTargetAway() {
	if (!target_) return;
	// ���ݒn��ۑ�
	Vector3 enemyPos = worldTransform_->transform.translate;
	Vector3 targetPos = target_->transform.translate;

	// �G���^�[�Q�b�g����
	Vector3 direction = targetPos - enemyPos;
	direction.y = 0.0f; // y���̉e������菜��

	// �����x�N�g���̐��K��
	direction = Normalize(direction);

	// �����o����
	const float pushStrength = 30.5f;

	// y���l�̕ۑ�
	float originalY = target_->transform.translate.y;

	// �����o��
	target_->transform.translate += direction * pushStrength;

	// y���̕���
	target_->transform.translate.y = originalY;
}



void GroundTypeEnemy3::OnCollisionEnter(BaseObject* other) {
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

void GroundTypeEnemy3::OnCollisionStay(BaseObject* other) {
}

void GroundTypeEnemy3::OnCollisionExit(BaseObject* other) {
}

void GroundTypeEnemy3::HitJump() {
	isHitReacting_ = true;
	hitReactionTimer_ = 0.0f;
	startScale_ = worldTransform_->transform.scale;
	targetScale_ = { 1.7f, 1.7f, 1.7f };   // �ꎞ�I�ɑ傫������
}


void GroundTypeEnemy3::UpdateWanderState() {
	BaseEnemy::RandomMove();
}

void GroundTypeEnemy3::UpdateChaseState() {
	if (target_) {
		// �^�[�Q�b�g�Ɍ������x�N�g�����v�Z
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

	// �^�[�Q�b�g�Ɍ������x�N�g�����v�Z
	Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;

	// y���̍����m�F�i�������قړ������`�F�b�N�j
	float yDiff = std::abs(toTarget.y);
	const float yTolerance = 3.0f; // �����̍���3�ȉ��̂Ƃ��̂݉������Ƃ��ł��܂�

	// ���������̃x�N�g���iXZ���ʂ݂̂ŋ����v�Z�j
	toTarget.y = 0.0f;
	float distance = Length(toTarget);

	// �ǐՈړ�
	Vector3 direction = Normalize(toTarget);
	velocity_ = direction * speed_ * 3.0f;

	// �ʒu�X�V�iy���ێ��j
	float originalY = worldTransform_->transform.translate.y;
	worldTransform_->transform.translate += velocity_;
	worldTransform_->transform.translate.y = originalY;

	// ��]�̍X�V
	float targetRotationY = std::atan2(direction.x, direction.z);
	worldTransform_->transform.rotate.y = targetRotationY;

	// ��苗�����ɂ���Ay�����قړ����Ƃ����������o��
	const float pushThreshold = 1.0f;
	if (distance < pushThreshold && yDiff < yTolerance) {
		PushTargetAway();
	}

	// �J���[�ύX(�퓬��ԗp)
	SetModelColor(Vector4{ 0.4f, 0.0f, 0.0f, 1.0f });
	
}

void GroundTypeEnemy3::UpdateActionState() {
	// �^�C�}�[�̍X�V
	stateTimer_ += 1.0f / 60.0f;

	// �^�[�Q�b�g�����݂��Ȃ��ꍇ�͜p�j
	if (!target_) {
		currentState_ = ActionState::Wander;
		return;
	}

	// �^�[�Q�b�g�Ƃ̋������v�Z
	Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
	float distance = Length(toTarget);

	// �����Ɋ�Â��ď�Ԃ�ύX 
	ActionState newState = currentState_; // �f�t�H���g�͌��݂̏�Ԃ��ێ�

	if (distance > chaseDistance_) {
		// �v���C���[�������ꍇ�͜p�j 
		newState = ActionState::Wander;
	} else if (distance > combatDistance_) {
		// �ǐՔ͈͓��Ȃ�ǐ�
		newState = ActionState::Chase;
	} else {
		// �퓬�͈͓��Ȃ�퓬
		newState = ActionState::Combat;
	}

	// ��Ԃ��ω������ꍇ�A�^�C�}�[�����Z�b�g
	if (newState != currentState_) {
		currentState_ = newState;
		stateTimer_ = 0.0f;
	}
}
