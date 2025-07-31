#include "OBBLineObject.h"

static const uint32_t kMaxVertexNum = 24; // AABB��8���_ * 3�� = 24���C��

void OBBLineObject::Initialize(DirectXCommon* dxCommon)
{
	///	����������

	// DirectXCommon�̃C���X�^���X�������o�ϐ��ɋL�^
	dxCommon_ = dxCommon;

	// vertexResource_�𐶐�
	CreateVertexResource();

	// vertexBufferView_�𐶐�
	CreateVertexBufferView();
}

void OBBLineObject::Update()
{
    if (!obbCollider_) {
        return; // OBB���ݒ肳��Ă��Ȃ��ꍇ�͉������Ȃ�
    }

    const OBB& obb = obbCollider_->GetOBB();
    const Vector3& center = obb.center;

    // ��]��
    const Vector3& xAxis = obb.axes[0]; // X�������x�N�g��
    const Vector3& yAxis = obb.axes[1]; // Y�������x�N�g��
    const Vector3& zAxis = obb.axes[2]; // Z�������x�N�g��

    // ���[�J����Ԃł�8���_
    Vector3 localVertices[8] = {
        { +obb.halfSize.x, +obb.halfSize.y, +obb.halfSize.z }, // v0
        { +obb.halfSize.x, +obb.halfSize.y, -obb.halfSize.z }, // v1
        { +obb.halfSize.x, -obb.halfSize.y, +obb.halfSize.z }, // v2
        { +obb.halfSize.x, -obb.halfSize.y, -obb.halfSize.z }, // v3
        { -obb.halfSize.x, +obb.halfSize.y, +obb.halfSize.z }, // v4
        { -obb.halfSize.x, +obb.halfSize.y, -obb.halfSize.z }, // v5
        { -obb.halfSize.x, -obb.halfSize.y, +obb.halfSize.z }, // v6
        { -obb.halfSize.x, -obb.halfSize.y, -obb.halfSize.z }  // v7
    };

    // ���C���`��p�C���f�b�N�X
    constexpr int lineIndices[24] = {
        0, 1, 1, 3, 3, 2, 2, 0, // ���
        4, 5, 5, 7, 7, 6, 6, 4, // ���
        0, 4, 1, 5, 2, 6, 3, 7  // ����
    };

    // ���_�ϊ��Ɗi�[
    for (int i = 0; i < 24; ++i) {
        const Vector3& local = localVertices[lineIndices[i]];

        // ���[�J�����W�����[���h��Ԃɕϊ��i�X���K�p�j
        Vector3 world =
            xAxis * local.x +
            yAxis * local.y +
            zAxis * local.z +
            center;

        vertexData_[i].position = { world.x, world.y, world.z, 1.0f };
        vertexData_[i].color = obbCollider_->GetColor(); // OBB�̐F���g�p
    }
}

void OBBLineObject::Draw(const ViewProjection& viewProjection)
{
}

void OBBLineObject::CreateVertexResource()
{
    // ���_�f�[�^�̊m��
    vertexResource_ = dxCommon_->CreateBufferResource(sizeof(LineDrawer::VertexData) * kMaxVertexNum);

    // ���_�f�[�^�̃}�b�s���O
    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
}

void OBBLineObject::CreateVertexBufferView()
{
    // VertexBufferView�̐���
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();

    vertexBufferView_.SizeInBytes = UINT(sizeof(LineDrawer::VertexData) * kMaxVertexNum);

    vertexBufferView_.StrideInBytes = sizeof(LineDrawer::VertexData);
}
