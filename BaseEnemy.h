#pragma once
#include "Kouro.h"
#include "BaseObject.h"
#include <iostream>
#include <list>
#include <memory>
#include <random>

#include "EnemyBullet.h"

class BaseEnemy : public BaseObject {
    ///--------------------------------------------------------------
    ///						 �����o�֐� 
public:
    // @brief ������
    virtual void Initialize();
    // @brief �X�V
    virtual void Update();
    /**----------------------------------------------------------------------------
     * \brief  Draw �`��
     * \param  viewProjection �r���[�ˉe�s��
     * \param  directionalLight �f�B���N�V���i�����C�g
     * \param  pointLight �|�C���g���C�g
     * \param  spotLight �X�|�b�g���C�g
     */
    void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

    ///--------------------------------------------------------------
    ///						 �����蔻��
private:
    /// \brief �ՓˊJ�n���̏���
    void OnCollisionEnter(BaseObject* other) override;

    /// \brief �Փˌp�����̏���
    void OnCollisionStay(BaseObject* other) override;

    /// \brief �ՓˏI�����̏���
    void OnCollisionExit(BaseObject* other) override;

    ///--------------------------------------------------------------
    ///						 ���o�͊֐�
public:
    /**----------------------------------------------------------------------------
     * \brief  SetPosition �ʒu��ݒ�
     * \param  position �ʒu
     */
    void SetPosition(const Vector3& position) {
        worldTransform_->transform.translate = position;
        spawnPosition_ = position; // �X�|�[���ʒu���ۑ�
    }

    /**----------------------------------------------------------------------------
     * \brief  SetHp HP��ݒ�
     * \param  hp HP
     */
    void SetHp(const int hp) { hp_ = hp; }

    /**----------------------------------------------------------------------------
     * \brief  GetPosition �ʒu���擾
     * \return �ʒu
     */
    const Vector3& GetPosition() const {
        return worldTransform_->transform.translate;
    }

    /**----------------------------------------------------------------------------
     * \brief  GetHp HP���擾
     * \return HP
     */
    const int GetHp() const { return hp_; }

    void SetTarget(WorldTransform* target) { target_ = target; }

    //�e�̎擾
    std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

private:
  
    // ������I�����郁�\�b�h
    Vector3 SelectDirection();

    void BulletUpdate();
    void BulletDraw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);
    void Fire();

    ///--------------------------------------------------------------
    /// �����o�ϐ�
private:
    //========================================
    // ���f��
    std::unique_ptr<Object3d> model_ = nullptr;
    //���[���h�ϊ�
    std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

    //HP
    int hp_ = 1;

    // �ړ��֘A
    float speed_ = 0.15f;   // �ړ����x
    float minX_ = -100.0f;  // ���̌��E
    float maxX_ = 100.0f;   // �E�̌��E
    int direction_ = 1;     // �ړ����� (1:�E, -1:��)
    Vector3 velocity_ = { 0.0f, 0.0f, 0.0f }; // ���݂̑��x�x�N�g��
    Vector3 spawnPosition_ = { 0.0f, 0.0f, 0.0f }; // �X�|�[���ʒu

    // �s�����
    ActionState currentState_ = ActionState::Wander;
    float stateTimer_ = 0.0f;
    float directionChangeTimer_ = 0.0f;

    // �s���p�����[�^
    float chaseDistance_ = 64.0f;    // ���̋������Ȃ�v���C���[��ǂ�������
    float combatDistance_ = 32.0f;   // �퓬���s������
    float safeDistance_ = 10.0f;     // ���S�����i���̋�����ۂj
    float wanderRadius_ = 32.0f;     // �p�j���a
    float directionChangeInterval_ = 3.0f; // �����ύX�̊Ԋu

    // ����������
    std::mt19937 rng_;
    std::uniform_real_distribution<float> angleDist_{ 0.0f, 2.0f * 3.14159f };
    std::uniform_real_distribution<float> strengthDist_{ 0.5f, 1.0f };

    WorldTransform* target_;
    std::list<std::unique_ptr<EnemyBullet>> bullets_;
    const float kIntervalTiem = 5.0f;
    float intervalCounter_ = 0.0f;
};