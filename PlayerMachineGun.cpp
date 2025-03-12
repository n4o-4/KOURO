#include "PlayerMachineGun.h"
#include "Enemy.h"

PlayerMachineGun::PlayerMachineGun(const Vector3& position, const Vector3& velocity) {
	model_ = std::make_unique<Object3d>();
	model_->Initialize(Object3dCommon::GetInstance());

	ModelManager::GetInstance()->LoadModel("player.obj");
	model_->SetModel("player.obj");

	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	worldTransform_->transform.translate = position;
	worldTransform_->transform.scale = { 0.2f, 0.2f, 0.2f };  // マシンガンの弾は小さめ
	velocity_ = velocity;

	BaseObject::Initialize(worldTransform_->transform.translate, 0.5f);  // 当たり判定のサイズ
}

void PlayerMachineGun::Update() {
	lifeTime_++;

	// 一定時間で消滅
	if (lifeTime_ > 120) {  // 約2秒
		isActive_ = false;
	}

	// 移動処理
	worldTransform_->transform.translate = worldTransform_->transform.translate + velocity_;
	model_->SetLocalMatrix(MakeIdentity4x4());// ローカル行列を単位行列に
	worldTransform_->UpdateMatrix();

	BaseObject::Update(worldTransform_->transform.translate);
}

void PlayerMachineGun::Draw(ViewProjection viewProjection, DirectionalLight directionalLight,
	PointLight pointLight, SpotLight spotLight) {
	if (isActive_) {
		model_->Draw(*worldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
	}
}

///=============================================================================
///                        当たり判定の実装
///=============================================================================
void PlayerMachineGun::OnCollisionEnter(BaseObject* other) {
	// 敵接触
	if (Enemy* enemy = dynamic_cast<Enemy*>(other)) {
		//---------------------------------------
		//弾を消す
		isActive_ = false;
	}
}

void PlayerMachineGun::OnCollisionStay(BaseObject* other) {
	// 今回は特に処理なし
}

void PlayerMachineGun::OnCollisionExit(BaseObject* other) {
	// 今回は特に処理なし
}
