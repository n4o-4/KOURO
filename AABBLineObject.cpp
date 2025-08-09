#include "AABBLineObject.h"

static const uint32_t kMaxVertexNum = 24; // AABB��8���_ * 3�� = 24���C��

void AABBLineObject::Initialize(DirectXCommon* dxCommon)
{
	///	����������

	// DirectXCommon�̃C���X�^���X�������o�ϐ��ɋL�^
	dxCommon_ = dxCommon;

	// vertexResource_�𐶐�
	CreateVertexResource();

	// vertexBufferView_�𐶐�
	CreateVertexBufferView();
}

void AABBLineObject::Update()
{
	/// �X�V����

	if (!aabbCollider_) {
		return; // AABBCollider���ݒ肳��Ă��Ȃ��ꍇ�͉������Ȃ�
	}

    const AABB& aabb = aabbCollider_->GetAABB();
    const Vector3& center = aabb.center;

    // min/max �� center ���������Έʒu�ɕ␳
    Vector3 min = aabb.min - center;
    Vector3 max = aabb.max - center;

    // AABB��8���_�icenter���_�Ƃ���j
    Vector3 vertices[8] = {
        { min.x, min.y, min.z }, // v0
        { max.x, min.y, min.z }, // v1
        { max.x, min.y, max.z }, // v2
        { min.x, min.y, max.z }, // v3
        { min.x, max.y, min.z }, // v4
        { max.x, max.y, min.z }, // v5
        { max.x, max.y, max.z }, // v6
        { min.x, max.y, max.z }  // v7
    };

    // AABB�̕ӂ��\������C���f�b�N�X�i24�̃��C���j
    constexpr int lineIndices[24] = {
        0, 1, 1, 2, 2, 3, 3, 0, // ����
        4, 5, 5, 6, 6, 7, 7, 4, // ���
        0, 4, 1, 5, 2, 6, 3, 7  // ����
    };

    for (int i = 0; i < 24; ++i) {
        const Vector3& v = vertices[lineIndices[i]];
        vertexData_[i].position = { v.x, v.y, v.z, 1.0f };
		vertexData_[i].color = aabbCollider_->GetColor(); // AABBCollider�̐F���g�p
    }
}

void AABBLineObject::CreateVertexResource()
{
    // ���_�f�[�^�̊m��
    vertexResource_ = dxCommon_->CreateBufferResource(sizeof(LineDrawer::VertexData) * kMaxVertexNum);

	// ���_�f�[�^�̃}�b�s���O
    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void AABBLineObject::CreateVertexBufferView()
{
    // VertexBufferView�̐���
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

    vertexBufferView_.SizeInBytes = UINT(sizeof(LineDrawer::VertexData) * kMaxVertexNum);

    vertexBufferView_.StrideInBytes = sizeof(LineDrawer::VertexData);
}
