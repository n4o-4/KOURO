#pragma once
#include "Kouro.h"
#include "BaseObject.h"
#include <iostream>
#include <list>
#include <memory>
#include <random>
#include "PostEffect.h"
#include "EnemyBullet.h"

#include "BaseEnemy.h"
class GroundTypeEnemy4 :public BaseEnemy {
    // �s�����
    enum class ActionState {
        Wander,   // �p�j�i�X�|�[���t�߂œ������j
        Chase,    // �ǐՁi�v���C���[�ɋ߂Â��j
        Combat    // �퓬�i�X�g���b�t�B���O���Ȃ���U���j
    };

    ///--------------------------------------------------------------
    ///						�����o�֐� 
public:


    GroundTypeEnemy4() : BaseEnemy() {}

    // @brief ������
    void Initialize();
    // @brief �X�V
    void Update();
    /**----------------------------------------------------------------------------
      * \brief  Draw �`��
      * \param  viewProjection �r���[�ˉe�s��
      * \param  directionalLight �f�B���N�V���i�����C�g
      * \param  pointLight �|�C���g���C�g
      * \param  spotLight �X�|�b�g���C�g
      */
    void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

    ///--------------------------------------------------------------
    ///						
private:
    /// \brief �ՓˊJ�n���̏���
    void OnCollisionEnter(BaseObject* other) override;

    /// \brief �Փˌp�����̏���
    void OnCollisionStay(BaseObject* other) override;

    /// \brief �ՓˏI�����̏���
    void OnCollisionExit(BaseObject* other) override;

    ///--------------------------------------------------------------
    ///						 
public:

    //�W�����v
    void HitJump();

    /**----------------------------------------------------------------------------
      * \brief  SetPosition �ʒu��ݒ�
      * \param  position �ʒu
      */
    void SetPosition(const Vector3& position) {
        worldTransform_->transform.translate = position;
        spawnPosition_ = position; // ?X?|?[????u????
    }

    /**----------------------------------------------------------------------------
      * \brief  GetHp HP���擾
      * \return HP
      */
    void SetHp(const int hp) { hp_ = hp; }

    /**----------------------------------------------------------------------------
     * \brief  GetPosition
     * \return
     */
    const Vector3& GetPosition() const {
        return worldTransform_->transform.translate;
    }

    /**----------------------------------------------------------------------------
       * \brief  GetPosition �ʒu���擾
       * \return �ʒu
       */
    const int GetHp() const { return hp_; }

    void SetTarget(WorldTransform* target) { target_ = target; }

    //�e�̎擾
    std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

private:
    // �s����ԕʂ̍X�V���\�b�h
    void UpdateWanderState();
    void UpdateChaseState();
    void UpdateCombatState();

    // ��Ԃ�I�����郁�\�b�h
    void UpdateActionState();


    ///--------------------------------------------------------------
    /// �����o�ϐ�
private:


    // �s�����
    ActionState currentState_ = ActionState::Wander;

    // �N���X���� private: �̉��ɒǉ�
    bool isHitReacting_ = false;
    float hitReactionTimer_ = 0.0f;
    Vector3 startScale_;
    Vector3 targetScale_;
};

