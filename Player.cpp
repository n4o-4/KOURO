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
	objectTransform_->transform.translate = { 0.0f, initialY_, 3.0f };
}

void Player::Update()
{
	// 移動処理
	if (Input::GetInstance()->PushKey(DIK_W)) { Move({ 0.0f, 0.0f, 1.0f }); }
	if (Input::GetInstance()->PushKey(DIK_S)) { Move({ 0.0f, 0.0f, -1.0f }); }
	if (Input::GetInstance()->PushKey(DIK_A)) { Move({ -1.0f, 0.0f, 0.0f }); }
	if (Input::GetInstance()->PushKey(DIK_D)) { Move({ 1.0f, 0.0f, 0.0f }); }

	// ジャンプ処理
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		isJumping_ = true;
		isFloating_ = false;
		fallSpeed_ = 0.0f;    // 降下速度リセット
		objectTransform_->transform.translate.y += speed_;  // 上昇
	} else if (isJumping_) {
		if (!isFloating_) {
			// SPACEを離した瞬間、少しだけ上昇
			objectTransform_->transform.translate.y += floatBoost_;
			isFloating_ = true;  // 浮遊状態へ移行
		} else {
			// 下降処理（徐々に加速）
			fallSpeed_ += gravity_;
			if (fallSpeed_ > maxFallSpeed_) {
				fallSpeed_ = maxFallSpeed_; // 降下速度の上限を設定
			}
			objectTransform_->transform.translate.y -= fallSpeed_;

			// 着地判定
			if (objectTransform_->transform.translate.y <= initialY_) {
				objectTransform_->transform.translate.y = initialY_;
				isJumping_ = false;
				isFloating_ = false;
				fallSpeed_ = 0.0f;
			}
		}
	}

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

void Player::Jump()
{
	if (!isJumping_) {
		isJumping_ = true;
		jumpVelocity_ = 0.2f; // 初速度
	}
}
