#pragma once
// Engine
#include "Kouro.h"


class BaseCharacter
{
public: // ���J�����o�֐�

	// ����������
	virtual void Initialize(Model* model);

	// �X�V����
	virtual void Update();

	// �`�揈��
	virtual void Draw(DirectionalLight directionalLight,PointLight pointLight,SpotLight spotLight);


protected: // �h���p�����o�֐�

	void DrawImGui();

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
};

