#include "GroundTypeEnemy2.h"

void GroundTypeEnemy2::Initialize() {
    ModelManager::GetInstance()->LoadModel("enemy/bomb/bomb.obj");
    BaseEnemy::Initialize(ModelManager::GetInstance()->FindModel("enemy/bomb/bomb.obj"));
}

void GroundTypeEnemy2::Update() {
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

void GroundTypeEnemy2::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
    BaseEnemy::Draw(viewProjection, directionalLight, pointLight, spotLight);
}



void GroundTypeEnemy2::Attack() {
}

void GroundTypeEnemy2::OnCollisionEnter(BaseObject* other) {
}

void GroundTypeEnemy2::OnCollisionStay(BaseObject* other) {
}

void GroundTypeEnemy2::OnCollisionExit(BaseObject* other) {
}

void GroundTypeEnemy2::UpdateWanderState() {
    BaseEnemy::RandomMove();
}

void GroundTypeEnemy2::UpdateChaseState() {
    if (target_) {
        // �^�[�Q�b�g�Ɍ������x�N�g�����v�Z
        Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
        float distance = Length(toTarget);

        Vector3 direction = Normalize(toTarget);
        velocity_ = { direction.x * speed_,0.0f,direction.z * speed_ };

        // �ʒu���X�V
        worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;

        // �G�̌�����i�s�����ɍ��킹��
        float targetRotationY = std::atan2(direction.x, direction.z);
        worldTransform_->transform.rotate.y = targetRotationY;

        SetModelColor(Vector4{ 1.0f, 1.0f, 1.0f, 1.0f });
    }
}

void GroundTypeEnemy2::UpdateCombatState() {
    if (target_) {
        // �v���C���[�ւ̕����x�N�g�����v�Z
        Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
        Vector3 direction = Normalize(toTarget);

        // �ʏ푬�x��2�{�œːi
        float dashSpeed = speed_ * 3.0f;
        velocity_ = { direction.x * dashSpeed,0.0f,direction.z * dashSpeed };

        // �ʒu���X�V
        worldTransform_->transform.translate += velocity_;

        // ������i�s�����ɍ��킹��
        float targetRotationY = std::atan2(direction.x, direction.z);
        worldTransform_->transform.rotate.y = targetRotationY;

        SetModelColor(Vector4{ 1.0f, 0.0f, 0.0f, 1.0f });
    }
}

void GroundTypeEnemy2::UpdateActionState() {
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
