#include "SkyDome.h"

void SkyDome::Initialize() {

	model_ = std::make_unique<Object3d>();
	model_->Initialize(Object3dCommon::GetInstance());

	ModelManager::GetInstance()->LoadModel("skyDome/skyDome.obj");
	model_->SetModel("skyDome/skyDome.obj");

	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->transform.scale = { 1.0f,1.0f,1.0f };
}

void SkyDome::Update() {

#ifdef _DEBUG
	if (ImGui::TreeNode("skyDome")) {
		ImGui::DragFloat3("worldTransform_", &worldTransform_->transform.scale.x, 0.01f);

		ImGui::TreePop(); // TreeNodeを閉じる
	}

#endif

	worldTransform_->UpdateMatrix();
	model_->SetLocalMatrix(MakeIdentity4x4());
	model_->Update();
}

void SkyDome::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	model_->Draw(*worldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
}
