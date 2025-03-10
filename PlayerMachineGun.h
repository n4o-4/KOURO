#pragma once
#include "BaseObject.h"
#include <Object3d.h>

class PlayerMachineGun : public BaseObject {
public:
	// �R���X�g���N�^
	PlayerMachineGun(const Vector3& position, const Vector3& velocity);

	// �X�V����
	void Update();

	// �`�揈��
	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight,
		PointLight pointLight, SpotLight spotLight);

	// �A�N�e�B�u��Ԃ̎擾
	bool IsActive() const { return isActive_; }

private:
	// �ړ����x
	Vector3 velocity_;

	// ���f��
	std::unique_ptr<Object3d> model_;
	std::unique_ptr<WorldTransform> worldTransform_;

	// �A�N�e�B�u���
	bool isActive_ = true;

	// �e�̎���
	int lifeTime_ = 0;
};
