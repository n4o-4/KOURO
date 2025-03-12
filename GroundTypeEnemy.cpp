#include "GroundTypeEnemy.h"
#include "PlayerBullet.h"
#include <cmath>
#include <algorithm>

void GroundTypeEnemy::Initialize() {
	BaseEnemy::Initialize();
}

void GroundTypeEnemy::Update() {
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

   
    /*intervalCounter_ += 1.0f / 60.0f;
    if (kIntervalTiem <= intervalCounter_ && currentState_ == ActionState::Combat) {
        Jump();
        intervalCounter_ = fmod(intervalCounter_, kIntervalTiem);
    }*/

    BaseEnemy::Update();
}

void GroundTypeEnemy::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
    BaseEnemy::Draw(viewProjection, directionalLight, pointLight, spotLight);
}

void GroundTypeEnemy::MoveToJump() {
    if (target_) {
    
        Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
        toTarget.y = 0.0f; 
        float distance = Length(toTarget);
        Vector3 direction = Normalize(toTarget);

        
        if (!isJumping_) {
            isJumping_ = true;
            jumpVelocity_ = 5.0f;  
            jumpTime_ = 0.0f;      
        }

       
        jumpTime_ += 1.0f / 60.0f;
        jumpVelocity_ -= gravity_ * jumpTime_;

       
        Vector3 moveDirection = direction * speed_;
        moveDirection.y = jumpVelocity_;

       
        worldTransform_->transform.translate = worldTransform_->transform.translate + moveDirection;

       
        if (worldTransform_->transform.translate.y <= groundHeight_) {
            worldTransform_->transform.translate.y = groundHeight_;
            isJumping_ = false;
        }
    
    
    }

}

void GroundTypeEnemy::OnCollisionEnter(BaseObject* other) {
    if (dynamic_cast<PlayerBullet*>(other)) {
        --hp_;
    }
}

void GroundTypeEnemy::OnCollisionStay(BaseObject* other) {
}

void GroundTypeEnemy::OnCollisionExit(BaseObject* other) {
}

void GroundTypeEnemy::UpdateWanderState() {
    BaseEnemy::RandomMove();
}

void GroundTypeEnemy::UpdateChaseState() {
    MoveToJump();
}

void GroundTypeEnemy::UpdateCombatState() {
    if (target_) {
        // �^�[�Q�b�g�Ɍ������x�N�g�����v�Z
        Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
        float distance = Length(toTarget);
        Vector3 direction = Normalize(toTarget);

        // �X�g���b�t�B���O�i���ړ��j�̃x�N�g�����v�Z
        Vector3 strafeDir = { direction.z, 0.0f, -direction.x }; // 90�x��]

        // ���ԂƂƂ��ɕ�����ς��邽�߂̌W���i-1����1�͈̔́j
        float strafeFactor = sinf(stateTimer_ * 0.5f);

        // �����Ɋ�Â��čs��������
        Vector3 moveDirection;

        if (distance < safeDistance_) {
            // �߂�����ꍇ�A���
            //moveDirection = -direction * 0.7f + strafeDir * strafeFactor * 0.3f;
            moveDirection.x = -direction.x * 0.7f + strafeDir.x * strafeFactor * 0.3f;
            moveDirection.y = 0.0f;
            moveDirection.z = -direction.z * 0.7f + strafeDir.z * strafeFactor * 0.3f;
        } else if (distance > combatDistance_) {
            // ��������ꍇ�A�ڋ�
            moveDirection = direction * 0.7f + strafeDir * strafeFactor * 0.3f;
        } else {
            // �K�؂ȋ����̏ꍇ�A�X�g���b�t�B���O�̂�
            moveDirection = strafeDir * strafeFactor;
        }

        // ���x���X�V
        velocity_ = Normalize(moveDirection) * speed_ * 0.8f;

        // �ʒu���X�V
        worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;

        // ��Ƀ^�[�Q�b�g�̕�������
        float targetRotationY = std::atan2(direction.x, direction.z);
        worldTransform_->transform.rotate.y = targetRotationY;
    }
}

void GroundTypeEnemy::UpdateActionState() {
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
