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

	// \brief  Update �X�V

	void Update() override;
	
	///================================================================================
	///                                        setter

	/**
	 * \brief AABB�̏���ݒ肷��
	 * \parma AABB AABB�̍ŏ����W�ƍő���W
	*/

	void SetAABB(const AABB& aabb) { aabb_ = aabb; }

	///================================================================================
	///                                        getter
	
	/**================================================================================
	 * \brief  GetAABB AABB���擾����
	 * \return AABB
	 */

	AABB GetAABB() const { return aabb_; }

private: /// ����J�����o�֐�

public: /// ���J�����o�ϐ�

private: /// ����J�����o�ϐ�
	
	// AABB�̍ŏ����W�ƍő���W

	AABB aabb_ = {};

};

