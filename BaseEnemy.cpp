#include "BaseEnemy.h"
#include "PlayerBullet.h"
#include <cmath>
#include <algorithm>

///=============================================================================
///						������
void BaseEnemy::Initialize() {
    //========================================
    // ���f����������
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
        // ���f���̃��[�J���s����X�V
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
// �R���e�L�X�g�x�[�X�̕����I��
Vector3 BaseEnemy::SelectDirection() {
    // �����͏ȗ��i�V���v������D��j
    return velocity_;
}