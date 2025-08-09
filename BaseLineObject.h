#pragma once

#include "DirectXCommon.h"
#include "SrvManager.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

namespace LineDrawer
{
	struct VertexData // ���C���`��p��VertexData
	{
		Vector4 position; // ���_�ʒu
		Vector4 color; // ���_�J���[
	};
}

class BaseLineObject // ���C���I�u�W�F�N�g�̊��N���X
{
public: // ���J�����o�֐�

	virtual ~BaseLineObject() = default;

	/**
	 * \brief  Initialize ������
	 * \param  dxCommon DirectXCommon�̃C���X�^���X
	 */

	virtual void Initialize(DirectXCommon* dxCommon);

	/**
	 * \brief  Update �X�V
	 */

	virtual void Update();

	/**
	 * \brief  Draw �`��
	 * \param  viewProjection �r���[�v���W�F�N�V�����s��
	 */

	virtual void Draw(const ViewProjection& viewProjection);

protected:

	DirectXCommon* dxCommon_ = nullptr;

	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource_ = nullptr;

	LineDrawer::VertexData* vertexData_ = nullptr;

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_;
};

