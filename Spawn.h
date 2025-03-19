#pragma once
#include "Kouro.h"

class Spawn {
public:
	void Initialize();
	void Update();
	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

	void SetPosition(const Vector3& position) {
		spawnWorldTransform_->transform.translate = position;
	}
private:
	//Spawn model
	std::unique_ptr<Object3d> spawnModel_ = nullptr;
	//Spawn ƒ[ƒ‹ƒh•ÏŠ·
	std::unique_ptr<WorldTransform> spawnWorldTransform_ = nullptr;
};

