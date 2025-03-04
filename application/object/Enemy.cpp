#include "Enemy.h"

///=============================================================================
///						初期化
void Enemy::Initialize() {\
	//========================================
	// モデルを初期化
	model_ = std::make_unique<Object3d>();
	model_->Initialize(Object3dCommon::GetInstance());
	//========================================
	// モデルを読み込む
	ModelManager::GetInstance()->LoadModel("enemy/enemy.obj");
	model_->SetModel("enemy/enemy.obj");
	//========================================
	// 初期位置を設定
	worldTransform_ = std::make_unique<WorldTransform>();
	worldTransform_->Initialize();
	//========================================
	// 当たり判定との同期
	BaseObject::Initialize(worldTransform_->transform.translate, 1.0f);
}
///=============================================================================
///						描画
void Enemy::Update() {
	//========================================
	// ワールド変換の更新
	worldTransform_->UpdateMatrix();
	// モデルのワールド変換行列を更新
	model_->SetLocalMatrix(MakeIdentity4x4());
	// モデルの更新
	model_->Update();
	//========================================
	// 当たり判定との同期
	BaseObject::Update(worldTransform_->transform.translate);
}
///=============================================================================
///						描画
void Enemy::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	//========================================
	// モデルの描画
	model_->Draw(*worldTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
}

///=============================================================================
///						当たり判定
///--------------------------------------------------------------
///						接触開始処理
void Enemy::OnCollisionEnter(BaseObject *other) {
}

///--------------------------------------------------------------
///						接触継続処理
void Enemy::OnCollisionStay(BaseObject *other) {
}

///--------------------------------------------------------------
///						接触終了処理
void Enemy::OnCollisionExit(BaseObject *other) {

}