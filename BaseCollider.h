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

	enum class ColliderType
	{
		kNone = -1,               // �R���C�_�[�Ȃ�
		kSphere,                  // ��
		kAABB,                    // �����s���E�{�b�N�X
		//kOBB,
		//kCapsule,
		//kPlane,
		kCountOfColliderType,
	};

public:

	// ������
	void Initialize(WorldTransform* worldTransform, ColliderType type);

	

private: // �����o�ϐ�


	WorldTransform* worldTransform_ = nullptr; // ���[���h�g�����X�t�H�[��

	ColliderType type_ = ColliderType::kNone; // �R���C�_�[�̎��
};

