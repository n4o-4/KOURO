#pragma once
#include "BaseCamera.h"
#include "WorldTransform.h"

class FollowCamera :    public BaseCamera
{
public:

	// ������
	void Initialize();

	// �X�V
	void Update();

	// �^�[�Q�b�g�̐ݒ�
	void SetTarget(WorldTransform* target) { target_ = target; }

	// �^�[�Q�b�g�̐ݒ�
	void SetOffset(const Vector3& offset) { offset_ = offset; }

private: // �����o�ϐ�e
	
	WorldTransform* target_ = nullptr;

	Vector3 offset_ = {};

};

