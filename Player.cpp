#include "Player.h"
#include <cmath>

void Player::Initialize()
{
	// Object3d を初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize(Object3dCommon::GetInstance());

	// モデルを設定
	ModelManager::GetInstance()->LoadModel("player.obj");
	object3d_->SetModel("player.obj");
	// 初期位置を設定
	objectTransform_ = std::make_unique<WorldTransform>();
	objectTransform_->Initialize();
	objectTransform_->transform.translate = { 0.0f, -2.0f, 3.0f };
}

void Player::Update()
{
	// キー入力で移動
	if (Input::GetInstance()->PushKey(DIK_W)) { Move({ 0.0f, 0.0f, 1.0f }); }  // 前進
	if (Input::GetInstance()->PushKey(DIK_S)) { Move({ 0.0f, 0.0f, -1.0f }); } // 後退
	if (Input::GetInstance()->PushKey(DIK_A)) { Move({ -1.0f, 0.0f, 0.0f }); } // 左移動
	if (Input::GetInstance()->PushKey(DIK_D)) { Move({ 1.0f, 0.0f, 0.0f }); }  // 右移動
	// 座標を適用
	//objectTransform_->transform.translate = position_;

	objectTransform_->UpdateMatrix();

	object3d_->SetLocalMatrix(MakeIdentity4x4());

	object3d_->Update();
}

void Player::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	// オブジェクトの描画
	object3d_->Draw(*objectTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);
}

void Player::Finalize()
{

}

void Player::Move(Vector3 direction)
{
	// `MyMath.h` の `Normalize()` を使用して正規化
	direction = Normalize(direction);

	// 速度を適用して移動
	objectTransform_->transform.translate.x += direction.x * speed_;
	objectTransform_->transform.translate.y += direction.y * speed_;
	objectTransform_->transform.translate.z += direction.z * speed_;
}
