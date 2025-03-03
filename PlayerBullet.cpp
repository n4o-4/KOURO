#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(Vector3 position, Vector3 velocity, Vector3 scale, Vector3 rotate)
	: velocity_(velocity) {

	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(Object3dCommon::GetInstance());
	ModelManager::GetInstance()->LoadModel("bullet.obj");
	object3d_->SetModel("bullet.obj");

	// `objectTransform_` を `unique_ptr` で管理
	objectTransform_ = std::make_unique<WorldTransform>();
	objectTransform_->Initialize();
	objectTransform_->transform.translate = position;
	objectTransform_->transform.scale = scale;
	objectTransform_->transform.rotate = rotate;
}

void PlayerBullet::Update() {
	objectTransform_->transform.translate = objectTransform_->transform.translate + velocity_;

	// 回転を加える（例: Z軸を毎フレーム 5 度回転）
	objectTransform_->transform.rotate.z += 5.0f;
	if (objectTransform_->transform.rotate.z > 360.0f) {
		objectTransform_->transform.rotate.z -= 360.0f;
	}

	// 画面奥に行き過ぎたら消す
	if (objectTransform_->transform.translate.z > 50.0f) {
		isActive_ = false;
	}

	// 行列を更新
	objectTransform_->UpdateMatrix();
}

void PlayerBullet::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	if (!isActive_) return;

	object3d_->Draw(*objectTransform_, viewProjection, directionalLight, pointLight, spotLight);
}
