#pragma once
#include "BaseScene.h"

#include "Kouro.h"

class OkaScene : public BaseScene
{
public: // �����o�֐�

	// ������
	void Initialize() override;

	// �I��
	void Finalize() override;

	// ���t���[���X�V
	void Update() override;

	// �`��
	void Draw() override;


};

