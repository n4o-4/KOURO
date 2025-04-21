#include "GroundTypeEnemy4.h"
#include <PlayerMissile.h>
#include <PlayerMachineGun.h>

void GroundTypeEnemy4::Initialize() {
	ModelManager::GetInstance()->LoadModel("enemy/WH/WH.obj");
	BaseEnemy::Initialize(ModelManager::GetInstance()->FindModel("enemy/WH/WH.obj"));

	startScale_.x = startScale_.y = startScale_.z = 1.0f;
	targetScale_.x = targetScale_.y = targetScale_.z = 1.0f;

	particleEmitter_->SetParticleCount(20);
	particleEmitter_->SetLifeTimeRange(ParticleManager::LifeTimeRange({ 1.0f,1.0f }));
	particleEmitter_->SetVelocityRange(ParticleManager::VelocityRange({ -3.0f,3.0f }, { -3.0f,3.0f }, { -3.0f,3.0f }));


}

void GroundTypeEnemy4::Update() {
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

void GroundTypeEnemy4::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	BaseEnemy::Draw(viewProjection, directionalLight, pointLight, spotLight);
	
}

void GroundTypeEnemy4::OnCollisionEnter(BaseObject* other) {
	if (dynamic_cast<PlayerMissile*>(other) || dynamic_cast<PlayerMachineGun*>(other)) {
		--hp_;
		HitJump();
		particleEmitter_->Emit();
	}
}

void GroundTypeEnemy4::OnCollisionStay(BaseObject* other) {
}

void GroundTypeEnemy4::OnCollisionExit(BaseObject* other) {
}

void GroundTypeEnemy4::HitJump() {
	isHitReacting_ = true;
	hitReactionTimer_ = 0.0f;
	startScale_ = worldTransform_->transform.scale;
	targetScale_ = { 1.7f, 1.7f, 1.7f };   // �ꎞ�I�ɑ傫������
}

void GroundTypeEnemy4::UpdateWanderState() {
	BaseEnemy::RandomMove();
}

void GroundTypeEnemy4::UpdateChaseState() {
	BaseEnemy::RandomMove();
}

void GroundTypeEnemy4::UpdateCombatState() {
	BaseEnemy::RandomMove();
}

void GroundTypeEnemy4::UpdateActionState() {
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
