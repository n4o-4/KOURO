#include "BaseEnemy.h"
#include "PlayerMissile.h"
#include <cmath>
#include <algorithm>

///=============================================================================
///						������
void BaseEnemy::Initialize() {
    //========================================
    // ���f���������
    model_ = std::make_unique<Object3d>();
    model_->Initialize(Object3dCommon::GetInstance());
    //========================================
    // ���f����ǂݍ���
    ModelManager::GetInstance()->LoadModel("enemy/enemy.obj");
    model_->SetModel("enemy/enemy.obj");
    //========================================
    // �����ʒu��ݒ�
    worldTransform_ = std::make_unique<WorldTransform>();
    worldTransform_->Initialize();
    //========================================
    // �����蔻��Ƃ̓���
    BaseObject::Initialize(worldTransform_->transform.translate, 1.0f);

    // ����������̏�����
    std::random_device rd;
    rng_ = std::mt19937(rd());

    // �X�|�[���ʒu�̏�����
    spawnPosition_ = worldTransform_->transform.translate;
}

///=============================================================================
///						�`��
void BaseEnemy::Update() {
    if (hp_ > 0) {
       
        // �e�̍X�V
        BulletUpdate();

        // ���[���h�ϊ��̍X�V
        worldTransform_->UpdateMatrix();
        // ���f���̃��[�J���s���X�V
        model_->SetLocalMatrix(MakeIdentity4x4());
        // ���f���̍X�V
        model_->Update();

        //========================================
        // �����蔻��Ƃ̓���
        BaseObject::Update(worldTransform_->transform.translate);
    }
}

///=============================================================================
///						�`��
void BaseEnemy::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
    BulletDraw(viewProjection, directionalLight, pointLight, spotLight);

    //========================================
    // ���f���̕`��
    if (hp_ > 0) {
        model_->Draw(*worldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
    }
}

///=============================================================================
///						�����蔻��
///--------------------------------------------------------------
///						�ڐG�J�n����
void BaseEnemy::OnCollisionEnter(BaseObject* other) {
}

///--------------------------------------------------------------
///						�ڐG�p������
void BaseEnemy::OnCollisionStay(BaseObject* other) {
}

///--------------------------------------------------------------
///						�ڐG�I������
void BaseEnemy::OnCollisionExit(BaseObject* other) {
}

void BaseEnemy::BulletUpdate() {
}

void BaseEnemy::BulletDraw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
    for (auto it = bullets_.begin(); it != bullets_.end(); ) {
        it->get()->Draw(viewProjection, directionalLight, pointLight, spotLight);
        ++it;
    }
}

void BaseEnemy::Fire() {
    if (target_) {
        std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
        newBullet->Initialize(*worldTransform_.get(), target_->transform.translate);
        bullets_.push_back(std::move(newBullet));
    }
}

void BaseEnemy::MoveToTarget()
{
    if (target_) {
        // �^�[�Q�b�g�Ɍ������x�N�g����v�Z
        Vector3 toTarget = target_->transform.translate - worldTransform_->transform.translate;
        float distance = Length(toTarget);

        Vector3 direction = Normalize(toTarget);
        velocity_ = direction * speed_;

        // �ʒu��X�V
        worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;

        // �G�̌�����i�s�����ɍ��킹��
        float targetRotationY = std::atan2(direction.x, direction.z);
        worldTransform_->transform.rotate.y = targetRotationY;
    }
}

void BaseEnemy::RandomMove()
{
    // �����ύX�^�C�}�[�̍X�V
    directionChangeTimer_ += 1.0f / 60.0f;

    // ����I�ɕ�����ύX
    if (directionChangeTimer_ >= directionChangeInterval_) {
        // �X�|�[���n�_�ɖ߂�����ƁA�����_���ȕ����������
        Vector3 toSpawn = spawnPosition_ - worldTransform_->transform.translate;
        float distanceToSpawn = Length(toSpawn);

        // �X�|�[���n�_���牓������ꍇ�̓X�|�[���n�_�ɖ߂�X������߂�
        float spawnWeight = std::min(distanceToSpawn / wanderRadius_, 0.8f);

        if (distanceToSpawn > wanderRadius_) {
            // �X�|�[���n�_�ɖ߂������D��
            velocity_ = Normalize(toSpawn) * speed_;
        }
        else {
            // �����_���ȕ�����I��
            float angle = angleDist_(rng_);
            Vector3 randomDir = { cosf(angle), 0.0f, sinf(angle) };
            velocity_ = Normalize(randomDir) * speed_;
        }

        directionChangeTimer_ = 0.0f;
    }

    // �ʒu��X�V
    worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;

    // �G�̌�����i�s�����ɍ��킹��
    if (Length(velocity_) > 0.01f) {
        float targetRotationY = std::atan2(velocity_.x, velocity_.z);
        worldTransform_->transform.rotate.y = targetRotationY;
    }
}
// �R���e�L�X�g�x�[�X�̕����I��
Vector3 BaseEnemy::SelectDirection() {
    // �����͏ȗ��i�V���v������D��j
    return velocity_;
}