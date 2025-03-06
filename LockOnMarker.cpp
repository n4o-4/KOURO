#include "LockOnMarker.h"

void LockOnMarker::Initialize() {

	//Multi lock on
	multiLockOn_ = std::make_unique<Object3d>();
	multiLockOn_->Initialize(Object3dCommon::GetInstance());
	ModelManager::GetInstance()->LoadModel("lockOn/Lock_on1.obj");
	multiLockOn_->SetModel("lockOn/Lock_on1.obj");

	multilockOnWorldTransform_ = std::make_unique<WorldTransform>();
	multilockOnWorldTransform_->Initialize();

	isVisible_ = false;

}

void LockOnMarker::Show() {
	isVisible_ = true;
}

void LockOnMarker::Hide() {
	isVisible_ = false;
}

void LockOnMarker::Update() {
	multilockOnWorldTransform_->UpdateMatrix();
	multiLockOn_->SetLocalMatrix(MakeIdentity4x4());
	multiLockOn_->Update();
}

void LockOnMarker::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	if (isVisible_) {
	multiLockOn_->Draw(*multilockOnWorldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
	}
}
