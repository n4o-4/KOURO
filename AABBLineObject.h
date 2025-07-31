#pragma once
#include "BaseLineObject.h"

#include "AABBCollider.h"

class AABBLineObject : public BaseLineObject
{
public: // ���J�����o�֐�

	~AABBLineObject() override = default;

	/**
	 * \brief  Initialize ������
	 */

	void Initialize(DirectXCommon* dxCommon)override;

	/**
	 * \brief  Update �X�V
	 */

	void Update()override;

	/**
	 * \brief  Draw �`��
	 * \param  viewProjection �r���[�v���W�F�N�V�����s��
	 */

	void Draw(const ViewProjection& viewProjection)override;

	/**
	 * \brief  SetAABBCollider AABBCollider��ݒ肷��
	 * \param  aabbCollider AABBCollider�ւ̃|�C���^
	 */

	void SetAABBCollider(AABBCollider* aabbCollider) { aabbCollider_ = aabbCollider; }

private: // ����J�����o�֐�

    /**
     * \brief  CreateVertexResource ���_���\�[�X�𐶐�����
     */

	void CreateVertexResource();

	/**
	 * \brief  CreateVertexBufferView ���_�o�b�t�@�r���[�𐶐�����
	 */

	void CreateVertexBufferView();

private: // ����J�����o�ϐ�

	AABBCollider* aabbCollider_ = nullptr; //!< AABBCollider�ւ̃|�C���^

};

