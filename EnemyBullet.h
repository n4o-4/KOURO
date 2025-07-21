#pragma once
#include "BaseBullet.h"

class EnemyBullet : public BaseBullet, public SphereCollider
{
public: // ���J�����o�֐�
	/**
	 * \brief  Initialize ������
	 * \param  model ���f��
	 */

	void Initialize(Model* model, Vector3 spawnPos) override;

	// \brief  Update �X�V

	void Update() override;

	/**
	 * \brief  Draw �`��
	 * \param  directionalLight �f�B���N�V���i�����C�g
	 * \param  pointLight �|�C���g���C�g
	 * \param  spotLight �X�|�b�g���C�g
	 */

	void Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) override;

	///================================================================================
	///                                        setter

   /**
	* \brief  SetCamera �J������ݒ�
	* \param  camera �J����
   */

	void SetCamera(BaseCamera* camera) { camera_ = camera; }

	/**
	 * \brief  SetVelocity ���x��ݒ�
	 * \param  velocity ���x
	*/

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

private: // ����J�����o�֐�

	// \brief  OnCollisionEnter �ՓˊJ�n���̏���

	void OnCollisionEnter(BaseCollider* other) override;

	// \brief  OnCollisionStay �Փ˒��̏���

	void OnCollisionStay(BaseCollider* other) override;

	// \brief  OnCollisionExit �ՓˏI�����̏���

	void OnCollisionExit(BaseCollider* other) override;

private: // ����J�����o�ϐ�

	std::unique_ptr<ParticleEmitter> emitter_; //!< �p�[�e�B�N���G�~�b�^�[
};

