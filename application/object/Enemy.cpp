#include "Enemy.h"

void Enemy::Initialize() {
	model_ = std::make_unique<Object3d>();
	model_->Initialize(Object3dCommon::GetInstance());

	ModelManager::GetInstance()->LoadModel("enemy/enemy.obj");
	model_->SetModel("enemy/enemy.obj");

	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
}

void Enemy::Update() {
	worldTransform_->UpdateMatrix();
	model_->SetLocalMatrix(MakeIdentity4x4());
	model_->Update();
}

void Enemy::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	model_->Draw(*worldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
}
