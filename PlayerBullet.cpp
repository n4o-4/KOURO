#include "PlayerBullet.h"

PlayerBullet::PlayerBullet(Vector3 position, Vector3 velocity, Vector3 scale, Vector3 rotate)
	: velocity_(velocity) {

	Initialize(position, velocity, scale, rotate);
}

void PlayerBullet::Initialize(Vector3 position, Vector3 velocity, Vector3 scale, Vector3 rotate)
{
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(Object3dCommon::GetInstance());
	ModelManager::GetInstance()->LoadModel("player.obj");
	object3d_->SetModel("player.obj");

	objectTransform_ = std::make_unique<WorldTransform>();
	objectTransform_->Initialize();
	objectTransform_->transform.translate = position;
	objectTransform_->transform.scale = scale;
	objectTransform_->transform.rotate = rotate;

	//========================================
	// 当たり判定との同期
	BaseObject::Initialize(objectTransform_->transform.translate, 1.0f);
}

void PlayerBullet::Update() {
	objectTransform_->transform.translate = objectTransform_->transform.translate + velocity_;

	// 回転を加える（例: Z軸を毎フレーム 5 度回転）
	objectTransform_->transform.rotate.z += 5.0f;
	if (objectTransform_->transform.rotate.z > 360.0f) {
		objectTransform_->transform.rotate.z -= 360.0f;
	}

	// 画面奥に行き過ぎたら消す
	if (objectTransform_->transform.translate.z > 300.0f) {
		isActive_ = false;
	}

	// 行列を更新
	objectTransform_->UpdateMatrix();
	object3d_->SetLocalMatrix(MakeIdentity4x4());// ローカル行列を単位行列に
	object3d_->Update();// 更新

	//========================================
	// 当たり判定との動機
	BaseObject::Update(objectTransform_->transform.translate);
}

void PlayerBullet::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	if (!isActive_) return;

	object3d_->Draw(*objectTransform_, viewProjection, directionalLight, pointLight, spotLight);
}

///=============================================================================
///						当たり判定
///--------------------------------------------------------------
///						接触開始処理
void PlayerBullet::OnCollisionEnter(BaseObject *other) {

}

///--------------------------------------------------------------
///						接触継続処理
void PlayerBullet::OnCollisionStay(BaseObject *other) {

}

///--------------------------------------------------------------
///						接触終了処理
void PlayerBullet::OnCollisionExit(BaseObject *other) {
}

