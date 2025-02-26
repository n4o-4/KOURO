#pragma once
#include "Object3d.h"
#include "Kouro.h"
class SkyDome {

public:
	void Initialize(std::unique_ptr<Object3d> model);
	void Update();
	void Draw();

private:

	std::unique_ptr<Object3d> model_ = nullptr;

	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;

	std::unique_ptr<DirectionalLight> directionalLight_ = nullptr;

	std::unique_ptr<PointLight> pointLight_ = nullptr;

	std::unique_ptr<SpotLight> spotLight_ = nullptr;
};

