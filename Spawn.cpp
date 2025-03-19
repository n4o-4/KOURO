#include "Spawn.h"

void Spawn::Initialize() {
	spawnModel_ = std::make_unique<Object3d>();
	spawnModel_->Initialize(Object3dCommon::GetInstance());
	// ƒ‚ƒfƒ‹‚ð“Ç‚Ýž‚Þ
	ModelManager::GetInstance()->LoadModel("Spawn/Spawn.obj");
	spawnModel_->SetModel(ModelManager::GetInstance()->FindModel("Spawn/Spawn.obj"));

	spawnWorldTransform_ = std::make_unique<WorldTransform>();
	spawnWorldTransform_->Initialize();

}

void Spawn::Update() {

	spawnWorldTransform_->UpdateMatrix();
	spawnModel_->SetLocalMatrix(MakeIdentity4x4());
	spawnModel_->Update();
}

void Spawn::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	spawnModel_->Draw(*spawnWorldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
}
