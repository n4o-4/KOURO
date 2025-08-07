#pragma once
#include "BaseCollider.h"

class OBBCollider : public BaseCollider
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
	 * \brief OBB�̏���ݒ肷��
	 * \parma OBB OBB�̍ŏ����W�ƍő���W
	*/

	void SetOBB(const OBB& obb) { obb_ = obb; }

	///================================================================================
	///                                        getter

	/**================================================================================
	 * \brief  GetOBB���擾����
	 * \return OBB
	 */

	OBB GetOBB() const { return obb_; }

	/**================================================================================
	 * \brief  color���擾����
	 * \return color
	 */

	Vector4 GetColor() const { return color_; }

private: /// ����J�����o�֐�

public: /// ���J�����o�ϐ�

private: /// ����J�����o�ϐ�

	// AABB�̍ŏ����W�ƍő���W

	OBB obb_ = {};
};

