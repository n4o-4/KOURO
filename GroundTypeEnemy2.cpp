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

void GroundTypeEnemy2::MoveToJump() {
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
    // �����ύX�^�C�}�[�̍X�V
    directionChangeTimer_ += 1.0f / 60.0f;

    // ����I�ɕ�����ύX
    if (directionChangeTimer_ >= directionChangeInterval_) {
        // �X�|�[���n�_�ɖ߂�����ƁA�����_���ȕ�����������
        Vector3 toSpawn = spawnPosition_ - worldTransform_->transform.translate;
        float distanceToSpawn = Length(toSpawn);

        // �X�|�[���n�_���牓������ꍇ�̓X�|�[���n�_�ɖ߂�X�������߂�
        float spawnWeight = std::min(distanceToSpawn / wanderRadius_, 0.8f);

        if (distanceToSpawn > wanderRadius_) {
            // �X�|�[���n�_�ɖ߂������D��
            velocity_ = Normalize(toSpawn) * speed_;
        } else {
            // �����_���ȕ�����I��
            float angle = angleDist_(rng_);
            Vector3 randomDir = { cosf(angle), 0.0f, sinf(angle) };
            velocity_ = Normalize(randomDir) * speed_;
        }

        directionChangeTimer_ = 0.0f;
    }
}

void GroundTypeEnemy2::UpdateChaseState() {
    BaseEnemy::MoveToTarget();
}

void GroundTypeEnemy2::UpdateCombatState() {
}

void GroundTypeEnemy2::UpdateActionState() {
}
