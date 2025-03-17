#include "SkyTypeEnemy.h"
#include "PlayerMissile.h"
void SkyTypeEnemy::Initialize() {
	ModelManager::GetInstance()->LoadModel("enemy/bat/bat.obj");
	BaseEnemy::Initialize(ModelManager::GetInstance()->FindModel("enemy/bat/bat.obj"));


}

void SkyTypeEnemy::Update() {
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
    }
}

void SkyTypeEnemy::OnCollisionStay(BaseObject* other) {
}

void SkyTypeEnemy::OnCollisionExit(BaseObject* other) {
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
