#include "BaseCharacter.h"

void BaseCharacter::Initialize(Model* model)
{
	///========================================
	/// �I�u�W�F�N�g3D
	
	// �����Ə�����
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(Object3dCommon::GetInstance());

	// ���f����ݒ�
	object3d_->SetModel(model);

	// ���[�J���s��̏�����
	object3d_->SetLocalMatrix(MakeIdentity4x4());



	///========================================
	/// ���[���h�g�����X�t�H�[��
	
	// �����Ə�����
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	// �����l�̐ݒ�


	///========================================
	/// ���̑��ϐ�
	
	// ���x�̏�����
	velocity_ = { 0.0f,0.0f,0.0f };

	// �L���t���O�̏�����
	isActive_ = true;
}

void BaseCharacter::Update()
{
	///========================================
	/// ���[���h�g�����X�t�H�[��
	
	// �X�V
	worldTransform_->UpdateMatrix();
}

void BaseCharacter::Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	///========================================
	/// �I�u�W�F�N�g3D

	// �`��
	object3d_->Draw(*worldTransform_.get(), camera_->GetViewProjection(), directionalLight, pointLight, spotLight);
}

void BaseCharacter::DrawImGui()
{
}