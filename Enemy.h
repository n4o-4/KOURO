#pragma once
#include "BaseCharacter.h"
class Enemy : public BaseCharacter
{
public: // ���J�����o�֐�
	// ����������
	void Initialize(Model* model) override;
	// �X�V����
	void Update() override;
	// �`�揈��
	void Draw(DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) override;

	void SetPosition(const Vector3& position) {
		if (worldTransform_->useQuaternion_) {
			worldTransform_->quaternionTransform.translate = position;
		}
		else {
			worldTransform_->transform.translate = position;
		}
		worldTransform_->UpdateMatrix();
	}

private: // ����J�����o�֐�

	

};

