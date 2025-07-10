#include "OBBCollider.h"

void OBBCollider::Initialize(WorldTransform* worldTransform)
{
	// �R���C�_�[�Ɋ֘A�t���郏�[���h�ϊ�����ݒ肷��
	colliderTransform_ = worldTransform;
	// OBB�̏�����
	obb_.center = { 0.0f, 0.0f, 0.0f };
	obb_.halfSize = { 1.0f, 1.0f, 1.0f };
	obb_.axes[0] = { 0.0f, 0.0f, 0.0f }; // X��
	obb_.axes[1] = { 0.0f, 0.0f, 0.0f }; // Y��
	obb_.axes[2] = { 0.0f, 0.0f, 0.0f }; // Z��
}

void OBBCollider::Update()
{
    // ���[���h�s�񂩂�X�P�[���E��]�E���s�ړ��𒊏o
    Matrix4x4 matWorld = colliderTransform_->matWorld_;

    // ���S�i���[���h���W�j�����[���h�s��̕��s�ړ�����
    obb_.center = {
        matWorld.m[3][0],
        matWorld.m[3][1],
        matWorld.m[3][2]
    };

    // ���[���h�s��̊e��x�N�g�����烍�[�J���������ƃX�P�[�����擾
    Vector3 xAxis = { matWorld.m[0][0], matWorld.m[0][1], matWorld.m[0][2] };
    Vector3 yAxis = { matWorld.m[1][0], matWorld.m[1][1], matWorld.m[1][2] };
    Vector3 zAxis = { matWorld.m[2][0], matWorld.m[2][1], matWorld.m[2][2] };

    // �e���̒����i�X�P�[���j���v�Z
    float scaleX = Length(xAxis);
    float scaleY = Length(yAxis);
    float scaleZ = Length(zAxis);

    // ���K�����Ď������x�N�g���ɂ���
    obb_.axes[0] = Normalize(xAxis);  // X��
    obb_.axes[1] = Normalize(yAxis);  // Y��
    obb_.axes[2] = Normalize(zAxis);  // Z��

    // �n�[�t�T�C�Y�i�X�P�[���̔����j
    obb_.halfSize = {
        scaleX * 0.5f,
        scaleY * 0.5f,
        scaleZ * 0.5f
    };
}
