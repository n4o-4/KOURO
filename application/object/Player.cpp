#include "Player.h"
#include <cmath>
#include "imgui.h"
#include "Enemy.h"

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
	objectTransform_->transform.translate = { 0.0f, initialY_ , 3.0f };

	//========================================
	// 当たり判定との同期
	BaseObject::Initialize(objectTransform_->transform.translate, 1.0f);
}

void Player::Update()
{
	// 移動処理
	if (Input::GetInstance()->PushKey(DIK_W)) { Move({ 0.0f, 0.0f, 1.0f }); }
	if (Input::GetInstance()->PushKey(DIK_S)) { Move({ 0.0f, 0.0f, -1.0f }); }
	if (Input::GetInstance()->PushKey(DIK_A)) { Move({ -1.0f, 0.0f, 0.0f }); }
	if (Input::GetInstance()->PushKey(DIK_D)) { Move({ 1.0f, 0.0f, 0.0f }); }

	if (Input::GetInstance()->Triggerkey(DIK_RETURN)) {
		Shoot(); // スペースキーで弾を撃つ
	}

	// 弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
	}

	// 画面外に出た弾を削除
	bullets_.erase(std::remove_if(bullets_.begin(), bullets_.end(),
		[](const std::unique_ptr<PlayerBullet>& bullet) { return !bullet->IsActive(); }),
		bullets_.end());

	Move(Input::GetInstance()->GetLeftStick());

	// ジャンプ処理
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		isJumping_ = true;
		isFloating_ = false;
		fallSpeed_ = 0.0f;    // 降下速度リセット
		objectTransform_->transform.translate.y += speed_;  // 上昇
	} else if (isJumping_) {
		if (!isFloating_) {
			// SPACEを離した瞬間、追加上昇を開始（ただし徐々に減衰する）
			boostVelocity_ = floatBoost_;
			isFloating_ = true;
		}

		// 追加上昇を適用
		if (boostVelocity_ > 0.0f) {
			objectTransform_->transform.translate.y += boostVelocity_;
			boostVelocity_ -= boostDecay_; // 徐々に減衰
			if (boostVelocity_ < 0.0f) {
				boostVelocity_ = 0.0f;
			}
		}

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
			boostVelocity_ = 0.0f; // 追加上昇もリセット
		}
	}

	// カメラの更新
	if (followCamera_) {
		followCamera_->Update(this);
	}

	objectTransform_->UpdateMatrix();// 行列更新
	object3d_->SetLocalMatrix(MakeIdentity4x4());// ローカル行列を単位行列に
	object3d_->Update();// 更新

	//========================================
	// 当たり判定との同期
	BaseObject::Update(objectTransform_->transform.translate);
	// ImGui描画
	DrawImGui();
}

void Player::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight)
{
	// オブジェクトの描画
	object3d_->Draw(*objectTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);

	// 弾の描画
	for (auto& bullet : bullets_) {
		bullet->Draw(viewProjection, directionalLight, pointLight, spotLight);
	}
}

void Player::Finalize()
{
	lockOnSystem_ = nullptr;
}

void Player::DrawImGui()
{
	ImGui::Begin("Player Bullets");

	int index = 0;
	for (const auto& bullet : bullets_) {
		if (bullet->IsActive()) {
			Vector3 pos = bullet->GetPosition();
			ImGui::Text("Bullet %d: (%.2f, %.2f, %.2f)", index, pos.x, pos.y, pos.z);
		}
		index++;
	}

	ImGui::End();
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

void Player::Shoot() {
	Vector3 bulletPos = objectTransform_->transform.translate;
	Vector3 bulletScale = { 0.5f, 0.5f, 0.5f };
	Vector3 bulletRotate = { 0.0f, 0.0f, 0.0f };

	if (lockOnSystem_ && lockOnSystem_->GetLockedEnemyCount() > 0) {
		for (Enemy* enemy : lockOnSystem_->GetLockedEnemies()) {
			if (!enemy) continue;

			Vector3 enemyPos = enemy->GetPosition();
			Vector3 direction = Normalize(enemyPos - bulletPos);

			bullets_.emplace_back(std::make_unique<PlayerBullet>(bulletPos, direction * 0.5f, bulletScale, bulletRotate));
		}
	} else {
		Vector3 bulletVelocity = { 0.0f, 0.5f, 0.0f };
		bullets_.emplace_back(std::make_unique<PlayerBullet>(bulletPos, bulletVelocity, bulletScale, bulletRotate));
	}
}

///=============================================================================
///						当たり判定
///--------------------------------------------------------------
///						接触開始処理
void Player::OnCollisionEnter(BaseObject* other) {
	if (dynamic_cast<Enemy*>(other)) {
		isJumping_ = true;
	}
}

///--------------------------------------------------------------
///						接触継続処理
void Player::OnCollisionStay(BaseObject* other) {
	if (dynamic_cast<Enemy*>(other)) {
		isJumping_ = true;
	}
}

///--------------------------------------------------------------
///						接触終了処理
void Player::OnCollisionExit(BaseObject* other) {

}
