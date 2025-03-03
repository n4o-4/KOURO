#pragma once
#include "Kouro.h"

class PakuScene;

class Enemy {


public:
	void Initialize();
	void Update();
	void Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight);

	void SetGameScene(PakuScene* gameScene) { gameScene_ = gameScene; }

	void SetPosition(const Vector3& position) { worldTransform_->transform.translate = position; }

private:

	std::unique_ptr<Object3d> model_ = nullptr;

	std::unique_ptr<WorldTransform> worldTransform_ = nullptr;


	PakuScene* gameScene_ = nullptr;



};

