#pragma once
// Engine
#include "Kouro.h"

class BaseCollider
{
public:

	struct Sphere
	{
		Vector3 center; // ���S
		float radius; // ���a
	};

	struct AABB
	{
		Vector3 min; // �ŏ����W
		Vector3 max; // �ő���W
	};

public:

	// ������
	virtual void Initialize(WorldTransform* worldTransform);

private: // �����o�ϐ�

	WorldTransform* worldTransform_ = nullptr; // ���[���h�g�����X�t�H�[��

protected:

	uint32_t collisionAttribute_ = 0; // �R���W��������

	uint32_t collisionMask_ = 0; // �R���W�����}�X�N
};

