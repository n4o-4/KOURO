#pragma once
#include "BaseCollider.h"

class AABBCollider : public BaseCollider
{
public: /// ���J�����o�֐�

	/**
	 * \brief  Initialize ������
	 * \param  worldTransform ���[���h�ό`���
	 */

	void Initialize(WorldTransform* worldTransform) override;
	

	/**
	 * \brief  GetAABB AABB���擾����
	 * \return AABB
	 */

	AABB GetAABB() const { return aabb_; }

	/**
	 * \brief AABB�̏���ݒ肷��
	 * \parma AABB AABB�̍ŏ����W�ƍő���W
	*/

	void SetAABB(const AABB& aabb) { aabb_ = aabb; }

	/**
	 * \brief  SetCollisionAttribute �R���W����������ݒ肷��
	 * \parma  CollisionAttribute �R���W��������
	 */

	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	

	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

private: /// ����J�����o�֐�


public: /// ���J�����o�ϐ�

private: /// ����J�����o�ϐ�
	
	// AABB�̍ŏ����W�ƍő���W

	AABB aabb_ = {};

};

