#include "SkyDome.h"

void SkyDome::Initialize() {

	model_ = std::make_unique<Object3d>();
	model_->Initialize(Object3dCommon::GetInstance());

	ModelManager::GetInstance()->LoadModel("skyDome/skyDome.obj");
	model_->SetModel("skyDome/skyDome");

	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();

}

void SkyDome::Update() {
	worldTransform_->UpdateMatrix();
	model_->SetLocalMatrix(MakeIdentity4x4());
	model_->Update();
}

void SkyDome::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	model_->Draw(*worldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
}
