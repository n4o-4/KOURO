#include "SkyDome.h"

void SkyDome::Initialize(std::unique_ptr<Object3d> model) {
	model_ = std::move(model);

	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initilaize();

	pointLight_ = std::make_unique<PointLight>();
	pointLight_->Initilize();

	spotLight_ = std::make_unique<SpotLight>();
	spotLight_->Initialize();

	
}

void SkyDome::Update() {
	worldTransform_->UpdateMatrix();
}

void SkyDome::Draw() {
	model_->Draw(*worldTransform_.get(), Camera::GetInstance()->GetViewProjection(), *directionalLight_.get(), *pointLight_.get(), *spotLight_.get());
}
