#define _USE_MATH_DEFINES
#define NOMINMAX
#include <algorithm>
#include "Player.h"
#include <cmath>
#include "MyMath.h"
#include "imgui.h"
#include "BaseEnemy.h"
#include "EnemyBullet.h"
#include "Vectors.h"

void Player::Initialize() {
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

	explosionEmitter_ = std::make_unique<ExplosionEmitter>();
	explosionEmitter_->Initialize("missileSmoke");
	// パーティクル設定の調整

	explosionEmitter_->SetParticleCount(10);
	explosionEmitter_->SetFrequency(0.04f);
	explosionEmitter_->SetLifeTimeRange(ParticleManager::LifeTimeRange({0.01f,0.01f}));
	//========================================
	// 当たり判定との同期
	BaseObject::Initialize(objectTransform_->transform.translate, 1.0f);
}
///=============================================================================
///                        更新処理
void Player::Update() {
	// 移動入力の取得
	Vector3 inputDirection = GetMovementInput();

	// ブースト処理
	HandleBoost();

	// 移動処理
	UpdateMove(inputDirection);

	// 反動の適用
	ApplyRecoil();

	// 揺れの適用
	ApplyShake();

	// 弾の処理
	UpdateBullets();

	// ジャンプ処理
	UpdateJump();

	// ブースト回転中処理
	if (isBoostSpinning_) {
		boostSpin_ += 0.4f;
		boostSpinFrame_++;

		if (boostSpin_ >= 2.0f * 3.14159265f || boostSpinFrame_ >= 16) { // 16フレーム（約0.26秒）で1回転想定
			boostSpin_ = 0.0f;
			isBoostSpinning_ = false;
			boostSpinFrame_ = 0;

			// ❗滑らかに戻さず、ピタッと元の角度へ
			objectTransform_->transform.rotate.x = 0.0f;
		} else {
			objectTransform_->transform.rotate.x += 0.4f;
		}
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
///=============================================================================
///                        描画
void Player::Draw(ViewProjection viewProjection, DirectionalLight directionalLight, PointLight pointLight, SpotLight spotLight) {
	// オブジェクトの描画
	object3d_->Draw(*objectTransform_.get(), viewProjection, directionalLight, pointLight, spotLight);

	// 弾の描画
	for (auto& bullet : bullets_) {
		bullet->Draw(viewProjection, directionalLight, pointLight, spotLight);
	}

	//マシンガンの描画
	for (auto& bullet : machineGunBullets_) {
		bullet->Draw(viewProjection, directionalLight, pointLight, spotLight);
	}
}
///=============================================================================
///                        終了処理
void Player::Finalize() {
	lockOnSystem_ = nullptr;
}
///=============================================================================
///                        Imgui描画
void Player::DrawImGui() {

#ifdef _DEBUG

	ImGui::Begin("Player Status");

	Vector3 pos = objectTransform_->transform.translate;
	Vector3 rot = objectTransform_->transform.rotate;
	Vector3 vel = velocity_;
	ImGui::Text("Position: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
	ImGui::Text("Rotation: (%.2f, %.2f, %.2f)", rot.x, rot.y, rot.z);
	ImGui::Text("Velocity: (%.2f, %.2f, %.2f)", vel.x, vel.y, vel.z);

	ImGui::Text("Speed: %.2f", Length(vel));
	ImGui::Text("Boost: %.1f / %.1f", currentBoostTime_, maxBoostTime_);
	ImGui::Text("Is Jumping: %s", isJumping_ ? "Yes" : "No");
	ImGui::Text("Is Boosting: %s", isBoosting_ ? "Yes" : "No");

	ImGui::End();

	// 弾の情報も表示
	ImGui::Begin("Player Bullets");
	int index = 0;
	for (const auto& bullet : bullets_) {
		if (bullet->IsActive()) {
			Vector3 pos = bullet->GetCollider()->GetPosition();
			ImGui::Text("Bullet %d: (%.2f, %.2f, %.2f)", index, pos.x, pos.y, pos.z);
		}
		index++;
	}
	ImGui::End();

#endif
}
///=============================================================================
///                        静的メンバ関数
///--------------------------------------------------------------
///                        移動入力の取得
Vector3 Player::GetMovementInput() {
	Vector3 inputDirection = { 0.0f, 0.0f, 0.0f };
	// 移動入力の取得
	if (Input::GetInstance()->PushKey(DIK_W)) { inputDirection.z += 1.0f; }
	if (Input::GetInstance()->PushKey(DIK_S)) { inputDirection.z -= 1.0f; }
	if (Input::GetInstance()->PushKey(DIK_A)) { inputDirection.x -= 1.0f; }
	if (Input::GetInstance()->PushKey(DIK_D)) { inputDirection.x += 1.0f; }
	// アナログスティック入力を合成
	Vector3 stickInput = Input::GetInstance()->GetLeftStick();
	inputDirection.x += stickInput.x;
	inputDirection.z += stickInput.z;

	Matrix4x4 rotateMatrix = MakeRotateMatrix(followCamera_->GetViewProjection().transform.rotate);

	inputDirection = TransformNormal(inputDirection, rotateMatrix);

	inputDirection.y = 0.0f;

	return inputDirection;
}
///--------------------------------------------------------------
///                        移動
void Player::UpdateMove(Vector3 direction) {
	// 入力方向の正規化
	if (Length(direction) > 0.0f) {
		direction = Normalize(direction);

		// 空中と地上で操作感度を変える
		float controlFactor = isJumping_ ? airControlFactor_ : 1.0f;

		// 入力方向に加速
		acceleration_ = direction * (accelerationRate_ * controlFactor);
	} else {
		// 入力がない場合は加速度をゼロに
		acceleration_ = { 0.0f, 0.0f, 0.0f };
	}

	// 加速度を速度に適用
	velocity_ = (velocity_ + acceleration_);

	// 最大速度を制限
	float speedLimit = isBoosting_ ? maxSpeed_ * boostFactor_ : maxSpeed_;
	float currentSpeed = Length(velocity_);

	// ブースト終了時に急減速しないようにする
	// 現在の速度が制限を超えていて、かつブースト中でない場合は徐々に減速
	if (currentSpeed > speedLimit) {
		if (isBoosting_) {
			// ブースト中は即座に最大速度まで加速
			velocity_ = (Normalize(velocity_) * speedLimit);
		} else {
			// ブースト終了後は徐々に減速（減速率はフレームごとに95%に）
			float slowDownRate = 0.95f;
			float targetSpeed = std::max(speedLimit, currentSpeed * slowDownRate);
			velocity_ = (Normalize(velocity_) * targetSpeed);
		}
	}

	// 摩擦による減速（入力がない場合のみ）
	if (Length(direction) < 0.1f) {
		// 現在の速度方向を記録
		Vector3 velocityDirection = { 0.0f, 0.0f, 0.0f };
		if (currentSpeed > 0.0f) {
			velocityDirection = Normalize(velocity_);
		}

		// 摩擦を適用
		if (currentSpeed > 0.0f) {
			Vector3 frictionForce = velocityDirection * -friction_;
			velocity_ = velocity_ + frictionForce;
		}

		// 摩擦適用後の速度が非常に小さい場合は停止
		// 閾値を0.001から0.01に上げて、より確実に停止するようにする
		if (Length(velocity_) < 0.01f) {
			velocity_ = { 0.0f, 0.0f, 0.0f };
		}

		// 摩擦で速度が反転してしまった場合も停止させる
		// Dotが使えない場合は、X軸とZ軸それぞれで方向が逆転したかチェック
		if (currentSpeed > 0.0f) {
			if ((velocityDirection.x * velocity_.x < 0 && fabsf(velocityDirection.x) > 0.01f) ||
				(velocityDirection.z * velocity_.z < 0 && fabsf(velocityDirection.z) > 0.01f)) {
				velocity_ = { 0.0f, 0.0f, 0.0f };
			}
		}
	}

	// 移動が入力されているときだけ
	if (std::abs(Length(velocity_)) > 0) {
		distinationRotateY_ = std::atan2(velocity_.x, velocity_.z);
	}

	// プレイヤーの向き
	objectTransform_->transform.rotate.y = LerpShortAngle(objectTransform_->transform.rotate.y, distinationRotateY_, 0.1f);
	// 位置の更新
	objectTransform_->transform.translate = (objectTransform_->transform.translate + velocity_);
}
///--------------------------------------------------------------
///                        ジャンプ
void Player::UpdateJump() {
	if (!isJumping_) {
		// スペースキーが押されたらジャンプ開始
		if (Input::GetInstance()->Triggerkey(DIK_SPACE) ||
			Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::A)) {
			isJumping_ = true;
			jumpVelocity_ = 0.6f;  // 初速を設定
			fallSpeed_ = 0.0f;     // 降下速度リセット
		}
	}

	if (isJumping_) {
		// 上昇処理（ふわっと浮く）
		objectTransform_->transform.translate.y += jumpVelocity_;

		// 上昇速度をゆっくり減衰
		jumpVelocity_ -= 0.005f;  // これ以上減衰を強くすると上がらない可能性がある

		// 上昇が終了したら降下開始
		if (jumpVelocity_ <= 0.0f) {
			jumpVelocity_ = 0.0f;
			fallSpeed_ += gravity_;
			if (fallSpeed_ > maxFallSpeed_) {
				fallSpeed_ = maxFallSpeed_;
			}
			objectTransform_->transform.translate.y -= fallSpeed_;
		}

		// 着地判定
		if (objectTransform_->transform.translate.y <= initialY_) {
			objectTransform_->transform.translate.y = initialY_;
			isJumping_ = false;
			jumpVelocity_ = 0.0f;
			fallSpeed_ = 0.0f;
		}
	}
}

///                        ジャンプしているか
void Player::IsJump() {
	if (!isJumping_) {
		isJumping_ = true;
		jumpVelocity_ = 0.2f; // 初速度
	}
}
///--------------------------------------------------------------
///						 弾の処理と更新
void Player::UpdateBullets() {
	if (Input::GetInstance()->Triggerkey(DIK_RETURN) || Input::GetInstance()->TriggerGamePadButton(Input::GamePadButton::RIGHT_SHOULDER)) {
		Shoot();
	}
	// 弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
	}
	// 画面外に出た弾を削除
	bullets_.erase(std::remove_if(bullets_.begin(), bullets_.end(),
		[](const std::unique_ptr<PlayerMissile>& bullet) { return !bullet->IsActive(); }),
		bullets_.end());

	//マシンガンの弾の更新
	 // マシンガンの発射
	if (Input::GetInstance()->PushKey(DIK_J) ||
		Input::GetInstance()->PushGamePadButton(Input::GamePadButton::LEFT_SHOULDER)) {
		isShootingMachineGun_ = true;
	} else {
		isShootingMachineGun_ = false;
	}

	if (isShootingMachineGun_ && machineGunCooldown_ <= 0) {
		ShootMachineGun();
		machineGunCooldown_ = 5;  // 一定間隔で発射
	}

	if (machineGunCooldown_ > 0) {
		machineGunCooldown_--;
	}

	// マシンガン弾の更新
	for (auto& bullet : machineGunBullets_) {
		bullet->Update();
	}

	// 不要な弾の削除
	machineGunBullets_.erase(std::remove_if(machineGunBullets_.begin(), machineGunBullets_.end(),
		[](const std::unique_ptr<PlayerMachineGun>& bullet) { return !bullet->IsActive(); }),
		machineGunBullets_.end());
}
///                        射撃
void Player::Shoot() {
	Vector3 bulletPos = objectTransform_->transform.translate;
	Vector3 bulletScale = { 0.5f, 0.5f, 0.5f };
	Vector3 bulletRotate = { 0.0f, 0.0f, 0.0f };

    if (lockOnSystem_ && lockOnSystem_->GetLockedEnemyCount() > 0) {
        for (BaseEnemy* enemy : lockOnSystem_->GetLockedEnemies()) {
            if (!enemy) continue;
			
            // 敵ごとのロックオンレベルを取得
            LockOn::LockLevel lockLevel = lockOnSystem_->GetLockLevel(enemy);
            int lockLevelValue = static_cast<int>(lockLevel);  // 数値に変換

			Vector3 enemyPos = enemy->GetPosition();
			Vector3 direction = Normalize(enemyPos - bulletPos);

			// ロックオンレベルに応じて初速と挙動を変更
			Vector3 initialVelocity;
			if (lockLevel == LockOn::LockLevel::PreciseLock) {
				// 精密ロックオン：より直線的な初速
				initialVelocity = Normalize((direction * 0.7f) + Vector3{ 0.0f, 0.3f, 0.0f });
				initialVelocity = initialVelocity * 0.35f;  // 初速は少し速く
			} else {
				// 簡易ロックオン：上向きに弧を描く初速
				initialVelocity = Normalize((direction * 0.3f) + Vector3{ 0.0f, 0.7f, 0.0f });
				initialVelocity = initialVelocity * 0.25f;  // 初速は少し遅め
			}

			// 新しい弾を作成してターゲットを設定
			auto newBullet = std::make_unique<PlayerMissile>(
				bulletPos, initialVelocity, bulletScale, bulletRotate, lockLevelValue);
			newBullet->SetTarget(enemy); // ターゲットを設定
			bullets_.push_back(std::move(newBullet));

			// 発射エフェクト（ロックオンレベルに応じて調整）
			if (lockLevel == LockOn::LockLevel::PreciseLock) {
				// TODO:精密ロックオンのエフェクト（より派手に）
				//ParticleManager::GetInstance()->Emit("missileTrail", bulletPos, 10);
			} else {
				// TODO:簡易ロックオンのエフェクト（控えめに）
				//ParticleManager::GetInstance()->Emit("missileTrail", bulletPos, 5);
			}
		}
	} else {
		// ロックオンがない場合は上方向に発射（通常ミサイル）
		Vector3 bulletVelocity = { 0.0f, 0.5f, 0.0f };
		bullets_.push_back(std::make_unique<PlayerMissile>(bulletPos, bulletVelocity, bulletScale, bulletRotate, 0));
	}

	// 発射時の反動や効果音はロックオンレベルに関わらず共通
	// ...既存コード...
}

///--------------------------------------------------------------
///                        ブースト処理
bool Player::HandleBoost() {
	bool stateChanged = false;

	// クイックブーストのクールダウンを減少
	if (quickBoostCooldown_ > 0.0f) {
		quickBoostCooldown_ -= 1.0f;
	}

	// クイックブースト中の処理
	if (isQuickBoosting_) {
		// フレームカウントを減らす
		quickBoostFrames_--;

		// クイックブースト中は一定の高速度を維持する
		if (Length(velocity_) > 0.01f) {
			Vector3 boostDirection = Normalize(velocity_);
			float boostSpeed = maxSpeed_ * 4.0f; // 通常速度の4倍の速さ

			// フレーム後半になると徐々に減速（緩急をつける）
			if (quickBoostFrames_ < maxQuickBoostFrames_ / 2) {
				float ratio = static_cast<float>(quickBoostFrames_) / (maxQuickBoostFrames_ / 2);
				boostSpeed *= (0.7f + 0.3f * ratio); // 70%～100%の間で調整
			}

			// 速度を直接設定（加算ではなく）
			velocity_ = boostDirection * boostSpeed;
		}

		// クイックブースト終了判定
		if (quickBoostFrames_ <= 0) {
			isQuickBoosting_ = false;
			// 急に遅くならないように減速率を設定
			velocity_ = velocity_ * 0.8f;
			stateChanged = true;
		}
	}
	// 通常状態での入力処理
	else {
		// ブースト入力の検出
		bool boostInput = Input::GetInstance()->Triggerkey(DIK_LSHIFT) ||
			Input::GetInstance()->PushGamePadButton(Input::GamePadButton::X);

		// クイックブースト開始条件
		if (boostInput && quickBoostCooldown_ <= 0.0f && currentBoostTime_ >= quickBoostConsumption_) {
			isQuickBoosting_ = true;
			quickBoostFrames_ = maxQuickBoostFrames_;
			currentBoostTime_ -= quickBoostConsumption_;
			quickBoostCooldown_ = maxQuickBoostCooldown_;

			Vector3 boostDirection;

			Vector3 inputDirection = { 0.0f, 0.0f, 0.0f };

			// 移動入力の取得
			Vector3 stickInput = Input::GetInstance()->GetLeftStick();
			inputDirection.x += stickInput.x;
			inputDirection.z += stickInput.z;

			// カメラの向きに合わせて入力方向を変換
			Vector3 rotate = followCamera_->GetViewProjection().transform.rotate;
			rotate.x = 0.0f;

			Matrix4x4 rotateMatrix = MakeRotateMatrix(rotate);

			inputDirection = TransformNormal(inputDirection, rotateMatrix);

			// ✅ 方向決定 & 回転条件分岐
			if (Length(inputDirection) > 0.0f) 
			{
				boostDirection = Normalize(inputDirection);
			} 
			else if (Length(velocity_) > 0.01f) 
			{
				boostDirection = Normalize(velocity_);
			} 
			else
			{
				// ❌ 動いてなかったら演出スキップ
				return stateChanged;
			}

			// ✅ 回転開始
			boostSpin_ = 0.0f;
			isBoostSpinning_ = true;

			// 加速ベクトルを適用
			//acceleration_ = boostDirection * (accelerationRate_ * 8.0f);
			velocity_ = boostDirection * (accelerationRate_ * 8.0f);

			stateChanged = true;
		}


		// ブースト回復（非クイックブースト中のみ）
		if (currentBoostTime_ < maxBoostTime_) {
			float recoveryMultiplier = (Length(velocity_) < 0.05f) ? 2.0f : 1.0f;
			currentBoostTime_ += boostRecoveryRate_ * recoveryMultiplier;
			if (currentBoostTime_ > maxBoostTime_) currentBoostTime_ = maxBoostTime_;
		}
	}

	return stateChanged;
}
void Player::ShootMachineGun() {
	// 弾の初期位置をプレイヤーの位置に設定
	Vector3 bulletPos = objectTransform_->transform.translate;

	// プレイヤーの向いている方向を取得（Y軸の回転だけ考慮）
	Vector3 forward = { 0.0f, 0.0f, 1.0f };  // Z方向が基準
	Matrix4x4 rotationMatrix = MakeRotateMatrix(followCamera_->GetViewProjection().transform.rotate);
	forward = TransformNormal(forward, rotationMatrix);

	// **ランダムなばらつきを加える**
	float spread = 0.2f;  // ばらつきの強さ（大きくするともっと弾が広がる）
	float randomX = ((rand() % 100) / 100.0f - 0.5f) * spread; // -spread/2 ～ spread/2
	float randomY = ((rand() % 100) / 100.0f - 0.5f) * spread; // -spread/2 ～ spread/2

	// ばらつきを適用
	Vector3 bulletVelocity = forward * 1.5f + Vector3(randomX, randomY, 0.0f);
	bulletVelocity = Normalize(bulletVelocity) * 1.5f;  // 速度を一定に

	// マシンガン弾を生成
	machineGunBullets_.push_back(std::make_unique<PlayerMachineGun>(bulletPos, bulletVelocity));

	// **反動を適用**
	Vector3 recoilDirection = -forward;
	recoilVelocity_ += recoilDirection * recoilStrength_;

	// **揺れを適用**
	shakeIntensity_ = 2.0f;

	explosionEmitter_->SetPosition(objectTransform_.get()->transform.translate);
	explosionEmitter_->Emit();
}

void Player::ApplyRecoil() {

	if (Length(recoilVelocity_) > 0.001f) {
		/// 反動を適用=======================================
		objectTransform_->transform.translate.x += recoilVelocity_.x; // X軸
		objectTransform_->transform.translate.z += recoilVelocity_.z; // Z軸
		//Y軸は反動を適用しない
		///=================================================

		recoilVelocity_ *= recoilDecay_; // 減衰
	} else {
		recoilVelocity_ = { 0.0f, 0.0f, 0.0f }; // ゼロに戻す
	}

}
void Player::ApplyShake() {
	if (shakeIntensity_ > 0.01f) {  // ある程度の揺れが残っているとき
		float shakeAmount = shakeIntensity_ * 0.1f;  // **揺れの強さを 2 倍に！**

		// ランダムな揺れを適用（-1.0f ~ 1.0f の範囲）
		float offsetX = (rand() % 80 - 40) * shakeAmount * 0.02f;  // **横揺れ強化**
		//float offsetY = (rand() % 100 - 50) * shakeAmount * 0.015f; // **縦揺れ強化**
		float offsetRot = (rand() % 200 - 100) * shakeAmount * 0.002f; // **回転揺れ強化**

		// **Y座標が initialY_ より下に行かないようにする**
		//float newY = objectTransform_->transform.translate.y + offsetY;
		//if (newY < initialY_) {
		//	newY = initialY_;  // **下がりすぎたら initialY_ に固定**
		//}

		// **揺れを適用**
		objectTransform_->transform.translate.x += offsetX;
		//objectTransform_->transform.translate.y = newY;  // 修正後のY座標
		objectTransform_->transform.rotate.y += offsetRot;  // **回転も強めに揺らす！**

		// 徐々に揺れを減衰（減衰を遅くして揺れを長くする）
		shakeIntensity_ *= 0.92f;  // **0.85f → 0.92f に変更して長めに揺れる！**
	} else {
		shakeIntensity_ = 0.0f;
	}
}
///=============================================================================
///						当たり判定
///--------------------------------------------------------------
///						接触開始処理
void Player::OnCollisionEnter(BaseObject* other) {
	//========================================
	// 敵
	if (dynamic_cast<BaseEnemy*>(other)) {
		isJumping_ = true;
	}
	//========================================
	// 敵の弾
	if (dynamic_cast<EnemyBullet*>(other)) {
		// ダメージ処理
		hp_--;
		// ヒットリアクション
		isJumping_ = true;
	}
}
///--------------------------------------------------------------
///						接触継続処理
void Player::OnCollisionStay(BaseObject* other) {
	if (dynamic_cast<BaseEnemy*>(other)) {
		isJumping_ = true;
	}
}
///--------------------------------------------------------------
///						接触終了処理
void Player::OnCollisionExit(BaseObject* other) {

}
