#pragma once
#include "BaseScene.h"
#include "Kouro.h"

class MiyaScene : public BaseScene
{
	// ������
	void Initialize() override;

	// �I��
	void Finalize() override;

	// ���t���[���X�V
	void Update() override;

	// �`��
	void Draw() override;
};

