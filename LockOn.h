#pragma once
#include "Kouro.h"
#include "BaseObject.h"
#include "Enemy.h"
#include "LockOnMarker.h"

class LockOn {

public:

	void Initialize(); 
	void Update();
	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

	void SetPosition(const Vector3& position) { lockOnWorldTransform_->transform.translate = position; }
	// LockOn �͈͓��̓G�����m����֐�
	void DetectEnemies(const std::vector<std::unique_ptr<Enemy>>& enemies);
	//���o���ꂽ�G�̃��X�g���擾����֐�
	size_t GetLockedEnemyCount() const { return lockedEnemies_.size(); }

private:

	//lockOn model
	std::unique_ptr<Object3d> lockOn_ = nullptr;
	//���[���h�ϊ�
	std::unique_ptr<WorldTransform> lockOnWorldTransform_ = nullptr;

	// ���o���ꂽ�G�̃��X�g
	std::vector<Enemy*> lockedEnemies_;

	//���o���ꂽEnemy�̑O�ɕ\�������I�u�W�F�N�g
	std::vector<std::unique_ptr<LockOnMarker>> lockOnMarkers_;

};

