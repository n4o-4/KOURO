#pragma once

#include "Kouro.h"

class BaseBullet
{
public: // ���J�����o�֐�
	
	/**
	 * \brief  Initialize ������
	 * \param  model ���f��
	 */

	virtual void Initialize(Model* model, Vector3 spawnPos);

	// \brief  Update �X�V

	virtual void Update();
	
	
	/**
	 * \brief  Draw �`��
	 * \param  directionalLight �f�B���N�V���i�����C�g
	 * \param  pointLight �|�C���g���C�g
	 * \param  spotLight �X�|�b�g���C�g
	 */

	virtual void Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

	/**
	 * \brief  GetIsAlive
	 * \return	isAlive �����t���O
	 */

	bool GetIsAlive() const { return isActive_; }

protected: // �h���p�����o�ϐ�

	// 3D���f��
	std::unique_ptr<Object3d> object3d_;

	// ���[���h�g�����X�t�H�[��
	std::unique_ptr<WorldTransform> worldTransform_;

	// ���x
	Vector3 velocity_ = { 0.0f,0.0f,0.0f };

	// �L���t���O
	bool isActive_ = true;

	BaseCamera* camera_ = nullptr;

	// �����萔
	const float kLifeTime_ = 5.0f; //!< �e�̎���

	float lifeTimer_ = 0.0f; //!< �e�̎����^�C�}�[
};

