#include "BaseCollider.h"

void BaseCollider::Initialize(WorldTransform* worldTransform)
{
	// �R���C�_�[�Ɋ֘A�t���郏�[���h�ϊ�����ݒ肷��

	worldTransform_ = worldTransform;
}

void BaseCollider::PrevCollisionsClear()
{
	// �Փ˂��Ă����R���C�_�[�̃Z�b�g���N���A

	prevCollisions_.clear();
}

void BaseCollider::AddPrevCollision(BaseCollider* collider)
{
	// �O��̏Փˏ��ɃR���C�_�[��ǉ�

	prevCollisions_.insert(collider);
}
