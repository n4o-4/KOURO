#include "PlayerMachineGun.h"

PlayerMachineGun::PlayerMachineGun(const Vector3& position, const Vector3& velocity) {
	model_ = std::make_unique<Object3d>();
	model_->Initialize(Object3dCommon::GetInstance());

	ModelManager::GetInstance()->LoadModel("machinegun_bullet.obj");
	model_->SetModel("machinegun_bullet.obj");

	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->transform.translate = position;
	worldTransform_->transform.scale = { 0.2f, 0.2f, 0.2f };  // ƒ}ƒVƒ“ƒKƒ“‚Ì’e‚Í¬‚³‚ß
	velocity_ = velocity;

	BaseObject::Initialize(worldTransform_->transform.translate, 0.5f);  // “–‚½‚è”»’è‚ÌƒTƒCƒY
}

void PlayerMachineGun::Update() {
	lifeTime_++;

	// ˆÚ“®ˆ—
	worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;

	// ˆê’èŽžŠÔ‚ÅÁ–Å
	if (lifeTime_ > 120) {  // –ñ2•b
		isActive_ = false;
	}

	BaseObject::Update(worldTransform_->transform.translate);
}

void PlayerMachineGun::Draw(ViewProjection viewProjection, DirectionalLight directionalLight,
	PointLight pointLight, SpotLight spotLight) {
	if (isActive_) {
		model_->Draw(*worldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
	}
}
