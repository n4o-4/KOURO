#pragma once
#include "BaseCharacter.h"
class Player : public BaseCharacter
{
public: // ���J�����o�֐�

	// ����������
	void Initialize(Model* model) override;

	// �X�V����
	void Update() override;

	// �`�揈��
	void Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) override;

	// �J������ݒ�
	void SetCamera(BaseCamera* camera) { camera_ = camera; }
};

