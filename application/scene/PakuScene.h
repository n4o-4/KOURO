#pragma once
#include "BaseScene.h"

#include "Kouro.h"

class PakuScene : public BaseScene
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

