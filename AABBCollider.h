#pragma once
#include "BaseCollider.h"

class AABBCollider : public BaseCollider
{
public: /// ���J�����o�֐�

	// ������

	void Initialize(WorldTransform* worldTransform) override;
	

	/**
    * @brief AABB�̏����擾����
    *
    * @return AABB�̏��
    */

	AABB GetAABB() const { return aabb_; }

	/**
	 * @brief AABB�̏���ݒ肷��
	*/

	void SetAABB(const AABB& aabb) { aabb_ = aabb; }

	// collisionAttribute��ݒ�

	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	// collisionMask��ݒ�

	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

private: /// ����J�����o�֐�


public: /// ���J�����o�ϐ�

private: /// ����J�����o�ϐ�
	
	// AABB�̍ŏ����W�ƍő���W

	AABB aabb_ = {};

};

