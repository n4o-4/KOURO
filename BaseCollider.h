#pragma once
#include <unordered_set>

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

public: /// ���J�����o�֐�

	/**
	 * \brief  Initialize ������
	 * \param worldTransform ���[���h�ό`���
	 */

	virtual void Initialize(WorldTransform* worldTransform);
	
	/**
	 * \brief  Update �X�V
	 */

	virtual void Update() = 0;

	///================================================================================
	///                                        setter

	/**================================================================================
	 * \brief  SetCollisionAttribute �R���W����������ݒ肷��
	 * \return collisionAttribute(�R���W��������)
	 */

	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	/**================================================================================
	 * \brief  SetCollisionMask �R���W�����}�X�N��ݒ肷��
	 * \parma  CollisionMask �R���W�����}�X�N
	 */

	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

	///================================================================================
	///                                        getter

	/**================================================================================
	 * \brief  GetCollisionAttribute �R���W�����������擾����
	 * \return collisionAttribute(�R���W��������)
	 */

	uint32_t GetCollisionAttribute() const { return collisionAttribute_; }

	/**================================================================================
	 * \brief  GetCollisionMask �R���W�����}�X�N���擾����
	 * \return collisionMask(�R���W�����}�X�N)
	 */

	uint32_t GetCollisionMask() const { return collisionMask_; }	


	
	// \brief  OnCollisionEnter �ՓˊJ�n���̏���

	virtual void OnCollisionEnter(BaseCollider* other) = 0;

	// \brief  OnCollisionStay �Փ˒��̏���

	virtual void OnCollisionStay(BaseCollider* other) = 0;

	// \brief  OnCollisionExit �ՓˏI�����̏���

	virtual void OnCollisionExit(BaseCollider* other) = 0;

protected:

	WorldTransform* worldTransform_ = nullptr; // ���[���h�g�����X�t�H�[��

	uint32_t collisionAttribute_ = 0; // �R���W��������

	uint32_t collisionMask_ = 0; // �R���W�����}�X�N

	std::unordered_set<BaseCollider*> prevCollisions_; // �Փ˂��Ă����R���C�_�[�̃Z�b�g
};

